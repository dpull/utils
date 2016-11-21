#include "io.h"
#include <stdio.h>
#include <stdlib.h>

static long get_file_size(FILE *file) {
    long offset = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, offset, SEEK_SET);
    return size;
}

void* io_readfile(const char *file_path, long *buffer_size) {
    FILE * file = fopen(file_path, "rb");
    if (!file)
        return NULL;
    
    long file_size = get_file_size(file);
    void *buffer = malloc(file_size);
    size_t nitems = fread(buffer, file_size, 1, file);
    if (nitems != 1) {
        free(buffer);
        buffer = NULL;
        file_size = 0;
    }
    
    fclose(file);
    *buffer_size = file_size;
    return buffer;
}

void io_freebuffer(void *buffer) {
    free(buffer);
}
