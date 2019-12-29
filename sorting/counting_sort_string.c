/* counting sort is good for sorting string
 * because each element in a string (a char)
 * has fixed-length (1 byte), and the value range
 * is usually small. (for English alphabet it's 26)
 *
 * And since string is a array of naked key without
 * associated data, you don't really need to derive the
 * actual index from the counting, thus the last 2 loops
 * in the classic counting sort can be omitted.
 *
 * The following demonstrate sorting a string
 * of lowercase letters.
 */

#include <string.h>
#define LOA 26
void counting_sort_string( char *str ) {
    size_t len = strlen( str );
    size_t count[LOA];
    memset( count, 0, sizeof count );
    for ( size_t i = 0; i < len; i++ ) count[str[i] - 'a']++;
    size_t sz = 0;
    for ( size_t i = 0; i < LOA; i++ )
        while ( count[i]-- ) str[sz++] = i + 'a';
}

#include <stdio.h>
int main() {
    char str[] = "goodmorning";
    counting_sort_string( str );
    printf( "%s\n", str );
}
