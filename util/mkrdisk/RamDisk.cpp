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

#include <cstdio>
#include <cstring>
#include "RamDisk.h"

RamDisk::RamDisk()
: _data(nullptr)
, _dataSize(0)
{

}

RamDisk::~RamDisk()
{
    delete[] _data;
}

void RamDisk::addFile(const char* internalPath, const char* externalPath)
{
    char* newData;
    size_t newDataSize;
    size_t fSize;
    FILE* f;
    RDiskFile entry;

    f = fopen(externalPath, "rb");
    fseek(f, 0, SEEK_END);
    fSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    newDataSize = _dataSize + fSize;
    newData = new char[newDataSize];
    memcpy(newData, _data, _dataSize);
    fread(newData + _dataSize, fSize, 1, f);
    fclose(f);
    delete[] _data;
    _data = newData;
    entry.offset = _dataSize;
    _dataSize = newDataSize;
    entry.size = fSize;
    entry.nameOffset = _strTable.size();
    entry.nameSize = strlen(internalPath);
    _strTable += internalPath;
    _files.push_back(entry);
}

static uint32_t round16(uint32_t n)
{
    return (((n - 1) / 16) + 1) * 16;
}

static uint32_t remain16(uint32_t n)
{
    return round16(n) - n;
}

void RamDisk::dump(const char* path)
{
    FILE* f;
    RDiskHeader header;
    char pad[16];

    for (int i = 0; i < 16; ++i)
        pad[i] = (char)0xff;

    f = fopen(path, "wb");
    header.fileCount = (uint32_t)_files.size();
    header.fileSectionOffset = 16 + round16(_strTable.size());

    fwrite((char*)&header, sizeof(header), 1, f);
    fwrite(pad, 8, 1, f);
    fwrite(_strTable.c_str(), _strTable.size(), 1, f);
    fwrite(pad, remain16(_strTable.size()), 1, f);
    for (auto entry : _files)
    {
        entry.nameOffset += 16;
        entry.offset += (16 + round16(_strTable.size()) + _files.size() * 16);
        fwrite((const char*)&entry, 16, 1, f);
    }
    fwrite(_data, _dataSize, 1, f);
    fclose(f);
}
