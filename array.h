#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The zero-based index of item in the sorted base, if key is found;
 * otherwise, a negative number that is the bitwise complement of the index of
 * the next element that is larger than key or, if there is no larger element,
 * the bitwise complement of num.
 */
int binary_search(const void* key, const void* base, size_t num, size_t size, int (*compar)(const void*, const void*));

bool permutation(int* array, size_t num, bool (*callback)(const int*, size_t num));

void print_array(int* array, size_t num, FILE* fd);

#ifdef __cplusplus
}
#endif
