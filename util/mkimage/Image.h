#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>

class Image
{
public:
    Image(size_t size);
    ~Image();

    void setBootLoader1(const char* path);
    void serialize(const char* path);

private:
    size_t      _size;
    size_t      _bl1Size;
    char*       _bl1Data;
};

#endif
