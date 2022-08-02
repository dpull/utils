#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct circular_buffer;
struct circular_buffer_value {
	intptr_t value;
};

struct circular_buffer* circular_buffer_create(size_t capacity);
void circular_buffer_destory(struct circular_buffer* circular_buffer);

bool circular_buffer_empty(struct circular_buffer* circular_buffer);
bool circular_buffer_full(struct circular_buffer* circular_buffer);
size_t circular_buffer_count(struct circular_buffer* circular_buffer);

struct circular_buffer_value* circular_buffer_pushback(struct circular_buffer* circular_buffer);
struct circular_buffer_value* circular_buffer_pushfront(struct circular_buffer* circular_buffer);

struct circular_buffer_value* circular_buffer_popback(struct circular_buffer* circular_buffer);
struct circular_buffer_value* circular_buffer_popfront(struct circular_buffer* circular_buffer);

typedef bool(circular_buffer_callback)(struct circular_buffer_value* circular_buffer_value);
void circular_buffer_traverse(struct circular_buffer* circular_buffer, circular_buffer_callback* callback);

#ifdef __cplusplus
}
#endif
