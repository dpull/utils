#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void* io_readfile(const char *file_path, long *buffer_size);
void io_freebuffer(void *buffer);

#ifdef __cplusplus
}
#endif