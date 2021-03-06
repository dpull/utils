#include "hashmap.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define HASHMAP_NULL (-1)
#define HASHMAP_SENTINEL (-2)

struct hashmap {
	size_t capacity;
	struct hashmap_value hashmap_value[0];
};

struct hashmap* hashmap_create(size_t capacity) {
	size_t length = sizeof(struct hashmap_value) * capacity;
	if (length == 0)
		return NULL;

	struct hashmap* hashmap = (struct hashmap*)malloc(sizeof(struct hashmap) + length);
	if (!hashmap)
		return NULL;

	hashmap->capacity = capacity;
	for (size_t i = 0; i < capacity; ++i) {
		hashmap->hashmap_value[i].key = HASHMAP_NULL;
		hashmap->hashmap_value[i].value = 0;
	}
	return hashmap;
}

void hashmap_destory(struct hashmap* hashmap) { free(hashmap); }

struct hashmap_value* hashmap_add(struct hashmap* hashmap, intptr_t key) {
	assert(key > HASHMAP_NULL);

	size_t max_count = hashmap->capacity;
	size_t index = key % max_count;

	for (size_t i = 0; i < hashmap->capacity; i++) {
		struct hashmap_value* hashmap_value = hashmap->hashmap_value + index;
		if (hashmap_value->key == HASHMAP_NULL || hashmap_value->key == HASHMAP_SENTINEL) {
			hashmap_value->key = key;
			return hashmap_value;
		}
		index = (index + 1) % max_count;
	}
	return NULL;
}

struct hashmap_value* hashmap_get(struct hashmap* hashmap, intptr_t key) {
	assert(key > HASHMAP_NULL);

	size_t max_count = hashmap->capacity;
	size_t index = key % max_count;
	for (size_t i = 0; i < hashmap->capacity; i++) {
		struct hashmap_value* hashmap_value = hashmap->hashmap_value + index;
		if (hashmap_value->key == key)
			return hashmap_value;

		if (hashmap_value->key == HASHMAP_NULL)
			break;

		index = (index + 1) % max_count;
	}
	return NULL;
}

int hashmap_remove(struct hashmap* hashmap, intptr_t key) {
	struct hashmap_value* hashmap_value = hashmap_get(hashmap, key);
	if (hashmap_value) {
		hashmap_value->key = HASHMAP_SENTINEL;
	}
	return hashmap_value ? 0 : -1;
}

void hashmap_traverse(struct hashmap* hashmap, hashmap_callback* callback) {
	for (size_t i = 0; i < hashmap->capacity; i++) {
		struct hashmap_value* hashmap_value = hashmap->hashmap_value + i;
		if (hashmap_value->key == HASHMAP_NULL || hashmap_value->key == HASHMAP_SENTINEL)
			continue;
		if (callback(hashmap_value))
			break;
	}
}
