#include <stdlib.h>
#include <string.h>

#define TRIE_MAX_NODE_N 100008
#define ALPHA_SZ 26

// .bss section is inited with zero
static int tr[TRIE_MAX_NODE_N][ALPHA_SZ];
static int suffix[TRIE_MAX_NODE_N];
static int pid[TRIE_MAX_NODE_N];

typedef struct {
    size_t begin, pid;
} match;

const match *aho_corasick( const char *const text, const char *const *const patterns, const int patterns_n,
                           int *const matches_n ) {
    int tot = 0;
    memset( tr, 0, sizeof tr );
    memset( pid, 0, sizeof pid );
    // root is 0

    int *const plens = malloc( patterns_n * sizeof *plens );
    for ( int i = 0; i < patterns_n; i++ ) {
        const int len = plens[i] = strlen( patterns[i] );
        int cur = 0;
        for ( int j = 0; j < len; j++ ) {
            const int idx = patterns[i][j] - 'a';
            if ( !tr[cur][idx] ) tr[cur][idx] = ++tot;
            cur = tr[cur][idx];
        }
        pid[cur] = i + 1;
    }

    // TODO make sure root never get into the queue
    int *const Q = malloc( tot * sizeof *Q );
    int head = 0, tail = 0;
    for ( int i = 0; i < ALPHA_SZ; i++ )
        if ( tr[0][i] ) {
            // the suffix of root doesn't matter
            // but the suffix of the first level nodes
            // has to be root
            suffix[tr[0][i]] = 0;
            Q[tail++] = tr[0][i];
        }
    while ( head < tail ) {
        int cur = Q[head++];
        for ( int i = 0; i < ALPHA_SZ; i++ ) {
            if ( tr[cur][i] ) {
                suffix[tr[cur][i]] = tr[suffix[cur]][i];
                Q[tail++] = tr[cur][i];
            } else {
                tr[cur][i] = tr[suffix[cur]][i];
            }
        }
    }

    // the meaning of suffix array changes here

    int *visited = calloc( tot + 1, sizeof *visited );
    head = tail = 0;

    for ( int i = 0; i < ALPHA_SZ; i++ )
        if ( tr[0][i] ) {
            Q[tail++] = tr[0][i];
            visited[tr[0][i]] = 1;
        }
    while ( head < tail ) {
        int cur = Q[head++];
        if ( !pid[suffix[cur]] ) {
            suffix[cur] = suffix[suffix[cur]];
            // the property of bfs guarantees that suffix[suffix[x]] is always already dict_suffix[suffix[x]]
        }
        for ( int i = 0; i < ALPHA_SZ; i++ ) {
            if ( tr[cur][i] && !visited[tr[cur][i]]) {
                Q[tail++] = tr[cur][i];
                visited[tr[cur][i]] = 1;
            }
        }
    }
    free(visited);

    // begin search
    int mcap = 128, msz = 0;
    match *matches = malloc( mcap * sizeof *matches );
    const int tlen = strlen( text );
    for ( int i = 0, cur = 0; i < tlen; i++ ) {
        cur = tr[cur][text[i] - 'a'];
        for ( int x = cur; x; x = suffix[x] ) {
            if ( pid[x] ) {
                if ( msz >= mcap ) matches = realloc( matches, ( mcap <<= 1 ) * sizeof *matches );
                matches[msz++] = ( match ){i - plens[pid[x] - 1] + 1, pid[x] - 1};
            }
        }
    }

    free( plens );
    free( Q );

    *matches_n = msz;
    return matches;
}

#include <stdio.h>
int main() {
    const char *text =
        "loremipsumdolorsitametconsecteturadipiscingelitduissitametsapienultriciesestvolutpatmalesuadacrassemperestsedo"
        "dioeuismodcursusquisquevelvolutpateratsedmalesuadapretiummetussedconsequatpellentesqueegeteleifendliberononfer"
        "mentumodioinhachabitasseplateadictumstnullamurnaelitlaciniavitaecommodonecmattisnecurnasedfringillaetpurusvita"
        "emattismorbimattispretiumelitetmollisnibhvolutpatacetiamidtinciduntliberofusceutimperdieterosvelultricesligula"
        "vestibulumanteipsumprimisinfaucibusorciluctusetultricesposuerecubiliacuraeinterdumetmalesuadafamesacanteipsump"
        "rimisinfaucibuspraesenteuodioidestfacilisismolestieaeneanquistemporodionecultricesquamsuspendisseidmaurisarcuv"
        "estibulumsedmolestieelitaportaligulapraesentauctorporttitorporttitormaecenasidconsecteturpurusduisconsequatnul"
        "lavelvenenatisfacilisisnequelectusconvallisjustositametvehiculamagnanuncsitametsemdonecluctusaliquammiuteleife"
        "ndclassaptenttacitisociosquadlitoratorquentperconubianostraperinceptoshimenaeosmorbinonfelisineratconsequatdap"
        "ibusintegervitaeerosmassadoneclaoreetvariusrisusettempusmiefficiturutetiamatarcuaduipretiumefficiturmauriscons"
        "ecteturegetturpisactempusdonecpulvinarsodalesgravidainconguediamutcondimentumeuismodvestibulumacvolutpatjustop"
        "hasellusaliquetdignissimodionecornareutaturnaipsumpellentesquecongueduiaugueeuultricieslacusullamcorpervariusm"
        "orbivelcursuslacusmaecenasegestasnuncarcueusemperloremsuscipitquiscrasvulputatesemperjustovitaeauctorelitrhonc"
        "usquissedporttitoretorcietcondimentumnullaatpellentesquelacusplaceratconvallisduimaecenassitamettemporturpissu"
        "spendissenonmetusnislmaecenasauctormattismisitametiaculisnuncmaurisliberonisimattisegetsollicitudineumalesuada"
        "idvelitdonecutviverraarcuorcivariusnatoquepenatibusetmagnisdisparturientmontesnasceturridiculusmussuspendissen"
        "ecjustoasapienrutrumornarevitaeidipsumvivamusestdiamfeugiatnecegestassitametornareaturpispraesentnonipsumeleme"
        "ntumcondimentumnullaidmollisdolornamconsequatmaurisvitaeultriciesportaliberomifacilisisminecfringillanuncmagna"
        "suscipitnequenuncullamcorperipsumamitristiquesagittisintegerdictumrisuseuvehiculamattislacusmagnatempormaurise"
        "getsodalesfelisnisletliberophasellusidligulainmaurisvenenatisaliquametiambibendumlectusnecmassarutrumsedcursus"
        "dolortinciduntsuspendisseconsequatcursusfelissitametbibendumvelitfinibusegetdonecvitaetristiquerisusvivamusqui"
        "seratvolutpatsemlobortiselementumetiamenimmagnavariusvitaehendreritnecconsectetureumidoneclobortissedrisusetco"
        "nsecteturphasellusaaliquetmaurissedmalesuadavehiculasapienquiselementumexsagittisvitaeorcivariusnatoquepenatib"
        "usetmagnisdisparturientmontesnasceturridiculusmussuspendissedignissimvelitduiatultriciesorcirutrumacmaurisultr"
        "iciessitametnisiinpretiumcrasdignissimdiamatmollisegestascrasnibhurnaposuereacmetusutmolestiecursuslectusnulla"
        "sagittisacipsumnectristique";
    const char *patterns[] = {"lorem", "nec", "donec", "mattis", "ipsum"};
    int msz;
    const match *matches = aho_corasick( text, patterns, sizeof patterns / sizeof *patterns, &msz );
    for ( int i = 0; i < msz; i++ ) {
        printf( "matched: %10s at index %lu\n", patterns[matches[i].pid], matches[i].begin );
    }
    free( (void *)matches );
}
