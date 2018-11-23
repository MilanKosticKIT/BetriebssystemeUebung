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
    DMap dmap = DMap();
    uint8_t dMapArray[DATA_BLOCKS / 8];
    uint8_t readArray[DATA_BLOCKS / 8];
    for (int i = 0; i < DATA_BLOCKS / 8; i++) {
        dMapArray[i] = 0;
    }
    dmap.setAll((char *) dMapArray);
    dmap.getAll((char *) readArray);
    REQUIRE(memcmp(dMapArray, readArray, sizeof(DATA_BLOCKS)) == 0);
}
/*
TEST_CASE("testcase_name", "[dmap]" ) {
    SECTION("name of section") {
        remove(BD_PATH);
        testData input;
        testData output;
        input.before = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        input.first = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        input.second = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
        input.third = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
        input.fourth = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
        input.fifth = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
        input.after = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        
        fsIO.writeDevice(2, input);
        fsIO.readDevice(2, output);
        
        REQUIRE(memcmp(&input, &output, sizeof(testData)) == 0);
        remove(BD_PATH);
    }
}
 */
