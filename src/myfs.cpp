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

#include <errno.h>
#include <string>

#include "macros.h"
#include "constants.h"
#include "myfs.h"
#include "myfs-info.h"
#include "myfs-structs.h"

MyFS* MyFS::_instance = NULL;

MyFS* MyFS::Instance() {
    if(_instance == NULL) {
        _instance = new MyFS();
    }
    return _instance;
}

MyFS::MyFS() {
    this->logFile= stderr;
}

MyFS::~MyFS() {

}

int MyFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();

    const char* name = path;
    if (*path == '/') {
        if (strlen(path) == 1) {
            name = ".";
        } else {
            name++;
        }
    }

    fileStats fileStats1;
    int res = root.get(name, &fileStats1);
    if (res == -1){
        LOG(path);
        LOG(name);
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
    stats.st_nlink = fileStats1.nlink;

    *statbuf = stats;

    LOG(path);
    LOG(name);
    RETURN(res);
}

int MyFS::fuseReadlink(const char *path, char *link, size_t size) {
    LOGM();
    return 0;
}

int MyFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();

    // TODO: Implement this!

    const char* name = path + 1;

    root.createEntry(name, mode);
    fileStats stats;
    root.get(path, &stats);
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

    const char* name = path + 1;

    fileStats file;
    root.get(name, &file);

    std::list<uint16_t >::const_iterator iterator;
    std::list<uint16_t> list;
    fat.iterateFAT(file.first_block, &list);
    for (iterator = list.begin(); iterator != list.end(); ++iterator){
        dmap.clear(*iterator);
    }
    fat.deleteFromFAT(file.first_block);

    superblock.emptySpaceSize += file.size;
    root.deleteEntry(name);

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

    const char* name = path + 1;

    fileStats file;
    if (root.get(name, &file) == -1){
        RETURN(-errno);
    }

    LOG("mode");
    LOGI(file.mode);
    if (file.userID == geteuid()) {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("User RDWR");
            if ((file.mode & S_IRWXU) != 0) {
                RETURN(0);
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            LOG("User WRONLY");
            if ((file.mode & S_IWUSR) != 0) {
                RETURN(0);
            }
        } else {
            LOG("User RDONLY");
            if ((file.mode & S_IRUSR) != 0) {                      // hier
                RETURN(0);
            }
        }
    } else if (file.groupID == getegid()) {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("Group RDWR");
            if ((file.mode & S_IRWXG) != 0) {
                RETURN(0);
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            LOG("Group WRONLY");
            if ((file.mode & S_IWGRP) != 0) {
                RETURN(0);
            }
        } else {
            LOG("Group RDONLY");
            if ((file.mode & S_IRGRP) != 0) {                      // hier
                RETURN(0);
            }
        }
    } else {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("Other RDWR");
            if ((file.mode & S_IRWXO) != 0) {
                RETURN(0);
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            LOG("Other WRONLY");
            if ((file.mode & S_IWOTH) != 0) {
                RETURN(0);
            }
        } else {
            LOG("Other RDONLY");
            if ((file.mode & S_IROTH) != 0) {                      // hier
                RETURN(0);
            }
        }
    }

    LOG("fileInfo flags:");
    LOGI(fileInfo->flags);
    LOG("Read-only");
    LOGI(O_RDONLY);
    LOG("Write-only");
    LOGI(O_WRONLY);
    LOG("Read-Write");
    LOGI(O_RDWR);


    errno = EACCES;
    RETURN(-errno);
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    char testBlock[BLOCK_SIZE + 1];
    for (int i = 0; i < BLOCK_SIZE; i++) {
        testBlock[i] = 'A';
    }
    for(int i = DATA_START; i < DATA_START + 10; i++) {
        if (blockDevice->read(i, testBlock) < 0) {
            LOG("ERROR!!!!!!!!!!!!!!!!!!!!!!!!");
            LOGI(errno);
        }
        testBlock[BLOCK_SIZE] = '\0';
        LOG(testBlock);
    }















    const char* name = path + 1;

    fileStats file;
    if (root.get(name, &file) == -1){
        RETURN(-errno);
    }
    if (file.size <= offset){
        RETURN(0); //EOF
    }
    if ((uint64_t)file.size < offset + size) {
        size = file.size - offset;
    }

    off_t blockNo = offset / BLOCK_SIZE; // block number of file (not block number in filesystem!)
    off_t blockOffset = offset % BLOCK_SIZE; // offset in the block

    //number of blocks you need to read for this operation (upper limit)
    int howManyBlocks = (size + blockOffset) / BLOCK_SIZE;
    if ((size + blockOffset) % BLOCK_SIZE != 0) howManyBlocks++;


    std::list<uint16_t> list;
    fat.iterateFAT(file.first_block, &list);

    std::list<uint16_t >::const_iterator iterator = list.begin();
    uint16_t blocks[howManyBlocks]; //saves all block locations needed for this operation

    for ( int t = 0; t < blockNo ; t++){
        ++iterator;
    }

    for(int i = 0; i < howManyBlocks; i++){
        blocks[i] = *iterator;
        ++iterator;
    }

    char buffer[BLOCK_SIZE];
    blockDevice->read(DATA_START + blocks[0], buffer);
    memcpy(buf, buffer + blockOffset, BLOCK_SIZE - blockOffset);
    for (int j = 1; j < howManyBlocks - 1; j++) {
        blockDevice->read(DATA_START + blocks[j], buf - blockOffset + BLOCK_SIZE * j);
    }
    blockDevice->read(DATA_START + blocks[howManyBlocks - 1], buffer);
    memcpy(buf - blockOffset + (howManyBlocks - 1) * BLOCK_SIZE, buffer, (size + blockOffset) % BLOCK_SIZE);

    RETURN((int)size);
}

int MyFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    // TODO: Implement this!

    //const char* name = path + 1;

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

    //const char* name = path + 1;

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

    // TODO: Implement this!

    if (strcmp("/", path) == 0) {
        for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
            if (root.exists(i)) {
                struct stat s = {};
                char* name;
                root.getName(i, &name);
                fuseGetattr(name, &s);
                filler(buf, name, &s, 0);
            }
        }
        filler(buf, "..", NULL, 0);

        RETURN(0);
    } else {
        errno = ENOTDIR;
        RETURN(-errno);
    }
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

    //const char* name = path + 1;

    RETURN(0);
}

void MyFS::fuseDestroy() {
    LOGM();

    blockDevice->close();
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

        if (blockDevice->open(((MyFsInfo *) fuse_get_context()->private_data)->contFile) == 0) {
            uint16_t* fatArray = new uint16_t[DATA_BLOCKS];
            uint8_t* dMapArray = new uint8_t[(DATA_BLOCKS + 1) / 8];
            fileStats* rootArray = new fileStats[ROOT_ARRAY_SIZE];


            int ret = 0;
            ret = fsIO.readDevice(SUPERBLOCK_START, &superblock, sizeof(superblock));
            if (ret < 0) {
                LOG("Error at blockdevice.read() (Reading superblock)");
                LOGI(ret);
                LOG("Errno:");
                LOGI(errno);
            }
            ret = fsIO.readDevice(DMAP_START, dMapArray, sizeof(*dMapArray) * (DATA_BLOCKS + 1) / 8);
            if (ret < 0) {
                LOG("Error at blockdevice.read() (Reading dmap)");
                LOGI(ret);
                LOG("Errno:");
                LOGI(errno);
            }
            ret = fsIO.readDevice(FAT_START, fatArray, sizeof(*fatArray) * DATA_BLOCKS);
            if (ret < 0) {
                LOG("Error at blockdevice.read() (Reading fat)");
                LOGI(ret);
                LOG("Errno:");
                LOGI(errno);
            }
            ret = fsIO.readDevice(ROOT_START, rootArray, sizeof(*rootArray) * ROOT_ARRAY_SIZE);
            if (ret < 0) {
                LOG("Error at blockdevice.read() (Reading root)");
                LOGI(ret);
                LOG("Errno:");
                LOGI(errno);
            }

            dmap.setAll(dMapArray);
            fat.setAll(fatArray);
            root.setAll(rootArray);

            LOG("----------dmap Array (first entries)----------");
            for(int i = 0; i < 10; i++) {
                LOGI((int) dMapArray[i]);
            }

            LOG("----------fat Array (first entries)-----------");
            for(int i = 0; i < 10; i++) {
                LOGI((int) fatArray[i]);
            }

            LOG("-------Root Array (first entries (size))-------");
            for(int i = 0; i < 10; i++) {
                fileStats stats;
                root.get(i, &stats);
                LOGI((int) stats.size);
            }
            LOG("-----------------------------------------------");

            delete[] dMapArray;
            delete[] fatArray;
            delete[] rootArray;
        } else {
            LOG("Error at blockdevice.open()");
        }
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
