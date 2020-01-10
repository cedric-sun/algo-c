#include <stdio.h>
#include <stdlib.h>

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

// static void heap_iter( int n ) {}

static void heap_recur( int n ) {
    if ( !n ) {
        print_intarr( p, plen );
        return;
    }
    for ( int i = 0; i <= n; i++ ) {
        heap_recur( n - 1 );
        swap( n & 1 ? i : 0, n );
    }
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
    p = malloc( ( plen = n ) * sizeof *p );
    for ( int i = 0; i < n; i++ ) p[i] = i;
    heap_recur( n - 1 );
    free( p );
    return 0;
}
