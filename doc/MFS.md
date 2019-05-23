# MFS

## Miaou File System

The filesystem work in chunks, or 4k blocks.

Chunks 0-62 are reserved. Chunk 63 is the superchunk.

Superchunk:

| offset  | size  | name | comment        |
|---------|-------|------|----------------|
|       0 |     8 | root | The root inode |

Bitmap chunks:

Every chunk that is a multiple of 64 is actually a bitmap of the following chunks.
Because a chunk has 4k bytes and a byte is 8 bits, each such bitmap can address around 134 MiB
of data.

Data chunk:

Files have the following structure:

| offset  |  size  |  name    | comment                |
|---------|--------|----------|------------------------|
|  0x0000 |      2 |   uid    | The user ID            |
|  0x0002 |      2 |   gid    | The group ID           |
|  0x0004 |      4 |  mode    | The file mode          |
|  0x0008 |      8 | btime    | The file birth time    |
|  0x0010 |      8 | atime    | The file access time   |
|  0x0018 |      8 | mtime    | The file modified time |
|  0x0020 |      8 | ctime    | The file change time   |
|  0x0028 |      8 | size     | The size in bytes      |
|  0x0030 | 8 * 16 | data[16] | Data pointers          |
|  0x00a0 |      8 | idata    | Indirect data ptr      |
|  0x00a8 |      8 | idata2   | Indirect data ptr2     |
|  0x00b0 |      8 | idata3   | Indirect data ptr3     |
|  0x00b8 |      8 | idata4   | Indirect data ptr4     |

Directories have a simple structure:

<u16 nameLen> <name> <u64> node
