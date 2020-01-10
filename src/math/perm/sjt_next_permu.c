/* O(n*(n!)) Permutation Gen
 *
 * The idea is similar to SJT, but not care about the
 * order of generated permutations.
 */

#include <stdlib.h>
#include <string.h>

static const size_t fac_seq[] = {1,   1,    2,     6,      24,      120,
                                 720, 5040, 40320, 362880, 3628800, 39916800};

#define MAX_LEN 9
static int ***permu[MAX_LEN];
// generate all permutations of [0,len) for len in [1,MAX_LEN]
void unordered_sjt( const size_t sz ) {
    permu = malloc(( MAX_ORDER+1 )*sizeof *permu);
    permu[1]=malloc(fac_seq[1]*sizeof **permu);
    permu[1][0]=malloc()
    for (size_t i=2;i<=sz;i++) {

    }
}

int **permu_gen( const int *arr, const size_t sz ) {
    // trick is to just generate the permutation of indices
    int **i_permu = sjt( sz );
    int **ans = malloc( fac_seq[sz] * sizeof *ans );
    for ( size_t i = 0; i < fac_seq[sz]; i++ ) {
        ans[i] = malloc( sz * sizeof **ans );
        for ( size_t j = 0; j < sz; j++ ) {
            ans[i][j] = arr[i_permu[i][j]];
        }
    }
    return ans;
}

#include <stdio.h>
int main() {
    const int arr[] = {114, 514, 128, 256};
#define ARR_SZ ( sizeof arr / sizeof *arr )
    int **permu = permu_gen( arr, ARR_SZ );
    for ( size_t i = 0; i < fac_seq[ARR_SZ]; i++ ) {
        for ( size_t j = 0; j < ARR_SZ; j++ ) {
            printf( "%6d", permu[i][j] );
        }
        putchar( '\n' );
        free( permu[i] );
    }
    free( permu );
}
