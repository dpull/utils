#pragma once
#include <stdbool.h>
    
void* io_readfile(const char *file_path, long *buffer_size);
void io_freebuffer(void *buffer);
