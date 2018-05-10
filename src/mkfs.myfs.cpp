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

void convertBlockToMetaData(MetaData* data, char* block);
void convertMetaDataToBlock(MetaData* data, char* block);
//TODO: Get these lines to a fitting header file.
BlockDevice blockDevice = *new BlockDevice();

int main(int argc, char *argv[]) {
    
    
    // TODO: Implement file system generation & copying of files here
    return 0;
}

//MARK: - Our Methods

//Called to change the dmap bit of an address
void changeDMAP(u_int16_t address, bool mode) {
    u_int32_t blockNo = DMAP_START + (address + DMAP_SIZE);
    char* block;
    blockDevice.read(blockNo, block);
    int bit = address % BLOCK_SIZE;
    char toChange = block[bit / sizeof(char)];
    bit %= sizeof(char);
    toChange = toChange & ~(1 << bit);     //Clear bit
    toChange = toChange | (mode << bit);   //Set bit to mode

    //TODO: Check wether these operators fit.
}

//Called to say an Address got empty
void clearPointInDMAP(u_int16_t deleteAddress) {
    changeDMAP(deleteAddress, 0);
    
    
}

//Called to say an Address was filled
void setPointInDMAP(u_int16_t filledAddress) {
    changeDMAP(filledAddress, 1);
}

//Called to check wehther an address is full
bool getDMAP(u_int16_t askedAddress) {
    u_int32_t blockNo = DMAP_START + (askedAddress + DMAP_SIZE);
    char* block;
    blockDevice.read(blockNo, block);
    int bit = askedAddress % BLOCK_SIZE;
    char toChange = block[bit / sizeof(char)];
    bit %= sizeof(char);
    return bool (toChange & (1<< bit)); //0 if the bit of toChange was 0, !=0 otherwise
}

//Called to get the next address (from 0 to 65535)
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
}

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
