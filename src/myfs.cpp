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
//#define DEBUG_METHODS
//#define DEBUG_RETURN_VALUES

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
    this->logFile = stderr;
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

    RETURN(0);
}

int MyFS::fuseReadlink(const char *path, char *link, size_t size) {
    LOGM();
    return 0;
}

int MyFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();

    const char* name = path;
    if (*path == '/') {
        if (strlen(path) == 1) {
            name = ".";
        } else {
            name++;
        }
    }
    int ret;

    ret = root.createEntry(name, mode);
    if (ret < 0) {
        RETURN(-errno);
    }
    fileStats stats;
    ret = root.get(name, &stats);
    if (ret < 0) {
        RETURN(-errno);
    }
    uint16_t firstBlock;
    ret = dmap.getFreeBlock(&firstBlock);
    if (ret < 0) {
        RETURN(-errno);
    }
    dmap.set(firstBlock);
    stats.first_block = firstBlock;
    fat.addLastToFAT(firstBlock);
    ret = root.update(stats);
    if (ret < 0) {
        RETURN(-errno);
    }

    RETURN(0);
}

int MyFS::fuseMkdir(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseUnlink(const char *path) {
    LOGM();

    const char* name = path;
    if (*path == '/') {
        if (strlen(path) == 1) {
            name = ".";
        } else {
            name++;
        }
    }

    fileStats file;
    int ret = root.get(name, &file);
    if (ret < 0){
        RETURN(-errno);
    }
    std::list<uint16_t >::const_iterator iterator;
    std::list<uint16_t> list;
    fat.iterateFAT(file.first_block, &list);
    for (iterator = list.begin(); iterator != list.end(); ++iterator){
        dmap.clear(*iterator);
        LOGF("Cleared Block: %d", *iterator);
    }
    fat.deleteFromFAT(file.first_block);

    //superblock.emptySpaceSize += file.size;
    ret = root.deleteEntry(name);
    if (ret < 0) {
        RETURN(-errno);
    }

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

    const char* oldname = path;
    if (*path == '/') {
        if (strlen(path) == 1) {
        } else {
            oldname++;
        }
    }
    const char* newname = newpath;
    if (*newpath == '/') {
        if (strlen(newpath) == 1) {
        } else {
            newname++;
        }
    }

    int fd = root.rename(oldname, newname);
    if (fd == -1){
        RETURN(-errno);
    }
    RETURN(0);
}

int MyFS::fuseLink(const char *path, const char *newpath) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();

    RETURN(0); // TODO: don't lie
    RETURN(-ENOSYS);
}

int MyFS::fuseUtime(const char *path, struct utimbuf *ubuf) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    const char* name = path;
    if (*path == '/') {
        if (strlen(path) == 1) {
            name = ".";
        } else {
            name++;
        }
    }
    LOGF("File Name: %s", name);

    fileStats file;
    int rootIndex = root.get(name, &file);
    if (rootIndex == -1){
        RETURN(-errno);
    }

    bool success = false;
    bool read = false;
    bool write = false;
    if (file.userID == geteuid()) {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("User RDWR");
            if ((file.mode & S_IRWXU) != 0) {
                read = true;
                write = true;
                success = true;
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            LOG("User WRONLY");
            if ((file.mode & S_IWUSR) != 0) {
                write = true;
                success = true;
            }
        } else {
            LOG("User RDONLY");
            if ((file.mode & S_IRUSR) != 0) {
                read = true;
                success = true;
            }
        }
    } else if (file.groupID == getegid()) {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("Group RDWR");
            if ((file.mode & S_IRWXG) != 0) {
                read = true;
                write = true;
                success = true;
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            LOG("Group WRONLY");
            if ((file.mode & S_IWGRP) != 0) {
                write = true;
                success = true;
            }
        } else {
            LOG("Group RDONLY");
            if ((file.mode & S_IRGRP) != 0) {
                read = true;
                success = true;
            }
        }
    } else {
        if ((fileInfo->flags & O_RDWR) != 0) {
            LOG("Other RDWR");
            if ((file.mode & S_IRWXO) != 0) {
                read = true;
                write = true;
                success = true;
            }
        } else if ((fileInfo->flags & O_WRONLY) != 0) {
            LOG("Other WRONLY");
            if ((file.mode & S_IWOTH) != 0) {
                write = true;
                success = true;
            }
        } else {
            LOG("Other RDONLY");
            if ((file.mode & S_IROTH) != 0) {
                read = true;
                success = true;
            }
        }
    }

    if (success) {
        for (int i = 0; i < NUM_OPEN_FILES; i++) {
            if (openFiles[i].rootIndex < 0) {
                openFiles[i].rootIndex = rootIndex;
                openFiles[i].write = write;
                openFiles[i].read = read;
                fileInfo->fh = i;
                RETURN(0);
            }
        }
        errno = EMFILE;
	    RETURN(-errno);
    } else {
        errno = EACCES;
        RETURN(-errno);
    }
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    if (offset < 0){
        offset = 0;
    }

    int fd = fileInfo->fh;

    if ((fd < 0) || (fd >= NUM_DIR_ENTRIES)) {
        errno = EBADF;
        RETURN(-errno);
    }
    if (openFiles[fd].rootIndex < 0 || !(openFiles[fd].read)) {
        errno = EBADF;
        RETURN(-errno);
    }

    int rootIndex = openFiles[fd].rootIndex;
    fileStats file;
    if (root.get(rootIndex, &file) == -1){
        RETURN(-errno);
    }
    if (file.size <= offset){
        RETURN(0); //EOF
    }
    if ((uint64_t)file.size < offset + size) {//TODO uint32_t
        size = file.size - offset;//TODO WHY?
    }

    file.last_time = time(NULL);    //set the access time to current time
    root.update(file);

    off_t blockNo = offset / BLOCK_SIZE; // block number of file (not block number in filesystem!)
    off_t blockOffset = offset % BLOCK_SIZE; // offset in the block

    //number of blocks you need to read for this operation (upper limit)
    int howManyBlocks = (size + blockOffset) / BLOCK_SIZE;
    if ((size + blockOffset) % BLOCK_SIZE != 0) howManyBlocks++;

    uint16_t currentBlock = file.first_block;
    uint16_t blocks[howManyBlocks]; //saves all block locations needed for this operation
    for ( int t = 0; t < blockNo ; t++){ // skip blockNo blocks
        fat.getNext(currentBlock, &currentBlock);
    }
    for(int i = 0; i < howManyBlocks; i++){ // save blocks for reading
        blocks[i] = currentBlock;
        fat.getNext(currentBlock, &currentBlock);
    }

    char buffer[BLOCK_SIZE];
    size_t readSize;
    if (blockOffset + size < BLOCK_SIZE) {
        readSize = size;
    } else {
        readSize = BLOCK_SIZE - (size_t)blockOffset;
    }
    if (openFiles[fd].bufferBlockNumber == blocks[0]) {
        memcpy(buf, openFiles[fd].buffer + blockOffset, readSize);
    } else {
        blockDevice->read(DATA_START + blocks[0], buffer);
        memcpy(buf, buffer + blockOffset, readSize);
        if (howManyBlocks == 1) {
            memcpy(openFiles[fd].buffer, buffer, BLOCK_SIZE);
            openFiles[fd].bufferBlockNumber = blocks[howManyBlocks - 1];
        }
    }
    for (int j = 1; j < howManyBlocks - 1; j++) {
        blockDevice->read(DATA_START + blocks[j], buf - blockOffset + BLOCK_SIZE * j); //Why (-blockOffset)?
	LOGF("Block %d wird gelesen",blocks[j]);//new testing TODO delete
    }
    if (howManyBlocks > 1) {
        blockDevice->read(DATA_START + blocks[howManyBlocks - 1], buffer);
        memcpy(buf - blockOffset + (howManyBlocks - 1) * BLOCK_SIZE, buffer, (size + blockOffset) % BLOCK_SIZE); //TODO check whether it shloud be memcpy(openfiles[fd].buffer, ...

        memcpy(openFiles[fd].buffer, buffer, BLOCK_SIZE);
        openFiles[fd].bufferBlockNumber = blocks[howManyBlocks - 1];
    }

    RETURN((int)size);
}

int MyFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    if (offset < 0){
      offset = 0;
    }

    int fd = fileInfo->fh;
    if (fd < 0 || fd >= NUM_DIR_ENTRIES) {
        errno = EBADF;
        RETURN(-errno);
    }
    if (openFiles[fd].rootIndex < 0 || !(openFiles[fd].write)) { // is file open in write mode?
        errno = EBADF;
        RETURN(-errno);
    }
    int rootIndex = openFiles[fd].rootIndex;
    fileStats file;
    if (root.get(rootIndex, &file) == -1){ // get file stats
        RETURN(-errno);
    }
    if (file.size < offset){
       size_t sizeZero = offset - file.size;
       char bufferZero[sizeZero];
       for (int i = 0; i < (int)sizeZero; i++) {
           bufferZero[i] = 0;
       }

       int ret = fuseWrite(path, bufferZero, sizeZero, file.size, fileInfo);
       if (ret < 0) { RETURN(-errno); };
    }

    file.last_time = time(NULL);
    file.modi_time = time(NULL);
    root.update(file);

    off_t blockNo = offset / BLOCK_SIZE; // block number of file (not block number in filesystem!)
    off_t blockOffset = offset % BLOCK_SIZE; // offset in the block

    //number of blocks you need to write in this operation (upper limit)
    int howManyBlocks = (int)(size + blockOffset) / BLOCK_SIZE;
    if ((size + blockOffset) % BLOCK_SIZE != 0) howManyBlocks++;

    uint16_t currentBlock = file.first_block;
    for (int t = 0; t < blockNo ; t++){ // skip blockNo blocks
        fat.getNext(currentBlock, &currentBlock);
    }

    uint16_t blocks[howManyBlocks]; //saves all block locations needed for this operation
    uint16_t previousBlock = currentBlock;
    int existingBlockCount = 0;
    while (currentBlock != FAT_TERMINATOR && existingBlockCount < howManyBlocks) {
        blocks[existingBlockCount] = currentBlock;
        existingBlockCount++;
        previousBlock = currentBlock;
        fat.getNext(currentBlock, &currentBlock);
    }
    int missingBlockCount = howManyBlocks - existingBlockCount;
    uint16_t nextBlock = currentBlock;
    if (missingBlockCount > 0) {
        while (missingBlockCount > 0) {
            int ret = dmap.getFreeBlock(&nextBlock);
            if (ret < 0) { RETURN(-errno); };
            dmap.set(nextBlock);
            fat.addNextToFAT(previousBlock, nextBlock);
            blocks[howManyBlocks - missingBlockCount] = nextBlock;
            previousBlock = nextBlock;

            missingBlockCount--;
        }
        fat.addLastToFAT(nextBlock);
    }

    char buffer[BLOCK_SIZE];
    size_t writeSize;
    if (blockOffset + size < BLOCK_SIZE) {
        writeSize = size;
    } else {
        writeSize = BLOCK_SIZE - (size_t)blockOffset;
    }
    if (openFiles[fd].bufferBlockNumber == blocks[0]) {
        memcpy(openFiles[fd].buffer + blockOffset, buf, writeSize);
        blockDevice->write(DATA_START + blocks[0], openFiles[fd].buffer);
    } else {
        blockDevice->read(DATA_START + blocks[0], buffer);
        memcpy(buffer + blockOffset, buf, writeSize);
        blockDevice->write(DATA_START + blocks[0], buffer);
    }
    for (int j = 1; j < howManyBlocks - 1; j++) {
        blockDevice->write(DATA_START + blocks[j], buf - blockOffset + BLOCK_SIZE * j);
    }
    if (howManyBlocks > 1) {
        blockDevice->read(DATA_START + blocks[howManyBlocks - 1], buffer);
        memcpy(buffer, buf - blockOffset + (howManyBlocks - 1) * BLOCK_SIZE, (size + blockOffset) % BLOCK_SIZE);
        blockDevice->write(DATA_START + blocks[howManyBlocks - 1], buffer);
    }

    for (int i = 0; i < NUM_OPEN_FILES; i++) {
        if (openFiles[i].rootIndex == rootIndex) {
            openFiles[i].bufferBlockNumber = FAT_TERMINATOR;
        }
    }
    if ((uint64_t)file.size < offset + size) {
        file.size = offset + size;
        int ret = root.update(file);
        if (ret < 0) { RETURN(-errno); };
    }
    RETURN((int)size);
}

int MyFS::fuseStatfs(const char *path, struct statvfs *statInfo) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseFlush(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();

    if (fileInfo->fh < 0 || fileInfo->fh >= NUM_DIR_ENTRIES) {
        errno = EBADF;
        RETURN(-errno);
    }
    if (openFiles[fileInfo->fh].rootIndex >= 0) {
        openFiles[fileInfo->fh].rootIndex = -1;
        openFiles[fileInfo->fh].read = false;
        openFiles[fileInfo->fh].write = false;
        openFiles[fileInfo->fh].bufferBlockNumber = FAT_TERMINATOR;
        RETURN(0);
    } else {
        errno = EBADF;
        RETURN(-errno);
    }
}

int MyFS::fuseFsync(const char *path, int datasync, struct fuse_file_info *fi) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseListxattr(const char *path, char *list, size_t size) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseRemovexattr(const char *path, const char *name) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseOpendir(const char *path, struct fuse_file_info *fileInfo) {
  LOGM();

  RETURN(0); // always grants access
}

int MyFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

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

    RETURN(0);
}

int MyFS::fuseFsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    LOGM();
    return -ENOSYS;
}

int MyFS::fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    RETURN(0); // TODO: don't lie
    RETURN(-ENOSYS);
}

int MyFS::fuseCreate(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
    LOGM();
    dev_t dev = 0;
    int ret = fuseMknod(path, mode, dev);
    if (ret < 0) {
        RETURN(ret);
    }
    ret = fuseOpen(path, fileInfo);
    if (ret < 0) {
        RETURN(ret);
    }

    RETURN(0);
}

void MyFS::fuseDestroy() {
    LOGM();

    blockDevice->close();
}

void* MyFS::fuseInit(struct fuse_conn_info *conn) {
    // Open logfile
    this->logFile= fopen(((MyFsInfo*) fuse_get_context()->private_data)->logFile, "w+");
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


        initializeFilesystem(((MyFsInfo *) fuse_get_context()->private_data)->contFile);
    }

    RETURN(0);
}

#ifdef __APPLE__
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t x) {
#else
int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
#endif
    LOGM();
    return -ENOSYS;
}

#ifdef __APPLE__
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size, uint x) {
#else
int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size) {
#endif
    LOGM();
    return -ENOSYS;
}

// TODO: Add your own additional methods here!

int MyFS::initializeFilesystem(char* containerFile) {
    if (blockDevice->open(containerFile) == 0) {
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

        delete[] dMapArray;
        delete[] fatArray;
        delete[] rootArray;

        for (int i = 0; i < NUM_OPEN_FILES; i++) {
            openFiles[i].rootIndex = -1;
            openFiles[i].read = false;
            openFiles[i].write = false;
            openFiles[i].bufferBlockNumber = FAT_TERMINATOR;
        }

        RETURN(0);
    }
    LOG("Error at blockdevice.open()");
    RETURN(-1);
}
