#include "stage2.h"

void* memcpy(void* dest, const void* src, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        ((char*)dest)[i] = ((char*)src)[i];

    return dest;
}

void* memmove(void* dest, const void* src, size_t n)
{
    if (dest < src || (void*)((char*)src + n) <= dest)
    {
        for (size_t i = 0; i < n; ++i)
            ((char*)dest)[i] = ((char*)src)[i];
    }
    else
    {
        /* Copy backward */
        while (n)
        {
            n--;
            ((char*)dest)[n] = ((char*)src)[n];
        }
    }

    return dest;
}

void* memset(void* dest, int value, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        ((char*)dest)[i] = (char)value;

    return dest;
}