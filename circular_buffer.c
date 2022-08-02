#include "circular_buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct circular_buffer {
	int capacity;
	int size;
	struct circular_buffer_value* first;
	struct circular_buffer_value* last;
	struct circular_buffer_value circular_buffer_value[0];
};

struct circular_buffer* circular_buffer_create(size_t capacity) {
	size_t length = sizeof(struct circular_buffer_value) * capacity;
	if (length == 0)
		return NULL;

	struct circular_buffer* circular_buffer = (struct circular_buffer*)malloc(sizeof(struct circular_buffer) + length);
	if (!circular_buffer)
		return NULL;

	circular_buffer->capacity = (int)capacity;
	circular_buffer->size = 0;
	circular_buffer->first = circular_buffer->circular_buffer_value;
	circular_buffer->last = circular_buffer->circular_buffer_value;
	return circular_buffer;
}

void circular_buffer_destory(struct circular_buffer* circular_buffer) { free(circular_buffer); }

bool circular_buffer_empty(struct circular_buffer* circular_buffer) { return circular_buffer->size == 0; }

bool circular_buffer_full(struct circular_buffer* circular_buffer) { return circular_buffer->size == circular_buffer->capacity; }

size_t circular_buffer_count(struct circular_buffer* circular_buffer) { return circular_buffer->size; }

static struct circular_buffer_value* increment(struct circular_buffer* circular_buffer, struct circular_buffer_value* value) {
	++value;
	if (value == (circular_buffer->circular_buffer_value + circular_buffer->capacity))
		value = circular_buffer->circular_buffer_value;
	return value;
}

static struct circular_buffer_value* decrement(struct circular_buffer* circular_buffer, struct circular_buffer_value* value) {
	if (value == circular_buffer->circular_buffer_value)
		value = circular_buffer->circular_buffer_value + circular_buffer->capacity;
	value--;
	return value;
}

struct circular_buffer_value* circular_buffer_pushback(struct circular_buffer* circular_buffer) {
	if (circular_buffer_full(circular_buffer))
		return NULL;
	struct circular_buffer_value* value = circular_buffer->last;
	circular_buffer->last = increment(circular_buffer, circular_buffer->last);
	circular_buffer->size++;
	return value;
}

struct circular_buffer_value* circular_buffer_pushfront(struct circular_buffer* circular_buffer) {
	if (circular_buffer_full(circular_buffer))
		return NULL;
	struct circular_buffer_value* value = circular_buffer->first;
	circular_buffer->first = decrement(circular_buffer, circular_buffer->first);
	circular_buffer->size++;
	return value;
}

struct circular_buffer_value* circular_buffer_popback(struct circular_buffer* circular_buffer) {
	if (circular_buffer_empty(circular_buffer))
		return NULL;
	struct circular_buffer_value* value = circular_buffer->last;
	circular_buffer->last = decrement(circular_buffer, circular_buffer->last);
	circular_buffer->size--;
	return value;
}

struct circular_buffer_value* circular_buffer_popfront(struct circular_buffer* circular_buffer) {
	if (circular_buffer_empty(circular_buffer))
		return NULL;
	struct circular_buffer_value* value = circular_buffer->first;
	circular_buffer->first = increment(circular_buffer, circular_buffer->first);
	circular_buffer->size--;
	return value;
}

void circular_buffer_traverse(struct circular_buffer* circular_buffer, circular_buffer_callback* callback) {
	struct circular_buffer_value* value = circular_buffer->first;
	for (int i = 0; i < circular_buffer->size; ++i) {
		if (callback(value))
			break;
		value = increment(circular_buffer, value);
	}
}
