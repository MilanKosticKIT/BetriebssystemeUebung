
//alles in Byte

#ifndef constants
#define constants

/* 4096 byte blocks
#define DATA_BYTES 33554432
#define DATA_BLOCKS 8192
#define BLOCK_SIZE  4096
*/

// 512 byte blocks

#define NAME_LENGTH 255
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64

#define DATA_BYTES (33554432 - 512)
#define DATA_BLOCKS 65535
#define BLOCK_SIZE  512
#define FAT_TERMINATOR DATA_BLOCKS

#define SUPERBLOCK_SIZE 1
#define DMAP_SIZE 16
#define FAT_SIZE 256
#define ROOT_SIZE 64 // todo #32

#define SUPERBLOCK_START 0
#define DMAP_START (SUPERBLOCK_START + SUPERBLOCK_SIZE)
#define FAT_START (DMAP_START + DMAP_SIZE)
#define ROOT_START (FAT_START + FAT_SIZE)
#define DATA_START (ROOT_START + ROOT_SIZE)

#endif
