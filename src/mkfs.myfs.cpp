//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <type_traits>

typedef struct{
    char name[NAME_LENGTH];
    off_t size;
    uid_t userID;
    gid_t groupID;
    time_t last_time;
    time_t modi_time;
    time_t change_time;
}fileStats;

//MARK: - Methodenheader
//DMAP
//TODO: Root?
MetaData getMetaData(u_int8_t indexOfFile);
void setMetaData(MetaData metaData, u_int8_t indexOfFile);
void convertBlockToMetaData(MetaData* data, char* block);
void convertMetaDataToBlock(MetaData* data, char* block);
void getBLockFromAddress(u_int32_t address, u_int32_t* blockNo, u_int32_t* byteNo);
//Superblock
void getSuperBlock(SuperBlock* superblock);
void setEmptySpaceSizeInSuperBlock(uint32_t emptySpaceSize);
void createSuperBlock(struct SuperBlock superBlock);

//TODO: Get these lines to a fitting header file.
//MARK: -

BlockDevice blockDevice = *new BlockDevice();


bool getStats(fileStats *status, char *path, blkcnt_t *blockCount){
    struct stat sb;
    stat(path,&sb);
    char* filename = basename(path);
    if (strlen(filename) > NAME_LENGTH) {
        return false;
    }
    strcpy(status->name, filename);//TODO: Pointer?!?!
    status->size = sb.st_size;
    status->userID = geteuid();
    status->groupID = getegid();
    status->modi_time = sb.st_mtime;
    time(&(status->last_time));
    time(&(status->change_time));
    *blockCount = sb.st_blocks;
    return true;
}
void updateRoot(fileStats *status){
    fileStats root[NUM_DIR_ENTRIES];
    
    
    
    
}

template <class T> void writeDevice(std::size_t block, const T& data) {
    static_assert(std::is_trivially_copyable<T>::value, "Can't operate on complex types!");
    
    const char* rawData = reinterpret_cast<const char*>(&data);
    
    static char buffer[BLOCK_SIZE];
    std::size_t blockCount = sizeof(T) / BLOCK_SIZE;
    std::size_t currentBlock = block;
    for(; currentBlock < block + blockCount; ++currentBlock) {
        blockDevice.write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
    }
    std::memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), sizeof(T) % BLOCK_SIZE);
    blockDevice.write(currentBlock, buffer);
}

template <class T, std::size_t N> void writeDevice(std::size_t block, const T (&data)[N]) {
    static_assert(std::is_trivially_copyable<T>::value, "Can't operate on complex types!");
    static_assert(sizeof(T) * N <= BLOCK_SIZE, "");
    
    static char buffer[BLOCK_SIZE];
    std::memcpy(buffer, data, sizeof(T) * N);
    blockDevice.write(block, buffer);
}

template<class T> void readDevice(std::size_t block, T& data) {
    static_assert(std::is_trivially_constructible<T>::value, "Can't operate on complex types!");
    static_assert(sizeof(T) <= BLOCK_SIZE, "");
    
    static char buffer[BLOCK_SIZE];
    blockDevice.read(block, buffer);
    std::memcpy(data, buffer, sizeof(T));
}

template<class T, std::size_t N> void readDevice(std::size_t block, T (&data)[N]) {
    static_assert(std::is_trivially_constructible<T>::value, "Can't operate on complex types!");
    static_assert(sizeof(T) * N <= BLOCK_SIZE, "");
    
    static char buffer[BLOCK_SIZE];
    blockDevice.read(block, buffer);
    std::memcpy(data, buffer, sizeof(T) * N);
}



int main(int argc, char *argv[]) {
    
    blockDevice.open("./Blockdevice.bin");
    
    /*
    fileStats foobar;
    foobar.size = 1024;
    writeDevice(15, foobar);
    
    fileStats bar;
    readDevice(15, &bar);
    
    std::cout << bar.size << std::endl;
     */
    
    /*
    char input[6] = {'a', 'b', 'c', 'd', 'e', '\0'};
    char putput[BLOCK_SIZE];
    char output[6];
    char* outputString = (char*) output;
    */
    int input[6] = {0, 1, 2, 3, 4, 5};
    int output[6];
    
    writeDevice(10, input);
    readDevice(10, output);
    for(int i = 0; i< 6; i++){
        std::cout << input[i] << std::endl;
    }
    for(int i = 0; i< 6; i++){
        std::cout << output[i] << std::endl;
    }
    
    // TODO: Implement file system generation & copying of files here
    return 0;
    
}










//MARK: - Our Methods

//MARK: - DMAP

//MARK: - Root
//Returns the metaData of the file behind the index.
MetaData getMetaData(u_int8_t indexOfFile) {
    u_int32_t blockNo = ROOT_START + indexOfFile;
    char* block;
    blockDevice.read(blockNo, block);
    MetaData data;
    convertBlockToMetaData(&data, block);       //TODO: Pls check deferensation, (and spelling).
    return data;
}

//Sets the MetaData of an file, which is refrenced via the index.
void setMetaData(MetaData metaData, u_int8_t indexOfFile) {
    u_int32_t blockNo = ROOT_START + indexOfFile;
    char* block;
    convertBlockToMetaData(&metaData, block);   //TODO: Pls check deferensation, (and spelling).
    blockDevice.write(blockNo, block);
}

//Casts the given block to a given MetaData instance
void convertBlockToMetaData(MetaData* data, char* block){
    //TODO: Implement this.
}

//Converts the given MetaData into a block, formatted as char*/String
void convertMetaDataToBlock(MetaData* data, char* block){
    //TODO: Implement this.
}

//Gets the block and the byte number of the given address
void getBLockFromAddress(u_int32_t address, u_int32_t* blockNo, u_int32_t* byteNo) {
    //TODO: Check whether needed
    *blockNo = address / BLOCK_SIZE;
    *byteNo = address % BLOCK_SIZE;
}



//MARK: - Superblock
//Returns the Superblock for the data system
void getSuperBlock(SuperBlock* superblock){
    //    TODO: Implement this.
    char* block = (char*) malloc(BLOCK_SIZE);
    blockDevice.read(0, block);
    
    uint32_t* data = (uint32_t*) block;
    superblock->fileSystemSize = *data;
    data++;
    superblock->emptySpaceSize = *data;
    data++;
    superblock->maximumStorageSize = *data;
    data++;
    superblock->dmapStart = *data;
    data++;
    superblock->fatStart = *data;
    data++;
    superblock->rootStart = *data;
    data++;
    superblock->dataStart = *data;
    data++;
    superblock->dmapSize = *data;
    data++;
    superblock->fatSize = *data;
    data++;
    superblock->rootSize = *data;
    data++;
    superblock->dataSize = *data;
    free(data);
}

//Sets the emptySpaceSize arttibute in the superblock
void setEmptySpaceSizeInSuperBlock(uint32_t emptySpaceSize){
    char* block = (char*) malloc(BLOCK_SIZE);
    uint32_t* data = (uint32_t*) block;
    
    data++;
    *data = emptySpaceSize;
    
    blockDevice.write(0, block);
    free(data);
}

//Writes the part of the filesystem for the superblock for the first time. In particular: The not changing values are written.
void createSuperBlock(struct SuperBlock superBlock){
    char* block = (char*) malloc(BLOCK_SIZE);
    uint32_t* data = (uint32_t*) block;
    
    *data = superBlock.fileSystemSize;
    data++;
    *data = superBlock.emptySpaceSize;
    data++;
    *data = superBlock.maximumStorageSize;
    data++;
    *data = superBlock.dmapStart;
    data++;
    *data = superBlock.fatStart;
    data++;
    *data = superBlock.rootStart;
    data++;
    *data = superBlock.dataStart;
    data++;
    *data = superBlock.dmapSize;
    data++;
    *data = superBlock.fatSize;
    data++;
    *data = superBlock.rootSize;
    data++;
    *data = superBlock.dataSize;
    data++;
    
    blockDevice.write(0, block);
    free(data);
}
