#include "stage2.h"

void pmain(uint8_t drive, const MbrEntry* mbrEntry)
{
    MfsPartition part;
    uint32_t inode;

    println("Bootloader loaded!");
    print("Booting from drive ");
    printhex8(drive);
    print(", MBR entry at ");
    printhex32((uint32_t)mbrEntry);
    putchar('\n');
    putchar('\n');

    detect_memory();

    mfs_init(&part, drive, mbrEntry);
    inode = part.inodeRoot;

    inode = mfs_seek_child(&part, inode, "boot");
    print("/boot inode:        ");
    printhex32(inode);
    putchar('\n');

    inode = mfs_seek_child(&part, inode, "kernel");
    print("/boot/kernel inode: ");
    printhex32(inode);
    putchar('\n');

    for (;;) {}
}
