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

#include <cstring>
#include <algorithm>
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

void Partition::createFile(const char* path, const char* data, size_t size)
{
    int last;
    uint64_t inode;
    char tmp[512];
    char* pathElement;
    size_t i;

    inode = _root;
    strcpy(tmp, path);
    pathElement = tmp;

    i = 0;
    for (;;)
    {
        if (pathElement[i] == '/' || pathElement[i] == 0)
        {
            last = (pathElement[i] == 0);
            pathElement[i] = 0;
            inode = makeChild(inode, pathElement);
            pathElement = pathElement + i + 1;
            i = 0;
            if (last)
                break;
        }
        else
            i++;
    }

    writeFile(inode, data, size);
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
    _root = allocChunk();
    getSuperChunk()->root = _root;
}

uint64_t Partition::makeChild(uint64_t parent, const char* name)
{
    size_t i;
    size_t dataLen;
    uint64_t child;
    uint16_t len;
    uint16_t len2;
    MfsFileChunk* dir;
    char* data;

    dir = getFileChunk(parent);
    if (dir->data[0] == 0)
        dir->data[0] = allocChunk();
    data = getChunk(dir->data[0]);
    dataLen = dir->size;
    len = strlen(name);
    i = 0;
    for (;;)
    {
        if (i >= dataLen)
            break;
        memcpy(&len2, data + i, 2);
        i += 2;
        if (len == len2 && strncmp(data + i, name, len) == 0)
        {
            i += len;
            memcpy(&child, data + i, 8);
            return child;
        }
        i += len2;
        i += 8;
    }
    child = allocChunk();
    memcpy(data + i, &len, 2);
    memcpy(data + i + 2, name, len);
    memcpy(data + i + 2 + len, &child, 8);
    dir->size += (len + 10);
    return child;
}

void Partition::writeFile(uint64_t inode, const char* data, size_t size)
{
    size_t remain;
    size_t len;
    MfsFileChunk* file;
    uint64_t tmp;
    uint64_t* idata;

    remain = size;
    file = getFileChunk(inode);
    for (int i = 0; i < 16; ++i)
    {
        if (!remain)
            break;
        len = std::min(remain, (size_t)MFS_CHUNK);
        tmp = allocChunk();
        memcpy(getChunk(tmp), data, len);
        remain -= len;
        data += len;
        file->data[i] = tmp;
    }
    if (remain)
    {
        file->idata = allocChunk();
        idata = (uint64_t*)getChunk(file->idata);
        for (int i = 0; i < MFS_CHUNK / 8; ++i)
        {
            if (!remain)
                break;
            len = std::min(remain, (size_t)MFS_CHUNK);
            tmp = allocChunk();
            memcpy(getChunk(tmp), data, len);
            remain -= len;
            data += len;
            idata[i] = tmp;
        }
    }
    file->size = size;
}
