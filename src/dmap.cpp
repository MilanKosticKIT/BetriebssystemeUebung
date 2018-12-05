//
//  DMap.cpp
//  mkfs.myfs
//
//  Created by MK on 22.10.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#include "dmap.h"
#include <errno.h>

DMap::DMap(){
    for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
        dMapValues[i] = 0;
    }
    firstFreeAddress = 0;
}



//Marks an address as empty.
void DMap::clear(uint16_t clearAddress) {
    int byte = clearAddress / 8;
    int bit = clearAddress % 8;

    uint8_t* byteToChange = dMapValues + byte;
    *byteToChange &= ~(1 << (7 - bit));

    if (firstFreeAddress > clearAddress) { // if a block before first free address is cleared
        firstFreeAddress = clearAddress; // set new first free block
    }
}

//Marks an address as full
void DMap::set(uint16_t setAddress) {
    int byte = setAddress / 8;
    int bit = setAddress % 8;

    uint8_t* byteToChange = dMapValues + byte;
    *byteToChange |= (1 << (7 - bit));

    if (firstFreeAddress >= setAddress) { // if the first free block was set
        findFirstFreeAddress(setAddress); // search next free block
    }
}

//Called to check wehther an address is full
bool DMap::isAdressFull(uint16_t blockNo) {
    int byte = blockNo / 8;
    int bit = blockNo % 8;

    uint8_t* byteToRead = dMapValues + byte;
    return (*byteToRead & (1 << (7 - bit)));
}

//Returns -1 when nothing found, >=0  when something found
//Starting search for new freeBlock at startAddress
int DMap::findFirstFreeAddress(uint16_t startAddress){
    //For not read only Version:
    uint16_t currentByte = startAddress / 8;
    uint8_t currentBit;

    for (; currentByte * 8 <= ADDRESS_MAX; currentByte++) {

        if (dMapValues[currentByte] != 0xFF) {
            currentBit = 0;
            while (currentBit < 8) {//run over bit
                if (~(dMapValues[currentByte] | ~(1 << (7 - currentBit)))) {
                    firstFreeAddress = currentByte * 8 + currentBit;
                    return 0;
                }
                currentBit++;
            }
        }
    }
    // set first free address to terminator. (No free address)
    firstFreeAddress = ADDRESS_MAX + 1;
    return -1;
}

//Returns the first empty block.
//When an error occurs -1 is returned, else 0.
int DMap::getFreeBlock(uint16_t* freeBlock){
    if (firstFreeAddress <= ADDRESS_MAX) {
        *freeBlock = firstFreeAddress;
        return 0;
    } else {
        errno = ENOSPC;
        return -1;
    }
}

void DMap::getAll(char* p) {
    for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++){
        *((uint8_t*) p + i) = dMapValues[i];
    }
}

void DMap::setAll(char* p) {
    for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++){
        dMapValues[i] = (uint8_t) *(p + i);
    }
    findFirstFreeAddress(0);
}
