//
//  test-fat.cpp
//  unittests
//
//  Created by MK on 28.11.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#include "catch.hpp"

#include <string.h>
#include <iostream>

#include "helper.hpp"
#include "constants.h"
#include "fat.h"

#define BD_PATH "Test_Blockdevice.bin"

TEST_CASE("FAT.setALL / FAT.getAll", "[FAT]"){
    SECTION("Normally initalized FAT used"){
        FAT fat = FAT();
        uint16_t fatArrary[DATA_BLOCKS];
        uint16_t readArray[DATA_BLOCKS];
        
        fatArrary[0] = 0;
        for(uint16_t i = 0; i < DATA_BLOCKS - 1; i++){
            fatArrary[i + 1] = 0;
        }
        
        fat.getAll((char * ) readArray);
        memcmp(fatArrary, readArray, sizeof(fatArrary));
    }
    SECTION("Manual to -1 set FAT"){
        FAT fat = FAT();
        uint16_t fatArrary[DATA_BLOCKS];
        uint16_t readArray[DATA_BLOCKS];
        
        fatArrary[0] = -1;
        for(uint16_t i = 0; i < DATA_BLOCKS - 1; i++){
            fatArrary[i + 1] = -1;
        }
        
        fat.setAll((char * ) fatArrary);
        fat.getAll((char * ) readArray);
        memcmp(fatArrary, readArray, sizeof(fatArrary));
    }
}
