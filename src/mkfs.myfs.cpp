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


int main(int argc, char *argv[]) {

    // TODO: Implement file system generation & copying of files here
    return 0;
}

//MARK: - Our Methods

//Called to change the dmap bit of an address
void changeDMAP(u_int16_t address, bool mode) {
    u_int32_t blockNo = DMAP_START + (address + DMAP_SIZE);
    char* block;
    BlockDevice::read(blockNo, block);
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
    BlockDevice::read(blockNo, block);
    int bit = askedAddress % BLOCK_SIZE;
    char toChange = block[bit / sizeof(char)];
    bit %= sizeof(char);
    return bool (toChange & (1<< bit)); //0 if the bit of toChange was 0, !=0 otherwise
}

//Called to get the next address
u_int16_t getAddress(u_int16_t currentAddress) {
    //TODO: Implement this.
}

//Called to set the address for an datablock
void setAddress(u_int16_t currentAddress, u_int16_t nextAddress) {
    //TODO: Implement this.
}

//
MetaData getMetaData(u_int8_t indexOfFile) {
    //TODO: Implement this.
}

//Sets the
void setMetaData(MetaData metaData) {
    //TODO: Implement this.
}

