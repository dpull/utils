#pragma once
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
    
struct io_buffer
{
    void *data;
    size_t length;
};

struct io_buffer *io_buffer_create(size_t length);
void io_buffer_destory(struct io_buffer *buffer);
    
struct io_buffer *io_readfile(const char *file_path);
int io_path_exist(const char *file_path);
    
#ifdef __cplusplus
}
#endif
