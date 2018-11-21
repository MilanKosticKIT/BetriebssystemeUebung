//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_structs_h
#define myfs_structs_h

// TODO: Add structures of your file system here

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

struct SuperBlock {
    uint32_t fileSystemSize;        //In blocks / Number of blocks
    uint32_t emptySpaceSize;        //In blocks / Number of blocks
    uint32_t maximumStorageSize;    //In blocks / Number of blocks
    uint32_t dmapStart;
    uint32_t fatStart;
    uint32_t rootStart;
    uint32_t dataStart;
    uint32_t dmapSize;
    uint32_t fatSize;
    uint32_t rootSize;
    uint32_t dataSize;

};

#endif /* myfs_structs_h */
