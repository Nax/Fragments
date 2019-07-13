#include <string.h>

int strcmp(const char* a, const char* b)
{
    size_t i;
    int va;
    int vb;
    int tmp;

    i = 0;
    for (;;)
    {
        va = a[i];
        vb = b[i];
        tmp = va - vb;
        if (tmp)
            return tmp;
        if (!va)
            return 0;
        ++i;
    }
}
