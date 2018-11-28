//
//  test-dmap.cpp
//  unittests
//
//  Created by MK on 21.11.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#include "catch.hpp"

#include <string.h>
#include <iostream>

#include "helper.hpp"
#include "constants.h"
#include "dmap.h"

#define BD_PATH "Test_Blockdevice.bin"

/*
 TEST_CASE( "dmap empty on creation", "[dmap]" ) {
 //TODO: Implement this :/
 //Is beeing taken care of when creating the dmap.
 //Therefore dmap.settAll has to work!
 }
 */

TEST_CASE("DMap.setAll / DMap.getAll", "[DMap]") {
    SECTION("Completly empty/same Array"){
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        uint8_t readArray[DATA_BLOCKS / 8];
        for (int i = 0; i < DATA_BLOCKS / 8; i++) {
            dMapArray[i] = 3;
            readArray[i] = 1;
        }
        dmap.setAll((char *) dMapArray);
        dmap.getAll((char *) readArray);
        REQUIRE(memcmp(dMapArray, readArray, sizeof(DATA_BLOCKS)) == 0);
    }
    SECTION("First entry different, rest of Array simmilar"){
        //Just to be sure my tests work. If this one gets marked as failed, all Sections of this Test_Case have to be corrected. (Short run!)
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        uint8_t readArray[DATA_BLOCKS / 8];
        for (int i = 0; i < DATA_BLOCKS / 8; i++) {
            dMapArray[i] = 0;
        }
        dmap.setAll((char *) dMapArray);
        dmap.getAll((char *) readArray);
        dMapArray[0] = 1;
        REQUIRE(memcmp(dMapArray, readArray, 1) == 1);
        dMapArray[0] = readArray[0];
        REQUIRE(memcmp(dMapArray, readArray, sizeof(DATA_BLOCKS -1)) == 0);
    }
    SECTION("First entry same, rest different") {
        //Just to be sure my tests work. If this one gets marked as failed, all Sections of this Test_Case have to be corrected. (Short run!)
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        uint8_t readArray[DATA_BLOCKS / 8];
        for (int i = 0; i < DATA_BLOCKS / 8; i++) {
            dMapArray[i] = 0;
        }
        dmap.setAll((char *) dMapArray);
        dmap.getAll((char *) readArray);
        for (int i = 0; i < DATA_BLOCKS / 8; i++) {
            dMapArray[i] = 42;
        }
        REQUIRE(memcmp(dMapArray, readArray, sizeof(DATA_BLOCKS)) == 42);
    }
}

TEST_CASE("Dmap.clear", "[DMap]") {
    SECTION("Clearing bit that is set") {
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        uint8_t readArray[DATA_BLOCKS / 8];
        readArray[0] =  dMapArray[0] = 0xff;
        for (int i = 1; i < DATA_BLOCKS / 8; i++) {
            readArray[i] = dMapArray[i] = 0;
        }
        dmap.setAll((char *) dMapArray);
        dMapArray[0] = 0x7f;
        dmap.clear(0);
        dmap.getAll((char *) readArray);
        REQUIRE(memcmp(dMapArray, readArray, sizeof(DATA_BLOCKS)) == 0);
    }
    SECTION("Clearing bit that is already clear") {
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        uint8_t readArray[DATA_BLOCKS / 8];
        readArray[0] = 0;
        dMapArray[0] = 0x7f;
        for (int i = 1; i < DATA_BLOCKS / 8; i++) {
            readArray[i] = dMapArray[i] = 0;
        }
        dmap.setAll((char *) dMapArray);
        dmap.clear(0);
        dmap.getAll((char *) readArray);
        REQUIRE(memcmp(dMapArray, readArray, sizeof(DATA_BLOCKS)) == 0);
    }
    SECTION("Accessing address that is to high") {
        //TODO: Define behaviour.
        //TODO: Implement behaviour.
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        for (int i = 0; i < DATA_BLOCKS / 8; i++) {
            dMapArray[i] = 0;
        }
        dmap.setAll((char *) dMapArray);
        dmap.clear(65535);
    }
}

TEST_CASE("Dmap.set" , "[DMap]") {
    SECTION("Setting empty bit") {
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        uint8_t readArray[DATA_BLOCKS / 8];
        for (int i = 0; i < DATA_BLOCKS / 8; i++) {
            readArray[i] = dMapArray[i] = 0;
        }
        dmap.setAll((char *) dMapArray);
        dMapArray[0] = 0x80;
        dmap.set(0);
        dmap.getAll((char *) readArray);
        REQUIRE(memcmp(dMapArray, readArray, sizeof(DATA_BLOCKS)) == 0);
    }
    SECTION("Setting bit that is already set") {
        //TODO: Define behaviour.
        //TODO: Implement behaviour.
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        uint8_t readArray[DATA_BLOCKS / 8];
        readArray[0] = 0;
        dMapArray[0] = 0x80;
        for (int i = 1; i < DATA_BLOCKS / 8; i++) {
            readArray[i] = dMapArray[i] = 0;
        }
        dmap.setAll((char *) dMapArray);
        dmap.set(0);
        dmap.getAll((char *) readArray);
        REQUIRE(memcmp(dMapArray, readArray, sizeof(DATA_BLOCKS)) == 0);
    }
    SECTION("Accessing address that is to high") {
        //TODO: Define behaviour.
        //TODO: Implement behaviour.
        DMap dmap = DMap();
        uint8_t dMapArray[DATA_BLOCKS / 8];
        for (int i = 1; i < DATA_BLOCKS / 8; i++) {
            dMapArray[i] = 0;
        }
        dmap.setAll((char *) dMapArray);
        dmap.set(65535);
    }
}

TEST_CASE("Dmap.getFreeBlock, with set blocks" , "[DMap]") {
    SECTION("Initial values of dmap") {
        DMap dmap = DMap();
        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 0);
    }
    SECTION("The first n blocks are set") {
        DMap dmap = DMap();
        for (uint16_t i = 0; i < 234; i++) {
            dmap.set(i);
        }
        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 234);
    }
    SECTION("Blocks are set with empty block between them") {
        DMap dmap = DMap();
        for (uint16_t i = 0; i < 51; i++) {
            dmap.set(i);
        }
        for (uint16_t i = 52; i < 163; i++) {
            dmap.set(i);
        }
        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 51);
    }
    SECTION("Some blocks are set, first block is free") {
        DMap dmap = DMap();
        for (uint16_t i = 43; i < 82; i++) {
            dmap.set(i);
        }
        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 0);
    }
}

TEST_CASE("Dmap.getFreeBlock, with set and cleared blocks" , "[DMap]") {
    SECTION("The first n blocks are set, one of them is cleared") {
        DMap dmap = DMap();
        for (uint16_t i = 0; i < 123; i++) {
            dmap.set(i);
        }
        dmap.clear(43);
        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 43);
    }
    SECTION("The first n blocks are set, the first is cleared") {
        DMap dmap = DMap();
        for (uint16_t i = 0; i < 51; i++) {
            dmap.set(i);
        }
        dmap.clear(0);
        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 0);
    }
    SECTION("The first n blocks are set, a block > n is cleared") {
        DMap dmap = DMap();
        for (uint16_t i = 0; i < 82; i++) {
            dmap.set(i);
        }
        dmap.clear(425);
        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 82);
    }
    SECTION("The first n blocks are set, many blocks are cleared") {
        DMap dmap = DMap();
        for (uint16_t i = 0; i < 351; i++) {
            dmap.set(i);
        }
        dmap.clear(425);
        dmap.clear(42);
        dmap.clear(5456);
        dmap.clear(33); // smallest number
        dmap.clear(54);
        for (uint16_t i = 101; i < 167; i++) {
            dmap.clear(i);
        }
        dmap.clear(234);
        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 33);
    }
}

TEST_CASE("Dmap.getFreeBlock, with setAll", "[DMap") {
    SECTION("Empty array") {
        DMap dmap = DMap();
        char dmapArray[DATA_BLOCKS / 8];
        for (int i = 0; i < DATA_BLOCKS / 8; i++) {
            dmapArray[i] = 0;
        }
        dmap.setAll(dmapArray);

        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 0);
    }
    SECTION("First n blocks are set") {
        DMap dmap = DMap();
        char dmapArray[DATA_BLOCKS / 8];
        for (int i = 0; i < 67; i++) {
            dmapArray[i] = (char) 0xFF;
        }
        dmapArray[67] = (char) 0xCF; // 11001111
        dmap.setAll(dmapArray);

        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        // (x * 8) - 1 is first free block for first x BYTES set.
        REQUIRE(firstFreeBlock == 67 * 8 + 2);
    }
    SECTION("First blocks is empty") {
        DMap dmap = DMap();
        char dmapArray[DATA_BLOCKS / 8];
        dmapArray[0] = (char) 0;
        dmap.setAll(dmapArray);

        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 0);
    }
}

TEST_CASE("Dmap.getFreeBlock, with full dmap", "DMap") {
    SECTION("Full dmap with setAll") {
        DMap dmap = DMap();
        char dmapArray[DATA_BLOCKS / 8];
        for (int i = 0; i < DATA_BLOCKS / 8; i++) {
            dmapArray[i] = (char) 0xFF;
        }
        dmap.setAll(dmapArray);

        uint16_t firstFreeBlock;
        REQUIRE(dmap.getFreeBlock(&firstFreeBlock) < 0); // Error code
    }
}
