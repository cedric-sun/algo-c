#include <stddef.h>
#include <stdlib.h>

/* Counting Sort
 *
 * stable: yes
 *
 * remember to free the returned pointer
 *
 * all elements in arr has to be non-negative,
 *
 * memo: shift keys in arr so that the range is [0,MAX-MIN]
 */
unsigned int *counting_sort( unsigned int *arr, size_t size ) {
    unsigned int max = 0;
    for ( size_t i = 0; i < size; i++ )
        if ( max < arr[i] ) max = arr[i];
    unsigned int *count = calloc( max + 1, sizeof *count );
    unsigned int *ans = malloc( size * sizeof *ans );
    for ( size_t i = 0; i < size; i++ ) count[arr[i]]++;
    for ( size_t i = 1; i < max + 1; i++ ) count[i] += count[i - 1];
    // go backward to make a stable sorting
    for ( ptrdiff_t i = size - 1; i >= 0; i-- ) {
        count[arr[i]]--;
        ans[count[arr[i]]] = arr[i];
    }
    free( count );
    return ans;
}

#include <stdio.h>
#include <time.h>
int main() {
#define N 10000
#define MOD 101
    srand( time( NULL ) );
    unsigned int *arr = malloc( N * sizeof *arr );
    for ( size_t i = 0; i < N; i++ ) arr[i] = rand() % MOD;
    unsigned int *ans = counting_sort( arr, N );
    for ( size_t i = 1; i < N; i++ ) {
        if ( ans[i] < ans[i - 1] ) {
            fputs( "panic", stderr );
            return -1;
        }
    }
    puts( "pass" );
    free( ans );
    return 0;
}
