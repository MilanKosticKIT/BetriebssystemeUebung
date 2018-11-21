//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_structs_h
#define myfs_structs_h

#include <sys/stat.h>
#include <sys/types.h>

#include "constants.h"

typedef struct {
    char name[NAME_LENGTH];
    off_t size;
    uid_t userID;
    gid_t groupID;
    time_t last_time;
    time_t modi_time;
    time_t change_time;
    uint16_t  first_block;
    mode_t mode;
} fileStats;

typedef struct {
    uint16_t fileSystemSize;        //In blocks / Number of blocks
    uint16_t emptySpaceSize;        //In blocks / Number of blocks
    uint16_t maximumStorageSize;    //In blocks / Number of blocks
    uint16_t dmapStart;
    uint16_t fatStart;
    uint16_t rootStart;
    uint16_t dataStart;
    uint16_t dmapSize;
    uint16_t fatSize;
    uint16_t rootSize;
    uint16_t dataSize;
} SuperBlock;

#endif /* myfs_structs_h */
