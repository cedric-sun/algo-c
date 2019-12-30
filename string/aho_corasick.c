/* aho_corasick for an alphabet of lowercase English letters */

#include <string.h>

#define LOA 26

typedef struct _ac_node {
    const char *str;
    struct _ac_node *child[LOA];
    struct _ac_node *suffix, *dict_suffix;
} ac_node;

typedef struct {
    size_t begin, pattern_id;
} match;

void gao(ac_node *cur) {
    for (size_t i=0;i<LOA;i++)
        if (cur->child[i])
            gao(cur->child[i]);
}

match *aho_corasick( const char *text, const char **patterns,
                     size_t patterns_sz, size_t *match_sz ) {
    ac_node *root = calloc( 1, sizeof *root );
    for ( size_t i = 0; i < patterns_sz; i++ ) {
        ac_node *cur = root;
        for ( char *p = patterns[i]; *p; p++ ) {
            const int idx = *p - 'a';
            if ( !cur->child[idx] )
                cur->child[idx] = calloc( 1, sizeof( ac_node ) );
            cur = cur->child[idx];
        }
        cur->str = pattern[i];
    }


    match *matches = malloc( 32 * sizeof *matches );
    int sz = 0;
    // GOGOGO
    *match_sz = sz;
    return matches;
}

int main( void ) {
    const char *text =
        "loremipsumdolorsitametconsecteturadipiscingelitduissitametsapienultric"
        "iesestvolutpatmalesuadacrassemperestsedodioeuismodcursusquisquevelvolu"
        "tpateratsedmalesuadapretiummetussedconsequatpellentesqueegeteleifendli"
        "berononfermentumodioinhachabitasseplateadictumstnullamurnaelitlaciniav"
        "itaecommodonecmattisnecurnasedfringillaetpurusvitaemattismorbimattispr"
        "etiumelitetmollisnibhvolutpatacetiamidtinciduntliberofusceutimperdiete"
        "rosvelultricesligulavestibulumanteipsumprimisinfaucibusorciluctusetult"
        "ricesposuerecubiliacuraeinterdumetmalesuadafamesacanteipsumprimisinfau"
        "cibuspraesenteuodioidestfacilisismolestieaeneanquistemporodionecultric"
        "esquamsuspendisseidmaurisarcuvestibulumsedmolestieelitaportaligulaprae"
        "sentauctorporttitorporttitormaecenasidconsecteturpurusduisconsequatnul"
        "lavelvenenatisfacilisisnequelectusconvallisjustositametvehiculamagnanu"
        "ncsitametsemdonecluctusaliquammiuteleifendclassaptenttacitisociosquadl"
        "itoratorquentperconubianostraperinceptoshimenaeosmorbinonfelisineratco"
        "nsequatdapibusintegervitaeerosmassadoneclaoreetvariusrisusettempusmief"
        "ficiturutetiamatarcuaduipretiumefficiturmaurisconsecteturegetturpisact"
        "empusdonecpulvinarsodalesgravidainconguediamutcondimentumeuismodvestib"
        "ulumacvolutpatjustophasellusaliquetdignissimodionecornareutaturnaipsum"
        "pellentesquecongueduiaugueeuultricieslacusullamcorpervariusmorbivelcur"
        "suslacusmaecenasegestasnuncarcueusemperloremsuscipitquiscrasvulputates"
        "emperjustovitaeauctorelitrhoncusquissedporttitoretorcietcondimentumnul"
        "laatpellentesquelacusplaceratconvallisduimaecenassitamettemporturpissu"
        "spendissenonmetusnislmaecenasauctormattismisitametiaculisnuncmaurislib"
        "eronisimattisegetsollicitudineumalesuadaidvelitdonecutviverraarcuorciv"
        "ariusnatoquepenatibusetmagnisdisparturientmontesnasceturridiculusmussu"
        "spendissenecjustoasapienrutrumornarevitaeidipsumvivamusestdiamfeugiatn"
        "ecegestassitametornareaturpispraesentnonipsumelementumcondimentumnulla"
        "idmollisdolornamconsequatmaurisvitaeultriciesportaliberomifacilisismin"
        "ecfringillanuncmagnasuscipitnequenuncullamcorperipsumamitristiquesagit"
        "tisintegerdictumrisuseuvehiculamattislacusmagnatempormaurisegetsodales"
        "felisnisletliberophasellusidligulainmaurisvenenatisaliquametiambibendu"
        "mlectusnecmassarutrumsedcursusdolortinciduntsuspendisseconsequatcursus"
        "felissitametbibendumvelitfinibusegetdonecvitaetristiquerisusvivamusqui"
        "seratvolutpatsemlobortiselementumetiamenimmagnavariusvitaehendreritnec"
        "consectetureumidoneclobortissedrisusetconsecteturphasellusaaliquetmaur"
        "issedmalesuadavehiculasapienquiselementumexsagittisvitaeorcivariusnato"
        "quepenatibusetmagnisdisparturientmontesnasceturridiculusmussuspendisse"
        "dignissimvelitduiatultriciesorcirutrumacmaurisultriciessitametnisiinpr"
        "etiumcrasdignissimdiamatmollisegestascrasnibhurnaposuereacmetusutmoles"
        "tiecursuslectusnullasagittisacipsumnectristique";
    const char *patterns[] = {"lorem", "nec", "donec", "mattis"};
    int match_sz;
    match *matches = aho_corasick( text, patterns, &match_sz );
    for ( size_t i = 0; i < match_sz; i++ ) {
        printf( "matched: %20s at index %d\n", patterns[matches[i].pattern_id],
                matches[i].begin );
    }
}
