#ifndef PARTITION_H
#define PARTITION_H

#include <cstdio>
#include <mfs.h>

class Partition
{
public:
    Partition(size_t size);
    ~Partition();

    void    createFile(const char* path, const char* data, size_t size);
    void    serialize(FILE* out);

private:
    char*           getRawChunk(uint64_t chunkId);
    char*           getChunk(uint64_t inode);
    MfsSuperChunk*  getSuperChunk();
    MfsFileChunk*   getFileChunk(uint64_t inode);
    MfsFileChunk*   getRootChunk();

    uint64_t    allocChunk();
    void        makeRoot();

    uint64_t    dirLookup(uint64_t parent, const char* filename);
    void        dirWrite(uint64_t parent, const char* filename, uint64_t child);

    uint64_t    makeChild(uint64_t parent, const char* name);
    void        readFile(char* dst, uint64_t inode);
    void        writeFile(uint64_t inode, const char* data, size_t size);

    size_t      _size;
    uint64_t    _root;
    char*       _data;
};

#endif
