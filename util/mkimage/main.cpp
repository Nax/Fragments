#include "Image.h"

int main(int argc, char** argv)
{
    Image* image;

    image = new Image(63 * 16 * 512 * 100);
    image->setBootLoader1(argv[2]);
    image->setBootLoader2(argv[3]);
    image->addFile(argv[4], "boot/kernel");
    image->serialize(argv[1]);
    delete image;
    return 0;
}
