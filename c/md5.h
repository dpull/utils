#pragma once

/**
 *  md5 hash function.
 *  @param message: aribtary string.
 *  @param len: message length.
 *  @param output: buffer to receive the hash value. Its size must be
 *  (at least) HASHSIZE.
 */
#define HASHSIZE (16)
void md5(const char* message, long len, char* hash_value);
int md5_tostring(char* hash_value, char* str_output, int str_output_size);
