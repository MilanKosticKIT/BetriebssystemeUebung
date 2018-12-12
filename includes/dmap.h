//
//  DMap.hpp
//  mkfs.myfs
//
//  Created by MK on 22.10.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#ifndef DMap_hpp
#define DMap_hpp

#include <stdio.h>
#include <stdint.h>
#include "myfs-structs.h"
#include "constants.h"

#define ADDRESS_MAX (DATA_BLOCKS - 1)

class DMap{

    uint16_t firstFreeAddress;
    uint8_t *dMapValues = new uint8_t[(DATA_BLOCKS + 1) / 8];

    int findFirstFreeAddress(uint16_t startAddress);
    bool isAdressFull(uint16_t blockNo);

public:
    DMap();
    void clear(uint16_t clearAddress);
    void set(uint16_t setAddress);
    int getFreeBlock(uint16_t* freeBlock);

    void getAll(char* p);
    void setAll(char* p);

};

#endif /* DMap_hpp */
