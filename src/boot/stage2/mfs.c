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

#include <fs/mfs.h>
#include "stage2.h"

SECTION(".data16") ALIGN(16) static char gMfsDiskBuffer[4096];
static char gMfsIndirectBuffer[4096][3];

void mfs_init(MfsPartition* part, uint8_t drive, const FragmentsMbrEntry* mbrEntry)
{
    char superchunk[4096];

    part->drive = drive;
    part->lbaPartition = mbrEntry->lbaFirst;
    part->lbaSuperchunk = part->lbaPartition + SECTORS_PER_CHUNK * 63;
    part->lbaStart = part->lbaSuperchunk + SECTORS_PER_CHUNK;

    disk_read_raw(superchunk, drive, part->lbaSuperchunk, sizeof(superchunk) / SECTOR_SIZE);
    memcpy(&part->inodeRoot, superchunk, 4);

    part->lbaRoot = part->lbaStart + SECTORS_PER_CHUNK * part->inodeRoot;

    print("Partition LBA:  ");
    printhex32(part->lbaPartition);
    putchar('\n');
    print("Superchunk LBA: ");
    printhex32(part->lbaSuperchunk);
    putchar('\n');
    print("Root LBA:       ");
    printhex32(part->lbaRoot);
    putchar('\n');
    print("Root inode:     ");
    printhex32(part->inodeRoot);
    putchar('\n');
}

void mfs_read(void* dst, const MfsPartition* part, uint32_t inode)
{
    disk_read_raw(gMfsDiskBuffer, part->drive, part->lbaStart + inode * SECTORS_PER_CHUNK, SECTORS_PER_CHUNK);
    memcpy(dst, gMfsDiskBuffer, sizeof(gMfsDiskBuffer));
}

uint32_t mfs_seek_child(const MfsPartition* part, uint32_t parent, const char* name)
{
    char tmp[MFS_CHUNK];
    uint32_t inode;
    char nameStr[256];
    uint16_t nameLen;
    size_t i;

    mfs_read(tmp, part, parent);
    inode = ((MfsFileChunk*)tmp)->data[0];
    mfs_read(tmp, part, inode);

    i = 0;
    for (;;)
    {
        memcpy(&nameLen, tmp + i, 2);
        memcpy(&nameStr, tmp + i + 2, nameLen);
        memcpy(&inode, tmp + i + 2 + nameLen, 4);
        i += (10 + nameLen);
        if (strncmp(nameStr, name, nameLen) == 0)
            return inode;
    }
}

void mfs_read_file(void* dst, const MfsPartition* part, uint32_t fileInode, size_t off, size_t size)
{
    MfsFileChunk fileChunk;

    size_t chunkFirst;
    size_t chunkLast;
    size_t chunkCount;
    size_t chunkIndex;
    size_t startOff;
    size_t readSize;
    uint32_t inode;
    int loadedIndirect;

    if (size == 0)
        return;

    chunkFirst = off / MFS_CHUNK;
    chunkLast = (off + size - 1) / MFS_CHUNK;
    chunkCount = chunkLast - chunkFirst + 1;
    startOff = off % MFS_CHUNK;
    loadedIndirect = 0;

    mfs_read((char*)&fileChunk, part, fileInode);
    for (size_t i = 0; i < chunkCount; ++i)
    {
        chunkIndex = chunkFirst + i;
        readSize = MFS_CHUNK;

        if (startOff)
            readSize -= startOff;
        if (size < readSize)
            readSize = size;

        if (chunkIndex < 16)
        {
            if (readSize == MFS_CHUNK)
                mfs_read(dst, part, fileChunk.data[chunkIndex]);
            else
            {
                mfs_read(gMfsIndirectBuffer[0], part, fileChunk.data[chunkIndex]);
                memcpy(dst, gMfsIndirectBuffer[0] + startOff, readSize);
                startOff = 0;
            }
        }
        else if (chunkIndex < 16 + 512)
        {
            if (!loadedIndirect)
            {
                mfs_read(gMfsIndirectBuffer[1], part, fileChunk.idata);
                loadedIndirect = 1;
            }
            memcpy(&inode, gMfsIndirectBuffer[1] + (chunkIndex - 16) * 8, 4);
            if (readSize == MFS_CHUNK)
                mfs_read(dst, part, inode);
            else
            {
                mfs_read(gMfsIndirectBuffer[0], part, inode);
                memcpy(dst, gMfsIndirectBuffer[0] + startOff, readSize);
                startOff = 0;
            }
        }
        dst = (char*)dst + readSize;
        size -= readSize;
    }
}
