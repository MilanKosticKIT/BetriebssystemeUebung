
//alles in Byte

#ifndef constants
#define constants


#define DATA_BYTES 33554432
#define DATA_BLOCKS 8192
#define BLOCK_SIZE  512
#define NAME_LENGTH 255
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64

#define SUPERBLOCK_START 0
#define DMAP_START 1
#define FAT_START 2
#define ROOT_START 34
#define DATA_START 36

#define DMAP_SIZE = FAT_START - DMAP_START;
#define FAT_SIZE = ROOT_START - FAT_START;
#define ROOT_SIZE = DATA_START - ROOT_START;

#endif
