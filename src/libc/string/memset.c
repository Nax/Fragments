#include <string.h>

void* memset(void* dest, int c, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        ((char*)dest)[i] = (char)c;
    return dest;
}
