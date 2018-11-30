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
        for(int i = 0; i < DATA_BLOCKS - 1; i++){
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
        std::list<uint16_t> readList;

        fat.addLastToFAT(0);
        fat.iterateFAT(0, &readList);
        REQUIRE(readList.size() == 1);
    }
    SECTION("Override value"){
        FAT fat = FAT();
        std::list<uint16_t> readList;

        fat.addToFAT(0, 6458);
        fat.addLastToFAT(0);
        fat.iterateFAT(0, &readList);
        REQUIRE(readList.size() == 1);
    }
    SECTION("Multiple values"){
        FAT fat = FAT();

        fat.addToFAT(345, 765);
        fat.addToFAT(345, 34);
        fat.addToFAT(345, 8);
        fat.addToFAT(345, 9261);
        fat.addLastToFAT(9261);

        std::list<uint16_t> list;
        fat.iterateFAT(345, &list);
        REQUIRE(list.size() == 5);
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
    SECTION("Multiple values"){
        FAT fat = FAT();

        std::list<uint16_t> values;
        values.push_back(345);
        values.push_back(765);
        values.push_back(34);
        values.push_back(8);
        values.push_back(9261);
        fat.addToFAT(345, 765);
        fat.addToFAT(345, 34);
        fat.addToFAT(345, 8);
        fat.addToFAT(345, 9261);
        fat.addLastToFAT(9261);

        std::list<uint16_t> list;
        std::list<uint16_t >::const_iterator iterator;
        std::list<uint16_t >::const_iterator valIter;
        fat.iterateFAT(345, &list);
        bool allSame = true;
        for (iterator = list.begin(), valIter = values.begin(); iterator != list.end(); ++iterator, valIter++){
            if (*iterator != *valIter) {
                allSame = false;
                break;
            }
        }
        REQUIRE(allSame);
    }
}
