#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifdef _MSC_VER
#include <io.h> 
#define access _access
#define F_OK 00
#endif 

#define IS_DIR_SEPATRATOR(cPos)         (cPos == '\\' || cPos == '/')
#define BUFFER_IDENTIFIER               'ACAI'

static long get_file_size(FILE *file)
{
    long offset = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, offset, SEEK_SET);
    return size;
}

struct io_buffer *io_buffer_create(size_t length)
{
    struct io_buffer* buffer = (struct io_buffer*)malloc(sizeof(*buffer) + length);
    buffer->identifier = BUFFER_IDENTIFIER;
    buffer->length = length;
    return buffer;
}

void io_buffer_destory(struct io_buffer *buffer)
{
    if (buffer) {
        assert(buffer->identifier == BUFFER_IDENTIFIER);
        free(buffer);
    }
}

struct io_buffer *io_readfile(const char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
        return NULL;
    
    long file_size = get_file_size(file);
    
    struct io_buffer *buffer = io_buffer_create(file_size + 1);
    
    size_t nitems = fread(BUFFER_DATA(buffer), file_size, 1, file);
    if (nitems != 1) {
        io_buffer_destory(buffer);
        buffer = NULL;
    }
    fclose(file);
    
    BUFFER_DATA(buffer)[file_size] = '\0';
    BUFFER_LENGTH(buffer) = file_size;
    return buffer;
}

int io_path_exist(const char *path)
{
    return access(path, F_OK) == 0;
}

static const char* _strip_path(const char *file_path)
{
    size_t length = strlen(file_path);
    if (length == 0)
        return NULL;
    
    const char *pos = file_path + length - 1;
    const char *file_name = file_path;
    
    while (pos >= file_path) {
        if (IS_DIR_SEPATRATOR(*pos)) {
            file_name = pos + 1;
            break;
        }
        pos--;
    }
    return file_name;
}

int io_path_get_filename(const char *file_path, char *buffer, size_t buffer_size)
{
    const char *file_name = _strip_path(file_path);
    if (!file_name)
        return false;
    
    strncpy(buffer, file_name, buffer_size);
    buffer[buffer_size - 1] = '\0';
    return true;
}

int io_path_get_directoryname(const char *file_path, char *buffer, size_t buffer_size)
{
    const char *file_name = _strip_path(file_path);
    if (!file_name)
        return false;
    
    size_t length = file_name - file_path;
    if (length >= buffer_size)
        return false;
    
    memcpy(buffer, file_path, length);
    buffer[length] = '\0';
    return true;
}

int io_path_combine(const char *file_path1, const char *file_path2, char *buffer, size_t buffer_size)
{
    size_t length = strlen(file_path1);
    if (length == 0) {
        strncpy(buffer, file_path2, buffer_size);
        buffer[buffer_size - 1] = '\0';
        return true;
    }
    
    const char *separator_char = "";
    if (!IS_DIR_SEPATRATOR(file_path1[length - 1]))
        separator_char = "/";
    
    int ret = snprintf(buffer, buffer_size, "%s%s%s", file_path1, separator_char, file_path2);
    buffer[buffer_size - 1] = '\0';
    
    return ret >= 0 && ret < (int)buffer_size;
}
