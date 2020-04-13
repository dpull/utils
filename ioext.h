#pragma once
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
    
struct io_buffer
{
    int		identifier;
    size_t	length;
    char	data[];
};
    
#define BUFFER_LENGTH(io_buffer) io_buffer->length
#define BUFFER_DATA(io_buffer) io_buffer->data

struct io_buffer *io_buffer_create(size_t length);
void io_buffer_destory(struct io_buffer *buffer);
    
struct io_buffer *io_readfile(const char *file_path);
bool io_path_exist(const char *file_path);
    
bool io_path_get_filename(const char *file_path, char *buffer, size_t buffer_size);
bool io_path_get_directoryname(const char *file_path, char *buffer, size_t buffer_size);
bool io_path_combine(const char *file_path1, const char *file_path2, char *buffer, size_t buffer_size);
    
#ifdef __cplusplus
}
#endif
