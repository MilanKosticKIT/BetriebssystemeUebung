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



TEST_CASE("DMap.setAll / DMap.getAll", "[DMap]") {
    SECTION("Completely empty/same Array"){
        DMap dmap = DMap();
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        uint8_t readArray[(DATA_BLOCKS + 1) / 8];
        for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
            dMapArray[i] = 3;
            readArray[i] = 1;
        }
        dmap.setAll(dMapArray);
        dmap.getAll(readArray);
        REQUIRE(memcmp(dMapArray, readArray, sizeof(dMapArray)) == 0);
    }
    SECTION("Random Array content"){
        DMap dmap = DMap();
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        uint8_t readArray[(DATA_BLOCKS + 1) / 8];
        dmap.setAll(dMapArray);
        dmap.getAll(readArray);
        REQUIRE(memcmp(dMapArray, readArray, sizeof(dMapArray)) == 0);
    }
}

TEST_CASE("DMap.clear", "[DMap]") {
    SECTION("Clearing bit that is set") {
        DMap dmap = DMap();
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        uint8_t readArray[(DATA_BLOCKS + 1) / 8];
        readArray[0] =  dMapArray[0] = 0xff; // set first entry to the same value
        dmap.setAll(dMapArray);
        dMapArray[0] = 0x7f; // "clear" dmapArray[0]
        dmap.clear(0); // clear dmap[0]
        dmap.getAll(readArray); // read cleared dmap
        REQUIRE(dMapArray[0] == readArray[0]);
    }
    SECTION("Clearing bit that is already clear") {
        DMap dmap = DMap();
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        uint8_t readArray[(DATA_BLOCKS + 1) / 8];
        readArray[0] = dMapArray[0] = 0x7f;
        dmap.setAll(dMapArray);
        dmap.clear(0);
        dmap.getAll(readArray);
        REQUIRE(readArray[0] == dMapArray[0]);
    }
    SECTION("Clearing multiple bits in dMap") {
        DMap dmap = DMap();
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        uint8_t readArray[(DATA_BLOCKS + 1) / 8];
        for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
            switch (i % 3) { // write multiple different values
                case 0: dMapArray[i] = readArray[i] = 0xD2; break;
                case 1: dMapArray[i] = readArray[i] = 0x39; break;
                default: dMapArray[i] = readArray[i] = 0x76; break;
            }
        }
        dmap.setAll(dMapArray);
        for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
            dmap.clear((uint16_t)(i * 8 + (i % 8))); // with clear
            uint8_t clearMask = ~((uint8_t) 1 << (7 - (i % 8))); // clear manually
            dMapArray[i] &= clearMask;
        }
        dmap.getAll(readArray);
        REQUIRE(memcmp(readArray, dMapArray, sizeof(dMapArray)) == 0);
    }
}

TEST_CASE("DMap.set" , "[DMap]") {
    SECTION("Setting empty bit") {
        DMap dmap = DMap();
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        uint8_t readArray[(DATA_BLOCKS + 1) / 8];
        dMapArray[0] = 0;
        dmap.setAll(dMapArray);
        dMapArray[0] = 0x80;
        dmap.set(0);
        dmap.getAll(readArray);
        REQUIRE(dMapArray[0] == readArray[0]);
    }
    SECTION("Setting bit that is already set") {
        DMap dmap = DMap();
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        uint8_t readArray[(DATA_BLOCKS + 1) / 8];
        readArray[0] = 0;
        dMapArray[0] = 0x80;
        dmap.setAll(dMapArray);
        dmap.set(0);
        dmap.getAll(readArray);
        REQUIRE(dMapArray[0] == readArray[0]);
    }
    SECTION("Setting multiple bits in dMap") {
        DMap dmap = DMap();
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        uint8_t readArray[(DATA_BLOCKS + 1) / 8];
        for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
            switch (i % 3) { // write multiple different values
                case 0: dMapArray[i] = readArray[i] = 0xD2; break;
                case 1: dMapArray[i] = readArray[i] = 0x39; break;
                default: dMapArray[i] = readArray[i] = 0x76; break;
            }
        }
        dmap.setAll(dMapArray);
        for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
            dmap.set((uint16_t)(i * 8 + (i % 8))); // with clear
            uint8_t setMask = (uint8_t) 1 << (7 - (i % 8)); // clear manually
            dMapArray[i] |= setMask;
        }
        dmap.getAll(readArray);
        REQUIRE(memcmp(readArray, dMapArray, sizeof(dMapArray)) == 0);
    }
}

TEST_CASE("DMap.getFreeBlock, with set blocks" , "[DMap]") {
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

TEST_CASE("DMap.getFreeBlock, with set and cleared blocks" , "[DMap]") {
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

TEST_CASE("DMap.getFreeBlock, with setAll", "[DMap]") {
    SECTION("Empty array") {
        DMap dmap = DMap();
        char dmapArray[(DATA_BLOCKS + 1) / 8];
        for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
            dmapArray[i] = 0;
        }
        dmap.setAll(dmapArray);

        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 0);
    }
    SECTION("First n blocks are set") {
        DMap dmap = DMap();
        char dmapArray[(DATA_BLOCKS + 1) / 8];
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
        char dmapArray[(DATA_BLOCKS + 1) / 8];
        dmapArray[0] = (char) 0;
        dmap.setAll(dmapArray);

        uint16_t firstFreeBlock;
        dmap.getFreeBlock(&firstFreeBlock);
        REQUIRE(firstFreeBlock == 0);
    }
}

TEST_CASE("DMap.getFreeBlock, with full dmap", "[DMap]") {
    SECTION("Full dmap with setAll") {
        DMap dmap = DMap();
        char dmapArray[(DATA_BLOCKS + 1) / 8];
        for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
            dmapArray[i] = (char) 0xFF;
        }
        dmap.setAll(dmapArray);

        uint16_t firstFreeBlock;
        REQUIRE(dmap.getFreeBlock(&firstFreeBlock) < 0); // Error code
    }
}
