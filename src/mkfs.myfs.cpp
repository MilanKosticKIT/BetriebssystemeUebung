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

void convertBlockToMetaData(MetaData* data, char* block);
void convertMetaDataToBlock(MetaData* data, char* block);
//TODO: Get these lines to a fitting header file.
BlockDevice blockDevice = *new BlockDevice();

int main(int argc, char *argv[]) {
    
    
    // TODO: Implement file system generation & copying of files here
    return 0;
}

//MARK: - Our Methods

//Called to say an Address got empty
void clearPointInDMAP(u_int16_t clearAddress) {
    uint32_t blockNoDMAP = DMAP_START + (clearAddress / (BLOCK_SIZE * 8));
    char* block = (char*) malloc(BLOCK_SIZE);
    blockDevice.read(blockNoDMAP, block);
    int bit = clearAddress % 8;
    int byte = clearAddress % BLOCK_SIZE;
    char* byteToChange = block + byte;
    *byteToChange &= ~(1 << bit);
    blockDevice.write(blockNoDMAP, block);
    free(block);
}

//Called to say an Address was filled
void setPointInDMAP(u_int16_t setAddress) {
    uint32_t blockNoDMAP = DMAP_START + (setAddress / (BLOCK_SIZE * 8));
    char* block = (char*) malloc(BLOCK_SIZE);
    blockDevice.read(blockNoDMAP, block);
    int bit = setAddress % 8;
    int byte = setAddress % BLOCK_SIZE;
    char* byteToChange = block + byte;
    *byteToChange |= (1 << bit);
    blockDevice.write(blockNoDMAP, block);
    free(block);
}

//Called to check wehther an address is full
bool isAdressFull(u_int16_t blockNo) {
    uint32_t blockNoDMAP = DMAP_START + (blockNo / (BLOCK_SIZE * 8));
    char* block = (char*) malloc(BLOCK_SIZE);
    blockDevice.read(blockNoDMAP, block);
    int bit = blockNo % 8;
    int byte = blockNo % BLOCK_SIZE;
    char* byteToRead = block + byte;
    bool isSet (*byteToRead & (1 << bit));
    free(block);
    return isSet;
}

//Called to get the next address (from 0 to 65535)
/*
u_int16_t getAddress(u_int16_t currentAddress) {
    u_int32_t blockNo, byteNo;
    getBLockFromAddress(currentAddress, &blockNo, &byteNo);
    blockNo += FAT_START;
    char* buffer;
    blockDevice.read(blockNo, buffer);
    u_int16_t* content = buffer;
    return content[byteNo];
    // TODO: Is probably wrong
}

//Called to set the address for an datablock (from 0 to 65535)
void setAddress(u_int16_t currentAddress, u_int16_t nextAddress) {
    u_int32_t blockNo, byteNo;
    getBLockFromAddress(currentAddress, &blockNo, &byteNo);
    blockNo += FAT_START;
    char* buffer;
    blockDevice.read(blockNo, buffer);
    u_int16_t* content = buffer;
    content[byteNo] = nextAddress;
    blockDevice.write(blockNo, buffer);
    // TODO: Is probably wrong
}*/

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
    *blockNo = address / BLOCK_SIZE;
    *byteNo = address % BLOCK_SIZE;
}

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
