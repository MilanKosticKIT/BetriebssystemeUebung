//
//  blockdevice.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 09.10.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

// DO NOT EDIT THIS FILE!!!

#include <cstdlib>
#include <cassert>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "macros.h"

#include <iostream>

#include "blockdevice.h"

//#undef DEBUG

BlockDevice::BlockDevice(u_int32_t blockSize) {
    assert(blockSize % 512 == 0);
    this->blockSize= blockSize;
}

void BlockDevice::resize(u_int32_t blockSize) {
    assert(blockSize % 512 == 0);
    this->blockSize= blockSize;
}

int BlockDevice::create(const char *path) {
    
    // Open Container file
    contFile = ::open(path, O_EXCL | O_RDWR | O_CREAT, 0666);
    if (contFile < 0) {
        if (errno == EEXIST)
            error("container file already exists");
        else
            error("unable to create container file");
    }
    
    this->size= 0;
    
    return 0;
}

int BlockDevice::open(const char *path) {
    
    // Open Container file
    contFile = ::open(path, O_EXCL | O_RDWR);
    if (contFile < 0) {
        if (errno == ENOENT)
            error("container file does not exists");
        else
            error("unknown error");
    }
    
    // read file stats
    struct stat st;
    if( fstat(contFile, &st) < 0 ) {
        error("fstat returned -1");
    }
    
    // get file size
    if(st.st_size > INT32_MAX) {
        error("file to large");
    }
    this->size= (uint32_t) st.st_size;
    
    return 0;
}


int BlockDevice::close() {
    
    ::close(this->contFile);
    
    return 0;
}

// this method returns 0 if successful, -1 otherwise
int BlockDevice::read(u_int32_t blockNo, char *buffer) {
#ifdef DEBUG
    fprintf(stderr, "BlockDevice: Reading block %d\n", blockNo);
#endif
    off_t pos = (blockNo) * this->blockSize;
    if (lseek (this->contFile, pos, SEEK_SET) != pos)
        return -1;
    
    int size = (this->blockSize);
    if (::read (this->contFile, buffer, size) != size)
        return -1;
    return 0;
}

int BlockDevice::write(u_int32_t blockNo, const char *buffer) {
#ifdef DEBUG
    fprintf(stderr, "BlockDevice: Writing block %d\n", blockNo);
#endif
    off_t pos = (blockNo) * this->blockSize;
    if (lseek (this->contFile, pos, SEEK_SET) != pos)
        return -1;

    int __size = (this->blockSize);
    if (::write (this->contFile, buffer, __size) != __size)
        return -1;

    return 0;
}

uint32_t BlockDevice::getSize() {
    
    // update size from file stats
    struct stat st;
    if( fstat(contFile, &st) < 0 ) {
        error("fstat returned -1");
    }
    
    if(st.st_size > INT32_MAX)
        error("file to large");
    this->size= (uint32_t) st.st_size;
    
    return this->size;
}


