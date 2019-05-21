#include <cstdio>
#include <cstring>
#include "Image.h"

static char* loadFile(size_t* sizeDst, const char* path)
{
    FILE* f;
    char* buf;

    f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    *sizeDst = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = new char[*sizeDst];
    fread(buf, *sizeDst, 1, f);
    fclose(f);

    return buf;
}

Image::Image(size_t size)
: _size(size)
, _bl1Size(0)
, _bl1Data(nullptr)
{

}

Image::~Image()
{
    delete[] _bl1Data;
}

void Image::setBootLoader1(const char* path)
{
    _bl1Data = loadFile(&_bl1Size, path);
}

void Image::serialize(const char* path)
{
    static const size_t bufferSize = 63 * 16 * 512;
    char* zero;
    FILE* f;

    zero = new char[bufferSize];
    memset(zero, 0, bufferSize);
    f = fopen(path, "w");
    for (size_t i = 0; i < _size / bufferSize; ++i)
        fwrite(zero, bufferSize, 1, f);
    delete[] zero;

    fseek(f, SEEK_SET, 0);
    fwrite(_bl1Data, _bl1Size, 1, f);
    fclose(f);
}
