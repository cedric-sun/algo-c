#include <stdio.h>
#include <stdlib.h>

static void print_intarr( int *arr, int len ) {
    for ( int i = 0; i < len; i++ ) {
        printf( "%4d", arr[i] );
    }
    putchar( '\n' );
}

static void heap_iter( int n ) {
    int *p = malloc( n * sizeof *p );
    int *c = malloc( n * sizeof *c );
    for ( int i = 0; i < n; i++ ) p[i] = i, c[i] = 0;
    print_intarr( p, n );  // ans found
    for ( int i = 0; i < n; ) {
        if ( c[i] < i ) {
            int j = i & 1 ? c[i] : 0;
            int tmp = p[i];
            p[i] = p[j];
            p[j] = tmp;
            c[i]++;
            i = 0;
            print_intarr( p, n );  // ans found
        } else {
            c[i++] = 0;
        }
    }
    free( p );
    free( c );
}

static void usage( const char *progname ) { fprintf( stderr, "Usage: %s <positive int>\n", progname ); }

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
    heap_iter( n );
}
