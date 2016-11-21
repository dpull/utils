#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct ini_t* ini_load(const char *data, unsigned data_length);
void ini_free(struct ini_t *ini);
const char* ini_get(struct ini_t *ini, const char *section, const char *key);

#ifdef __cplusplus
}
#endif
