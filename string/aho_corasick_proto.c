/* aho_corasick for an alphabet of lowercase English letters */

#include <stdlib.h>
#include <string.h>

#define LOA 26

typedef struct _ac_node {
    int pattern_id;
    struct _ac_node *child[LOA];
    struct _ac_node *suffix, *dict_suffix;
} ac_node;

typedef struct {
    size_t begin, pattern_id;
} match;

static inline ac_node *new_node() {
    ac_node *node = calloc( 1, sizeof *node );  // root->suffix = NULL;
    node->pattern_id = -1;
    return node;
}
match *aho_corasick( const char *const text, const char *const *const patterns,
                     size_t patterns_sz, size_t *match_sz ) {
    ac_node *root = new_node();  // root->suffix = NULL;
    size_t *plens = malloc( patterns_sz * sizeof *plens );
    for ( size_t i = 0; i < patterns_sz; i++ ) {
        ac_node *cur = root;
        plens[i] = strlen( patterns[i] );
        for ( const char *p = patterns[i]; *p; p++ ) {
            const int idx = *p - 'a';
            if ( !cur->child[idx] ) cur->child[idx] = new_node();
            cur = cur->child[idx];
        }
        cur->pattern_id = i;
    }

    size_t cap = 0x1000;  // init cap
    ac_node **Q = malloc( cap * sizeof *Q );
    size_t head = 0, tail = 0;  //[head,tail)
    Q[tail++] = root;

    while ( head < tail ) {
        ac_node *cur = Q[head++];
        for ( size_t i = 0; i < LOA; i++ ) {
            if ( cur->child[i] ) {
                for ( ac_node *cnd = cur->suffix; cnd; cnd = cnd->suffix ) {
                    if ( cnd->child[i] ) {
                        cur->child[i]->suffix = cnd->child[i];
                        break;
                    }
                }
                if ( !cur->child[i]->suffix ) cur->child[i]->suffix = root;
                if ( tail >= cap ) {
                    memcpy( Q, Q + head, tail - head );
                    tail -= head;
                    head = 0;
                    Q = realloc( Q, ( cap <<= 1 ) * sizeof *Q );
                }
                Q[tail++] = cur->child[i];
            }
        }
    }

    // second bfs to do suffix compress
    tail = head = 0;  // TODO why not put this into bfs?
    for ( size_t i = 0; i < LOA; i++ ) {
        if ( root->child[i] ) Q[tail++] = root->child[i];
    }

    while ( head < tail ) {
        ac_node *cur = Q[head++];
        for ( ac_node *cnd = cur->suffix; cnd; cnd = cnd->suffix ) {
            if ( cnd->dict_suffix ) {
                cur->dict_suffix = cnd->dict_suffix;
                break;
            }
            if ( ~cnd->pattern_id ) {
                cur->dict_suffix = cnd;
                break;
            }
        }
        for ( size_t i = 0; i < LOA; i++ ) {
            if ( cur->child[i] ) {
                if ( tail >= cap ) {
                    memcpy( Q, Q + head, tail - head );
                    tail -= head;
                    head = 0;
                }
                Q[tail++] = cur->child[i];
            }
        }
    }

    // begin search
    size_t mcap = 32, msz = 0;
    match *matches = malloc( mcap * sizeof *matches );
    const size_t tlen = strlen( text );
    ac_node *cur = root;
    for ( size_t i = 0; i < tlen; i++ ) {
        if ( cur == root && !cur->child[text[i] - 'a'] ) continue;
        while ( cur && !cur->child[text[i] - 'a'] ) {
            cur = cur->suffix;
        }
        if ( cur == NULL ) {
            cur = root;
            continue;
        }
        // cur can proceed now
        cur = cur->child[text[i] - 'a'];
        for ( ac_node *p = cur; p; p = p->dict_suffix ) {
            if ( ~p->pattern_id ) {
                if ( msz >= mcap ) {
                    matches =
                        realloc( matches, ( mcap <<= 1 ) * sizeof *matches );
                }
                matches[msz++] =
                    ( match ){i - plens[p->pattern_id] + 1, p->pattern_id};
            }
        }
    }

    free( plens );
    head = tail = 0;
    Q[tail++] = root;
    while ( head < tail ) {
        ac_node *cur = Q[head++];
        for ( size_t i = 0; i < LOA; i++ ) {
            if ( cur->child[i] ) {
                Q[tail++] = cur->child[i];
            }
        }
        free( cur );
    }
    free( Q );

    *match_sz = msz;
    return matches;
}

#include <stdio.h>
int main( void ) {
    const char *text =
        "loremipsumdolorsitametconsecteturadipiscingelitduissitametsapienul"
        "tric"
        "iesestvolutpatmalesuadacrassemperestsedodioeuismodcursusquisquevel"
        "volu"
        "tpateratsedmalesuadapretiummetussedconsequatpellentesqueegeteleife"
        "ndli"
        "berononfermentumodioinhachabitasseplateadictumstnullamurnaelitlaci"
        "niav"
        "itaecommodonecmattisnecurnasedfringillaetpurusvitaemattismorbimatt"
        "ispr"
        "etiumelitetmollisnibhvolutpatacetiamidtinciduntliberofusceutimperd"
        "iete"
        "rosvelultricesligulavestibulumanteipsumprimisinfaucibusorciluctuse"
        "tult"
        "ricesposuerecubiliacuraeinterdumetmalesuadafamesacanteipsumprimisi"
        "nfau"
        "cibuspraesenteuodioidestfacilisismolestieaeneanquistemporodionecul"
        "tric"
        "esquamsuspendisseidmaurisarcuvestibulumsedmolestieelitaportaligula"
        "prae"
        "sentauctorporttitorporttitormaecenasidconsecteturpurusduisconsequa"
        "tnul"
        "lavelvenenatisfacilisisnequelectusconvallisjustositametvehiculamag"
        "nanu"
        "ncsitametsemdonecluctusaliquammiuteleifendclassaptenttacitisociosq"
        "uadl"
        "itoratorquentperconubianostraperinceptoshimenaeosmorbinonfelisiner"
        "atco"
        "nsequatdapibusintegervitaeerosmassadoneclaoreetvariusrisusettempus"
        "mief"
        "ficiturutetiamatarcuaduipretiumefficiturmaurisconsecteturegetturpi"
        "sact"
        "empusdonecpulvinarsodalesgravidainconguediamutcondimentumeuismodve"
        "stib"
        "ulumacvolutpatjustophasellusaliquetdignissimodionecornareutaturnai"
        "psum"
        "pellentesquecongueduiaugueeuultricieslacusullamcorpervariusmorbive"
        "lcur"
        "suslacusmaecenasegestasnuncarcueusemperloremsuscipitquiscrasvulput"
        "ates"
        "emperjustovitaeauctorelitrhoncusquissedporttitoretorcietcondimentu"
        "mnul"
        "laatpellentesquelacusplaceratconvallisduimaecenassitamettemporturp"
        "issu"
        "spendissenonmetusnislmaecenasauctormattismisitametiaculisnuncmauri"
        "slib"
        "eronisimattisegetsollicitudineumalesuadaidvelitdonecutviverraarcuo"
        "rciv"
        "ariusnatoquepenatibusetmagnisdisparturientmontesnasceturridiculusm"
        "ussu"
        "spendissenecjustoasapienrutrumornarevitaeidipsumvivamusestdiamfeug"
        "iatn"
        "ecegestassitametornareaturpispraesentnonipsumelementumcondimentumn"
        "ulla"
        "idmollisdolornamconsequatmaurisvitaeultriciesportaliberomifacilisi"
        "smin"
        "ecfringillanuncmagnasuscipitnequenuncullamcorperipsumamitristiques"
        "agit"
        "tisintegerdictumrisuseuvehiculamattislacusmagnatempormaurisegetsod"
        "ales"
        "felisnisletliberophasellusidligulainmaurisvenenatisaliquametiambib"
        "endu"
        "mlectusnecmassarutrumsedcursusdolortinciduntsuspendisseconsequatcu"
        "rsus"
        "felissitametbibendumvelitfinibusegetdonecvitaetristiquerisusvivamu"
        "squi"
        "seratvolutpatsemlobortiselementumetiamenimmagnavariusvitaehendreri"
        "tnec"
        "consectetureumidoneclobortissedrisusetconsecteturphasellusaaliquet"
        "maur"
        "issedmalesuadavehiculasapienquiselementumexsagittisvitaeorcivarius"
        "nato"
        "quepenatibusetmagnisdisparturientmontesnasceturridiculusmussuspend"
        "isse"
        "dignissimvelitduiatultriciesorcirutrumacmaurisultriciessitametnisi"
        "inpr"
        "etiumcrasdignissimdiamatmollisegestascrasnibhurnaposuereacmetusutm"
        "oles"
        "tiecursuslectusnullasagittisacipsumnectristique";
    const char *patterns[] = {"lorem", "nec", "donec", "mattis", "ipsum"};
    size_t match_sz;
    match *matches = aho_corasick(
        text, patterns, sizeof patterns / sizeof *patterns, &match_sz );
    for ( size_t i = 0; i < match_sz; i++ ) {
        printf( "matched: %10s at index %lu\n", patterns[matches[i].pattern_id],
                matches[i].begin );
    }
    free( matches );
}
