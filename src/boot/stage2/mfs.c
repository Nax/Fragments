#include <fs/mfs.h>
#include "stage2.h"

SECTION(".data16") ALIGN(16) static char gMfsDiskBuffer[4096];

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

void mfs_read(char* dst, const MfsPartition* part, uint32_t inode)
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

void mfs_read_file(char* dst, const MfsPartition* part, uint32_t inode)
{
    disk_read_raw(gMfsDiskBuffer, part->drive, part->lbaStart + inode * SECTORS_PER_CHUNK, SECTORS_PER_CHUNK);
    memcpy(dst, gMfsDiskBuffer, sizeof(gMfsDiskBuffer));
}
