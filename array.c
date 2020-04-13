#include "array.h"

int binary_search(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *))
{
    int lo = 0;
    int hi = (int)num - 1;

    while (lo <= hi) {
        // i might overflow if lo and hi are both large positive numbers. 
        int i = lo + ((hi - lo) >> 1);

        int c = compar(key, (char*)base + i * size);
        if (c == 0) 
            return i;
        if (c < 0)
            lo = i + 1;
        else 
            hi = i - 1;
    }
    return ~lo;
}
