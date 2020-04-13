#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* The zero-based index of item in the sorted base, if key is found; 
* otherwise, a negative number that is the bitwise complement of the index of the next element that is larger than key or, 
* if there is no larger element, the bitwise complement of num.
*/
int binary_search(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *));

#ifdef __cplusplus
}
#endif
