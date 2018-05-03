//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_structs_h
#define myfs_structs_h

#define NAME_LENGTH 255
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64

// TODO: Add structures of your file system here


#define SUPER_START 0
#define DMAP_START 1
#define FAT_START 17
#define ROOT_START 273
#define DATA_START 337

#define SUPER_SIZE 1
#define DMAP_SIZE = FAT_START - DMAP_START;
#define FAT_SIZE = ROOT_START - FAT_START;
#define ROOT_SIZE = DATA_START - ROOT_START;
#define DATA_SIZE 65536


struct MetaData{
    //TODO: Discuss wether we implement this on our one or use stat()
};

#endif /* myfs_structs_h */
