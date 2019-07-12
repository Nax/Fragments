#include "RamDisk.h"
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
    RamDisk rdisk;

    for (int i = 2; i < argc; i += 2)
    {
        rdisk.addFile(argv[i], argv[i + 1]);
    }

    rdisk.dump(argv[1]);
    return 0;
}
