#include "hashtable.h"
#include <stdio.h>
#include <string.h>


size_t hash_function(const char* s) {
    size_t accumulator = 0;
    for (; *s; ++s)
        accumulator = accumulator * 31 + *s;
    return accumulator;
}

int eq(const char* s1, const char* s2) {
    return !strcmp(s1, s2);
}

int main(void) {
    hashtable* ht = ht_create(hash_function, eq, 4);

    puts("insert");
    printf("%d\n", ht_insert(ht, "abc"));
    printf("%d\n", ht_insert(ht, "abc"));
    printf("%d\n", ht_insert(ht, "def"));
    printf("%d\n", ht_insert(ht, "ggg"));
    printf("%d\n", ht_insert(ht, "ghi"));
    printf("%d\n", ht_insert(ht, "444"));

    puts("contains");
    printf("%d\n", ht_contains(ht, "abc"));
    printf("%d\n", ht_contains(ht, "def"));
    printf("%d\n", ht_contains(ht, "bbq"));

    puts("delete");
    printf("%d\n", ht_delete(ht, "aqq"));
    printf("%d\n", ht_delete(ht, "def"));

    puts("contains");
    printf("%d\n", ht_contains(ht, "abc"));
    printf("%d\n", ht_contains(ht, "def"));
    printf("%d\n", ht_contains(ht, "bbq"));

    ht_free(ht);
}