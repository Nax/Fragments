#include <string.h>
#include <rdisk.h>
#include "kernel.h"

void kimage_init(void)
{
    gKernel.kimageSize = gKernel.bootInfo.kimageSize;
    gKernel.kimage = kmalloc(0, gKernel.kimageSize);
    printhex64((uint64_t)gKernel.kimageSize); putchar('\n');
    printhex64((uint64_t)gKernel.kimage); putchar('\n');
    memcpy(gKernel.kimage, (void*)gKernel.bootInfo.kimage, gKernel.kimageSize);
}

const char* kimage_open(const char* path)
{
    size_t pathLen;
    RDiskHeader header;
    RDiskFile file;
    const char* kimageBase;

    kimageBase = gKernel.kimage;
    memcpy(&header, kimageBase, sizeof(header));

    pathLen = strlen(path);
    for (uint32_t i = 0; i < header.fileCount; ++i)
    {
        memcpy(&file, kimageBase + header.fileSectionOffset + i * sizeof(file), sizeof(file));
        if (file.nameSize == pathLen && strncmp(path, kimageBase + file.nameOffset, pathLen) == 0)
        {
            return kimageBase + file.offset;
        }
    }
    return NULL;
}
