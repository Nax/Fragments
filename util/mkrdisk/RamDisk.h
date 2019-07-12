#ifndef RAM_DISK_H
#define RAM_DISK_H

#include <vector>
#include <string>
#include <cstdint>

typedef struct {
    uint32_t    fileCount;
    uint32_t    fileSectionOffset;
} RDiskHeader;

typedef struct {
    uint32_t    nameOffset;
    uint32_t    nameSize;
    uint32_t    offset;
    uint32_t    size;
} RDiskFile;

class RamDisk
{
public:
    RamDisk();
    ~RamDisk();

    void addFile(const char* internalPath, const char* externalPath);
    void dump(const char* path);

private:
    std::string             _strTable;
    std::vector<RDiskFile>  _files;
    char*                   _data;
    size_t                  _dataSize;
};

#endif
