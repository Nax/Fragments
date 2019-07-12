#include "stage2.h"

void pmain(uint8_t drive, const FragmentsMbrEntry* mbrEntry)
{
    FragmentsKernelInfo kernelInfo;
    MfsPartition part;
    uint32_t inode;
    uint32_t inodeKernel;
    uint32_t inodeImage;

    println("Bootloader loaded!");
    print("Booting from drive ");
    printhex8(drive);
    print(", MBR entry at ");
    printhex32((uint32_t)mbrEntry);
    putchar('\n');
    putchar('\n');

    kernelInfo.drive = drive;
    memcpy(&kernelInfo.mbrEntry, mbrEntry, sizeof(*mbrEntry));

    detect_memory(&kernelInfo);

    mfs_init(&part, drive, mbrEntry);
    inode = part.inodeRoot;

    inode = mfs_seek_child(&part, inode, "boot");
    print("/boot inode:        ");
    printhex32(inode);
    putchar('\n');

    inodeKernel = mfs_seek_child(&part, inode, "kernel");
    print("/boot/kernel inode: ");
    printhex32(inodeKernel);
    putchar('\n');

    inodeImage = mfs_seek_child(&part, inode, "kimage");
    print("/boot/kimage inode: ");
    printhex32(inodeImage);
    putchar('\n');

    load(&kernelInfo, &part, inodeKernel, inodeImage);

    for (;;) {}
}
