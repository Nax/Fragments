#ifndef PARTITION_H
#define PARTITION_H

#include <cstdio>
#include <mfs.h>

class Partition
{
public:
    Partition(size_t size);
    ~Partition();

    void serialize(FILE* out);

private:
    char*           getRawChunk(uint64_t chunkId);
    char*           getChunk(uint64_t inode);
    MfsSuperChunk*  getSuperChunk();
    MfsFileChunk*   getFileChunk(uint64_t inode);
    MfsFileChunk*   getRootChunk();

    uint64_t    allocChunk();
    void        makeRoot();

    size_t  _size;
    char*   _data;
};

#endif
