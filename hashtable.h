#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include <stddef.h>

// we have generics at home
#define T const char*

struct hashtable;
typedef struct hashtable hashtable;

struct hashtable* ht_create(size_t(*hash_function)(T), int(*equality_function)(T, T), size_t initial_capacity);
void ht_free(struct hashtable* ht);
int ht_contains(struct hashtable* ht, T element);
int ht_insert(struct hashtable* ht, T element);
int ht_delete(struct hashtable* ht, T element);

#endif