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
            readArray[i + 1] = -1;
        }
        
        fat.getAll((char * ) readArray);
        REQUIRE(memcmp(fatArrary, readArray, sizeof(fatArrary)));
    }
    SECTION("Manual to -1 set FAT"){
        FAT fat = FAT();
        uint16_t fatArrary[DATA_BLOCKS];
        uint16_t readArray[DATA_BLOCKS];
        
        for(int i = 0; i < DATA_BLOCKS - 1; i++){
            fatArrary[i] = -1;
        }
        
        fat.setAll((char * ) fatArrary);
        fat.getAll((char * ) readArray);
        memcmp(fatArrary, readArray, sizeof(fatArrary));
    }
    SECTION("Manually added various address, with terminator"){
        FAT fat = FAT();
        uint16_t fatArray[DATA_BLOCKS];
        uint16_t readArray[DATA_BLOCKS];
        
        for (int i = 0; i < DATA_BLOCKS; i++) {
            fatArray[i] = 0;
        }
        fatArray[1] = 123;
        fatArray[123] = 3;
        fatArray[3] = 41183;
        fatArray[41183] = -1;
        
        fat.setAll((char *) fatArray);
        fat.getAll((char *) readArray);
        REQUIRE(memcmp(fatArray, readArray, sizeof(fatArray)));
    }
}

TEST_CASE("FAT.addlasttoFAT", "[FAT]"){
    SECTION("On empty FAT"){
        FAT fat = FAT();
        uint16_t fatArray[DATA_BLOCKS];
        std::list<uint16_t> readList;
        
        for (int i = 0; i < DATA_BLOCKS; i++) {
            fatArray[i] = 0;
        }
        
        fat.setAll((char *) fatArray);
        fat.addLastToFAT(0);
        fat.iterateFAT(0, &readList);
        REQUIRE(readList.size() == 1);
    }
}

TEST_CASE("FAT.addToFAT", "[FAT]"){
    SECTION("On empty FAT"){
        FAT fat = FAT();
        uint16_t fatArray[DATA_BLOCKS];
        uint16_t readArray[DATA_BLOCKS];
        
        for (int i = 0; i< DATA_BLOCKS; i++) {
            readArray[i] = fatArray[i] = 0;
        }
        
//        fat.setAll(<#char *p#>)
    }
}
