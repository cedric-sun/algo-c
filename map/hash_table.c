/**********************************************************
 *                       HASH TABLE
 **********************************************************/
#include <stdint.h>
#include <stdlib.h>

#define MAX_LF 0.75

typedef struct _ht_entry {
    size_t k;
    size_t v;
    struct _ht_entry *next;
} ht_entry;

typedef struct {
    size_t cap;
    double lf;
    size_t ent_cnt;
    ht_entry **buckets;
} hash_table;

static void ht_put( hash_table *ht, size_t k, size_t v ) {
    ht->lf = (double)ht->ent_cnt / ht->cap;
    if ( ht->lf > MAX_LF ) {  // extend + rehash
        size_t new_cap = ht->cap << 1;
        ht_entry **new_buckets = calloc( new_cap, sizeof( *new_buckets ) );
        for ( size_t i = 0; i < ht->cap; i++ ) {
            ht_entry *cur = *( ht->buckets + i );
            while ( cur ) {
                size_t ni = cur->k & ( new_cap - 1 );
                ht_entry *next = cur->next;
                cur->next = new_buckets[ni];
                new_buckets[ni] = cur;
                cur = next;
            }
        }
        free( ht->buckets );
        ht->buckets = new_buckets;
        ht->cap = new_cap;
    }
    size_t i = k & ( ht->cap - 1 );

    for ( ht_entry *cur = *( ht->buckets + i ); cur; cur = cur->next ) {
        if ( cur->k == k ) {  // duplication
            cur->v = v;
            return;
        }
    }

    ht_entry *new_ht_entry = malloc( sizeof( *new_ht_entry ) );
    new_ht_entry->k = k;
    new_ht_entry->v = v;
    new_ht_entry->next = ht->buckets[i];
    ht->buckets[i] = new_ht_entry;
    ht->ent_cnt++;
}

// return SIZE_MAX for empty bucket
static size_t ht_get( hash_table *ht, size_t k ) {
    size_t i = k & ( ht->cap - 1 );
    if ( ht->buckets[i] ) {
        for ( ht_entry *cur = *( ht->buckets + i ); cur; cur = cur->next ) {
            if ( cur->k == k ) {
                return cur->v;
            }
        }
    }
    return SIZE_MAX;
}

static hash_table *ht_init() {
    hash_table *ht = malloc( sizeof( *ht ) );
    ht->cap = 128;
    ht->lf = 0;
    ht->ent_cnt = 0;
    ht->buckets = calloc( ht->cap, sizeof( *ht->buckets ) );
    return ht;
}

static void ht_free( hash_table *ht ) {
    for ( size_t i = 0; i < ht->cap; i++ ) {
        ht_entry *cur = *( ht->buckets + i );
        while ( cur ) {
            // if v is a pointer to heap, free v here
            ht_entry *next = cur->next;
            free( cur );
            cur = next;
        }
    }
    free( ht->buckets );
    free( ht );
}

/**************************************
 *              UNIT TEST
 **************************************/
typedef struct {
    size_t k;
    size_t v;
} kvpair;

#include <stdio.h>
#include <time.h>

void test0() {
    puts( "============= test 0 ================" );
    srand( time( NULL ) );
#define N 100000
    kvpair **arr = malloc( N * sizeof *arr );

    hash_table *ht0 = ht_init();

    for ( size_t i = 0; i < N; i++ ) {
        kvpair *pair = malloc( sizeof *pair );
        *pair = ( kvpair ){rand(), rand()};
        arr[i] = pair;
        ht_put( ht0, pair->k, pair->v );
    }

    for ( size_t i = 0; i < N; i++ ) {
        kvpair *cur = arr[i];
        size_t v = ht_get( ht0, cur->k );
        if ( v != cur->v ) {
            fprintf( stderr,
                     "Possible Duplicate k. Expect: %lu, but was %lu.\n",
                     cur->v, v );
        }
        free( cur );
    }

    printf( "Entries number = %lu.\n", ht0->ent_cnt );
    printf( "Buckets number = %lu.\n", ht0->cap );
    ht_free( ht0 );
    free( arr );
}

void test1() {
    puts( "================= test 1 =================" );
    srand( time( NULL ) );
#define N 100000
    size_t *arr = malloc( N * sizeof *arr );

    hash_table *ht0 = ht_init();

    for ( size_t i = 0; i < N; i++ ) {
        arr[i] = rand();
        ht_put( ht0, ( size_t )( arr + i ), arr[i] );
    }

    for ( size_t i = 0; i < N; i++ ) {
        size_t v = ht_get( ht0, ( size_t )( arr + i ) );
        if ( v != arr[i] ) {
            fputs( "FAILED!", stderr );
        }
    }

    printf( "Entries number = %lu.\n", ht0->ent_cnt );
    printf( "Buckets number = %lu.\n", ht0->cap );
    ht_free( ht0 );
    free( arr );
}

int main() {
    test0();
    test1();
}
