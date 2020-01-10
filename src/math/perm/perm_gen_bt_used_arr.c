/* Somehow intuitive implementation to generate permutation using backtracking.
 *
 * This method naturally generates permutations of [0,sz) in lexicographical order.
 */
#include <stdlib.h>
#include <string.h>

static const size_t fac_seq[] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800};

static int **ans;

static int *buffer, *used, g_sz, ans_sz, nbyte;

void dfs( int i ) {
    if ( i == g_sz ) {
        ans[ans_sz++] = memcpy( malloc( nbyte ), buffer, nbyte );
        return;
    }
    for ( int j = 0; j < g_sz; j++ ) {
        if ( !used[j] ) {
            buffer[i] = j;
            used[j] = 1;
            dfs( i + 1 );
            used[j] = 0;
        }
    }
}

//[0,sz)
void permu_gen( int sz ) {
    g_sz = sz;
    nbyte = sz * sizeof( int );
    used = malloc( nbyte );
    buffer = malloc( nbyte );
    memset( used, 0, nbyte );

    ans = malloc( fac_seq[sz] * sizeof *ans );
    ans_sz = 0;

    dfs( 0 );

    free( buffer );
    free( used );
}

#include <stdio.h>
#define N 4
int main() {
    permu_gen( N );
    for ( int i = 0; i < ans_sz; i++ ) {
        for ( int j = 0; j < N; j++ ) printf( "%3d", ans[i][j] );
        putchar('\n');
        free( ans[i] );
    }
    free( ans );
}
