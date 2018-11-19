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

struct SuperBlock {
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
};

#endif /* myfs_structs_h */
