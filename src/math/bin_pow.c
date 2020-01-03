#include <stdint.h>

int64_t bin_pow( int64_t base, int64_t exp ) {
    int64_t res = 1;
    while ( exp > 0 ) {
        if ( exp & 1 ) res = res * base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

// base^exp % mod
int64_t bin_pow_mod( int64_t base, int64_t exp, int64_t mod ) {
    base %= mod;
    int64_t res = 1;
    while ( exp > 0 ) {
        if ( exp & 1 ) res = res * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return res;
}

#include <stdio.h>
int main() {
    printf( "%ld\n", bin_pow( -2, 61 ) );
    printf( "%ld\n", bin_pow_mod( 2, 11341,992) );
}
