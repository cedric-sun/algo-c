/* The backtracking method proposed by sedgewick,
 *
 * The idea is each time fix the last element, and generate permutations
 * of all previous elements.
 *
 * Not very useful:
 *      The order of generation is not lexicographical.
 *      Too many swap.
 *
 * Interesting thing is that you can start from any permutation of any numbers.
 */

#include <stdio.h>
#include <stdlib.h>

static void usage( const char *progname ) { fprintf( stderr, "Usage: %s <positive int>\n", progname ); }

static int *p, plen;

static void print_intarr( int *arr, int len ) {
    for ( int i = 0; i < len; i++ ) {
        printf( "%4d", arr[i] );
    }
    putchar( '\n' );
}

static inline void swap( int i, int j ) {
    int tmp = p[i];
    p[i] = p[j];
    p[j] = tmp;
}

static void gen( int i ) {
    if ( !i ) {
        print_intarr( p, plen );
        return;//actually okay to omit this return
    }
    for ( int j = 0; j <= i; j++ ) {
        swap( i, j );
        gen( i - 1 );
        swap( i, j );
    }
}

int main( int argc, char **argv ) {
    if ( argc != 2 ) {
        usage( argv[0] );
        return -1;
    }
    int n = atoi( argv[1] );
    if ( n <= 0 ) {
        usage( argv[0] );
        return -1;
    }
    p = malloc( ( plen = n ) * sizeof *p );
#define BEGIN 1
    for ( int i = BEGIN; i < BEGIN + n; i++ ) p[i % n] = i;
    gen( n - 1 );
    free( p );
    return 0;
}
