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

#include <cstdint>
#include <cstdio>
#include <cstring>
#include "Image.h"

static char* loadFile(size_t* sizeDst, const char* path)
{
    FILE* f;
    char* buf;

    f = fopen(path, "rb");
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
, _partition(size - 40 * 512)
, _bl1Size(0)
, _bl1Data(nullptr)
, _bl2Size(0)
, _bl2Data(nullptr)
{

}

Image::~Image()
{
    delete[] _bl1Data;
    delete[] _bl2Data;
}

void Image::setBootLoader1(const char* path)
{
    _bl1Data = loadFile(&_bl1Size, path);
}

void Image::setBootLoader2(const char* path)
{
    _bl2Data = loadFile(&_bl2Size, path);
}

void Image::addFile(const char* inPath, const char* outPath)
{
    char* data;
    size_t size;

    data = loadFile(&size, inPath);
    _partition.createFile(outPath, data, size);
    delete[] data;
}

void Image::serialize(const char* path)
{
    uint8_t tmp8;
    uint16_t tmp16;
    uint32_t tmp32;

    static const size_t bufferSize = 40 * 512;
    char* zero;
    FILE* f;

    zero = new char[bufferSize];
    memset(zero, 0, bufferSize);
    f = fopen(path, "wb");
    fwrite(zero, bufferSize, 1, f);
    delete[] zero;

    // Write first bootloader
    fseek(f, 0, SEEK_SET);
    fwrite(_bl1Data, _bl1Size, 1, f);

    // Serialize the partition
    fseek(f, bufferSize, SEEK_SET);
    _partition.serialize(f);

    // Write second bootloader
    fseek(f, bufferSize, SEEK_SET);
    fwrite(_bl2Data, _bl2Size, 1, f);

    // Fixup MBR partition table
    fseek(f, 0x1be, SEEK_SET);
    tmp8 = 0x80; fwrite(&tmp8, 1, 1, f);                    // Boot flag
    tmp8 = 0xff; fwrite(&tmp8, 1, 1, f);                    // CHS Dummy Head
    tmp16 = 0xffff; fwrite(&tmp16, 2, 1, f);                // CHS Dummy Sector + Cylinder
    tmp8 = 0x34; fwrite(&tmp8, 1, 1, f);                    // System ID
    tmp8 = 0xff; fwrite(&tmp8, 1, 1, f);                    // CHS Dummy Head
    tmp16 = 0xffff; fwrite(&tmp16, 2, 1, f);                // CHS Dummy Sector + Cylinder
    tmp32 = 40; fwrite(&tmp32, 4, 1, f);                    // Start LBA
    tmp32 = (_size / 512) - 40; fwrite(&tmp32, 4, 1, f);    // Partition Size
    fclose(f);

}
