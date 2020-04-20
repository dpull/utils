#pragma once
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hashmap;
struct hashmap_value 
{
    intptr_t key;
    intptr_t value;
};

struct hashmap* hashmap_create(size_t value_max_count);
void hashmap_destory(struct hashmap* hashmap);

struct hashmap_value* hashmap_add(struct hashmap* hashmap, intptr_t key);
struct hashmap_value* hashmap_get(struct hashmap* hashmap, intptr_t key);
int hashmap_remove(struct hashmap* hashmap, intptr_t key);

typedef bool (hashmap_callback)(struct hashmap_value* hashmap_value);
void hashmap_traverse(struct hashmap* hashmap, hashmap_callback* callback);

#ifdef __cplusplus
}
#endif
