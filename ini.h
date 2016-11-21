#pragma once
#include <stdbool.h>

struct ini_t* ini_load(const char *data, unsigned data_length);
void ini_free(struct ini_t *ini);
const char* ini_get(struct ini_t *ini, const char *section, const char *key);
