#include <string.h>

int strncmp(const char* a, const char* b, size_t n)
{
    int va;
    int vb;
    int tmp;

    for (size_t i = 0; i < n; ++i)
    {
        va = a[i];
        vb = b[i];
        tmp = va - vb;
        if (tmp)
            return tmp;
    }
    return 0;
}
