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






struct MetaData{
    //TODO: Discuss wether we implement this on our one or use stat()
};

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
