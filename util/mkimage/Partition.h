/*
 * Copyright (c) 2019, Maxime Bacoux
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
