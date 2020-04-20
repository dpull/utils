/**
 * Copyright (c) 2016 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `ini.c` for details.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ini_t* ini_load(const char* data, unsigned data_length);
void ini_free(struct ini_t* ini);
const char* ini_get(struct ini_t* ini, const char* section, const char* key);

#ifdef __cplusplus
}
#endif
