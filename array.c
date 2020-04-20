#include "array.h"

int binary_search(const void* key, const void* base, size_t num, size_t size, int (*compar)(const void*, const void*))
{
    int lo = 0;
    int hi = (int)num - 1;

    while (lo <= hi) {
        // i might overflow if lo and hi are both large positive numbers. 
        int i = lo + ((hi - lo) >> 1);

        int c = compar(key, (char*)base + i * size);
        if (c == 0)
            return i;
        if (c > 0)
            lo = i + 1;
        else
            hi = i - 1;
    }
    return ~lo;
}

static bool permutation_impl(int* array, size_t num, size_t start, bool (*callback)(const int*, size_t num))
{
    if (start == num - 1) {
        return callback(array, num);
    }

    int tmp = array[start];
    for (size_t i = start; i < num; i++) {
        array[start] = array[i];
        array[i] = tmp;

        if (!permutation_impl(array, num, start + 1, callback))
            return false;

        array[i] = array[start];
        array[start] = tmp;
    }
}

bool permutation(int* array, size_t num, bool (*callback)(const int*, size_t num))
{
    return permutation_impl(array, num, 0, callback);
}

void print_array(int* array, size_t num, FILE* fd)
{
    fprintf(fd, "[");
    for (size_t i = 0; i < num; i++) {
        const char* fmt = i == 0 ? "%d" : ", %d";
        fprintf(fd, fmt, array[i]);
    }
    fprintf(fd, "]");
}
