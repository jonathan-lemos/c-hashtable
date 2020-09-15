#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

struct ht_entry {
    int present;
    T value;
};

struct hashtable {
    struct ht_entry* elements;
    int(*eq)(T, T);
    size_t(*hash)(T);
    size_t capacity;
    size_t count;
    double load_factor;
};

#ifdef __GNUC__
__attribute__((noreturn))
#endif
void panic(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

// we have functional programming at home
struct hashtable* ht_create(size_t(*hash_function)(T), int(*equality_function)(T, T), size_t initial_capacity) {
    struct hashtable* ret = malloc(sizeof(*ret));

    if (!ret)
        panic("Failed to allocate hashtable");

    if (initial_capacity < 4)
        initial_capacity = 4;

    ret->capacity = initial_capacity;
    ret->elements = calloc(ret->capacity, sizeof(*ret->elements));
    ret->eq = equality_function;
    ret->hash = hash_function;
    ret->count = 0;
    ret->load_factor = 0.5;

    if (!ret->elements)
        panic("Failed to allocate hashtable contents");

    return ret;
}

void array_insert(struct ht_entry* arr, size_t arr_len, size_t(*hash)(T), T elem) {
    size_t index = hash(elem) % arr_len;

    for (size_t i = index, first = 1; first || i != index; i = (i + 1) % arr_len, first = 0)
        if (!arr[i].present) {
            arr[i].present = 1;
            arr[i].value = elem;
            return;
        }

    panic("No empty spots in the ht array");
}

void ht_resize(struct hashtable* ht, size_t new_capacity) {
    if (ht->count > new_capacity)
        panic("new_capacity cannot be below the amount of elements currently in the hashtable");

    struct ht_entry* new_elements = calloc(new_capacity, sizeof(*new_elements));
    if (!new_elements)
        panic("Failed to resize hashtable");

    for (size_t i = 0; i < ht->capacity; ++i)
        if (ht->elements[i].present)
            array_insert(new_elements, new_capacity, ht->hash, ht->elements[i].value);

    free(ht->elements);
    ht->elements = new_elements;
    ht->capacity = new_capacity;
}

void ht_free(struct hashtable* ht) {
    if (!ht) {
        return;
    }

    if (ht->elements)
        free(ht->elements);

    free(ht);
}

int ht_contains(struct hashtable* ht, T element) {
    size_t index = ht->hash(element) % ht->capacity;

    for (size_t i = index, first = 1; first || i != index; i = (i + 1) % ht->capacity, first = 0) {
        if (!ht->elements[i].present)
            return 0;

        if (ht->eq(ht->elements[i].value, element))
            return 1;
    }

    return 0;
}

int ht_insert(struct hashtable* ht, T element) {
    if (!ht)
        return 0;

    if (ht_contains(ht, element))
        return 0;

    if ((ht->count + 1.0) / ht->capacity > ht->load_factor) {
        ht_resize(ht, ht->capacity * 2);
        return ht_insert(ht, element);
    }

    ht->count++;
    array_insert(ht->elements, ht->capacity, ht->hash, element);
    return 1;
}

int ht_delete(struct hashtable* ht, T element) {
    if (!ht)
        return 0;

    if (!ht_contains(ht, element))
        return 0;

    size_t index = ht->hash(element) % ht->capacity;

    for (size_t i = index, first = 1; first || i != index; i = (i + 1) % ht->capacity, first = 0) {
        if (!ht->elements[i].present)
            return 0;

        if (ht->eq(ht->elements[i].value, element)) {
            size_t j = i;

            while (ht->elements[(j + 1) % ht->capacity].present) {
                ht[j] = ht[(j + 1) % ht->capacity];
                j = (j + 1) % ht->capacity;

                if (j == i)
                    break;
            }

            ht->elements[j].present = 0;
            ht->count--;
            return 1;
        }
    }

    return 0;
}
