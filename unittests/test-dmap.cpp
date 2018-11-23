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

TEST_CASE("DMap.setAll / DMap.getAll", "[dmap]") {
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
