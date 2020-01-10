#include "../../src/string/aho_corasick_optimal.c"

#include <stdio.h>
#include <stdlib.h>

static size_t zero;

int main() {
    FILE *text_stream = fopen( "text.txt", "r" );
    FILE *patterns_stream = fopen( "patterns.txt", "r" );

    char *text = NULL;
    zero = 0;
    ssize_t tlen = getline( &text, &zero, text_stream );
    text[tlen - 1] = 0;  // discard newline

    size_t ptns_n = 0, ptns_cap = 1000;
    const char **patterns = malloc( ptns_cap * sizeof *patterns );

    char *pattern_line;
    ssize_t pl_len;
    while ( pattern_line = NULL, zero = 0, ~( pl_len = getline( &pattern_line, &zero, patterns_stream ) ) ) {
        if ( ptns_n >= ptns_cap ) patterns = realloc( patterns, ( ptns_cap <<= 1 ) * sizeof *patterns );
        pattern_line[pl_len - 1] = 0;
        patterns[ptns_n++] = pattern_line;
    }
    free( pattern_line );
    fclose( text_stream );
    fclose( patterns_stream );

    int msz;
    const match *matches = aho_corasick( text, patterns, ptns_n, &msz );
    for ( int i = 0; i < msz; i++ ) {
        printf( "matched:\t%s\tat\tindex\t%lu\n", patterns[matches[i].pid], matches[i].begin );
    }
    free( (void *)matches );

    free( text );
    for ( size_t i = 0; i < ptns_n; i++ ) free( (void *)patterns[i] );
    free( patterns );
}
