//
//  myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 02.08.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

// For documentation of FUSE methods see https://libfuse.github.io/doxygen/structfuse__operations.html

#undef DEBUG

// TODO: Comment this to reduce debug messages
#define DEBUG
#define DEBUG_METHODS
#define DEBUG_RETURN_VALUES

#include "macros.h"
#include "constants.h"
#include "myfs.h"
#include "myfs-info.h"
#include "myfs-structs.h"
#include <errno.h>

MyFS* MyFS::_instance = NULL;

MyFS* MyFS::Instance() {
    if(_instance == NULL) {
        _instance = new MyFS();
    }
    return _instance;
}

MyFS::MyFS() {
    this->logFile= stderr;

    uint16_t  fatArray[DATA_BLOCKS];
    uint16_t  dmapArray[(DATA_BLOCKS + 1) / 16];
    fileStats rootArray[DATA_BLOCKS];

    fsIO.readDevice(SUPERBLOCK_START, superblock);
    fsIO.readDevice(DMAP_START, dmapArray);
    fsIO.readDevice(FAT_START, fatArray);
    fsIO.readDevice(ROOT_START, rootArray);

    fat.setAll((char*) fatArray);
    dmap.setAll((char*) dmapArray);
    root.setAll(rootArray);
}

MyFS::~MyFS() {

}

int MyFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();

    fileStats fileStats1;
    int res = root.get(path, &fileStats1);
    if (res == -1){
        RETURN(-errno);
    }

    struct stat stats = {};
    stats.st_uid = fileStats1.userID;
    stats.st_gid = fileStats1.groupID;
    stats.st_mode = fileStats1.mode;
    stats.st_size = fileStats1.size;
    stats.st_atime = fileStats1.last_time;
    stats.st_ctime = fileStats1.change_time;
    stats.st_mtime = fileStats1.modi_time;

    *statbuf = stats;

    RETURN(res);
}

int MyFS::fuseReadlink(const char *path, char *link, size_t size) {
    LOGM();
    return 0;
}

int MyFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();

    // TODO: Implement this!

    root.createEntry(path);
    fileStats stats;
    root.get(path, &stats);
    stats.mode = mode;
    uint16_t firstBlock;
    dmap.getFreeBlock(&firstBlock);
    dmap.set(firstBlock);
    stats.first_block = firstBlock;
    fat.addLastToFAT(firstBlock);
    root.update(stats);

    RETURN(0);
}

int MyFS::fuseMkdir(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseUnlink(const char *path) {
    LOGM();

    fileStats file;
    root.get(path, &file);

    std::list<uint16_t >::const_iterator iterator;
    std::list<uint16_t> list;
    fat.iterateFAT(file.first_block, &list);
    for (iterator = list.begin(); iterator != list.end(); ++iterator){
        dmap.clear(*iterator);
    }
    fat.deleteFromFAT(file.first_block);

    superblock.emptySpaceSize += file.size;
    root.deleteEntry(path);

    RETURN(0);
}

int MyFS::fuseRmdir(const char *path) {
    LOGM();
    return 0;
}

int MyFS::fuseSymlink(const char *path, const char *link) {
    LOGM();
    return 0;
}

int MyFS::fuseRename(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseLink(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();
    return 0;
}

int MyFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();
    return 0;
}

int MyFS::fuseUtime(const char *path, struct utimbuf *ubuf) {
    LOGM();
    return 0;
}

int MyFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    fileStats file;
    int exists = root.get(path, &file);

    if ( exists == -1){
        RETURN(-errno);
    }

    if (file.userID == geteuid()) {
        if (fileInfo->flags == O_RDONLY) {
            if (file.mode == S_IRUSR) {
                RETURN(0)
            }
        } else if (fileInfo->flags == O_WRONLY) {
            if (file.mode == S_IWUSR) {
                RETURN(0)
            }
        } else if (fileInfo->flags == O_RDWR) {
            if (file.mode == S_IRWXU) {
                RETURN(0)
            }
        }
    } else if (file.groupID == getegid()) {
        if (fileInfo->flags == O_RDONLY) {
            if (file.mode == S_IRGRP) {
                RETURN(0)
            }
        } else if (fileInfo->flags == O_WRONLY) {
            if (file.mode == S_IWGRP) {
                RETURN(0)
            }
        } else if (fileInfo->flags == O_RDWR) {
            if (file.mode == S_IRWXG) {
                RETURN(0)
            }
        }
    } else {
        if (fileInfo->flags == O_RDONLY) {
            if (file.mode == S_IROTH) {
                RETURN(0)
            }
        } else if (fileInfo->flags == O_WRONLY) {
            if (file.mode == S_IWOTH) {
                RETURN(0)
            }
        } else if (fileInfo->flags == O_RDWR) {
            if (file.mode == S_IRWXO) {
                RETURN(0)
            }
        }
    }
    errno = EACCES;
    RETURN(-errno);
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();


    fileStats file;
    root.get(path, &file);


    if ( file.size < offset){
        RETURN(0);
    }
    if (file.size < offset + size) {
        size = file.size - offset;
    }

    off_t blockNo = offset / BLOCK_SIZE; // block number of file (not block number in filesystem!)
    off_t blockOffset = offset % BLOCK_SIZE; // offset in the block
    int howManyBlocks = ceil(double(size + blockOffset) / double(BLOCK_SIZE)); //number of blocks you need to read for this operation (upper limit)


    std::list<uint16_t> list;
    fat.iterateFAT(file.first_block, &list);

    std::list<uint16_t >::const_iterator iterator = list.begin();
    int blocks[howManyBlocks];    //saves all block locations needed for this operation

    for ( int t = 0; t < blockNo ; t++){
        ++iterator;
    }


    for(int i = 0; i < howManyBlocks; i++){
        blocks[i] = *iterator;
        ++iterator;
    }

    char buffer[BLOCK_SIZE];
    blockDevice.read(blocks[0], buffer);
    memcpy(buf, buffer + blockOffset, BLOCK_SIZE - offset);
    for (int j = 1; j < howManyBlocks - 1; j++) {
        blockDevice.read(blocks[j], buf + blockOffset + BLOCK_SIZE * (j - 1));
    }
    blockDevice.read(blocks[howManyBlocks - 1], buffer);
    memcpy(buf + blockOffset + (howManyBlocks - 2) * BLOCK_SIZE, buffer, (size + blockOffset) % BLOCK_SIZE);




    RETURN(size);
}

int MyFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    // TODO: Implement this!

    RETURN(0);
}

int MyFS::fuseStatfs(const char *path, struct statvfs *statInfo) {
    LOGM();

    return 0;

}

int MyFS::fuseFlush(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    // TODO: Implement this!

    RETURN(0);
}

int MyFS::fuseFsync(const char *path, int datasync, struct fuse_file_info *fi) {
    LOGM();
    return 0;
}

int MyFS::fuseListxattr(const char *path, char *list, size_t size) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseRemovexattr(const char *path, const char *name) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseOpendir(const char *path, struct fuse_file_info *fileInfo) {
  LOGM();

  RETURN(0); // todo always grants access
  
  errno = EACCES;
  RETURN(-errno);
}

int MyFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    fileStats file;
    struct stat s;
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        root.get(i, &file);
        if (file.size >= 0) { // if file exists
            fuseGetattr(file.name, &s);
            filler(buf, file.name, NULL, 0);
        }
    }

    // TODO: Implement this!

    RETURN(0);
}

int MyFS::fuseReleasedir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    // TODO: Implement this!

    RETURN(0);
}

int MyFS::fuseFsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    RETURN(0);
}

int MyFS::fuseCreate(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
    LOGM();

    // TODO: Implement this!

    RETURN(0);
}

void MyFS::fuseDestroy() {
    LOGM();
}

void* MyFS::fuseInit(struct fuse_conn_info *conn) {
    // Open logfile
    this->logFile= fopen(((MyFsInfo *) fuse_get_context()->private_data)->logFile, "w+");
    if(this->logFile == NULL) {
        fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *) fuse_get_context()->private_data)->logFile);
    } else {
        //    this->logFile= ((MyFsInfo *) fuse_get_context()->private_data)->logFile;

        // turn of logfile buffering
        setvbuf(this->logFile, NULL, _IOLBF, 0);

        LOG("Starting logging...\n");
        LOGM();

        // you can get the containfer file name here:
        LOGF("Container file name: %s", ((MyFsInfo *) fuse_get_context()->private_data)->contFile);

        // TODO: Implement your initialization methods here!
    }

    RETURN(0);
}

#ifdef __APPLE__
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t x) {
#else
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
#endif
    LOGM();
    RETURN(0);
}

#ifdef __APPLE__
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size, uint x) {
#else
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size) {
#endif
    LOGM();
    RETURN(0);
}

// TODO: Add your own additional methods here!
