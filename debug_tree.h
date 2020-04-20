#pragma once

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

struct debug_tree* debug_tree_create(struct debug_tree* parent, const char* format, ...);
void debug_tree_destroy(struct debug_tree* tree);
void debug_tree_print(struct debug_tree* tree, FILE* stream);

#ifdef __cplusplus
}
#endif
