#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include "Partition.h"

class Image
{
public:
    Image(size_t size);
    ~Image();

    void setBootLoader1(const char* path);
    void setBootLoader2(const char* path);
    void addFile(const char* inPath, const char* outPath);
    void serialize(const char* path);

private:
    size_t      _size;
    Partition   _partition;
    size_t      _bl1Size;
    char*       _bl1Data;
    size_t      _bl2Size;
    char*       _bl2Data;
};

#endif
