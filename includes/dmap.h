//
//  DMap.hpp
//  mkfs.myfs
//
//  Created by MK on 22.10.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#ifndef DMap_hpp
#define DMap_hpp
#define ADDRESS_MAX 1

#include <stdio.h>
#include <stdint.h>
#include "myfs-structs.h"

class DMap{
    
    int firstFreeAddress;
    uint8_t* dMapValues;
    
    int findFirstFreeAddress(uint16_t startAddress);
    bool isAdressFull(uint16_t blockNo);
public:
    
    DMap();
    bool checkFreeSpace(uint16_t blockCount);
    void clear(uint16_t clearAddress);
    void set(uint16_t setAddress);
    int getFreeBlock(uint32_t* freeBlock);
    
    //TODO: Besprechung ob noetig.
    void writeDMap();
    void readDMap();
    
};

#endif /* DMap_hpp */


