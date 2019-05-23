#include <cstring>
#include "Partition.h"

Partition::Partition(size_t size)
: _size(size)
{
    _data = new char[size];
    memset(_data, 0, size);
    makeRoot();
}

Partition::~Partition()
{
    delete[] _data;
}

void Partition::serialize(FILE* out)
{
    fwrite(_data, _size, 1, out);
}

char* Partition::getRawChunk(uint64_t chunkId)
{
    return _data + chunkId * MFS_CHUNK;
}

char* Partition::getChunk(uint64_t inode)
{
    return getRawChunk(inode + 64);
}

MfsSuperChunk* Partition::getSuperChunk()
{
    return (MfsSuperChunk*)getRawChunk(63);
}

MfsFileChunk* Partition::getFileChunk(uint64_t inode)
{
    return (MfsFileChunk*)getChunk(inode);
}

MfsFileChunk* Partition::getRootChunk()
{
    return getFileChunk(getSuperChunk()->root);
}

uint64_t Partition::allocChunk()
{
    static const int kChunksPerSection = MFS_CHUNK * 8;
    int mask;
    int section;
    int chunk;
    char* bitmap;
    uint8_t value;

    section = 0;
    for (;;)
    {
        bitmap = getRawChunk(section * kChunksPerSection);
        for (chunk = 1; chunk < kChunksPerSection; ++chunk)
        {
            value = (uint8_t)bitmap[chunk / 8];
            mask = 1 << (chunk % 8);
            if (!(value & mask))
            {
                /* We found a free chunk */
                value |= mask;
                bitmap[chunk / 8] = value;
                return section * kChunksPerSection + chunk;
            }
        }
        section++;
    }
}

void Partition::makeRoot()
{
    uint64_t root;

    root = allocChunk();
    getSuperChunk()->root = root;
}
