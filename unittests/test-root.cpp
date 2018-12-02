//
//  test-root.cpp
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
#include "root.h"

TEST_CASE("Root.setAll / Root.getAll", "[Root]"){
    SECTION("Standard initialized Root"){
        Root root = Root();
        fileStats rootArray[ROOT_ARRAY_SIZE];
        root.getAll(rootArray);
        bool successfulInit = true;
        for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
            if (rootArray[i].size != -1) {
                successfulInit = false;
                break;
            }
        }
        REQUIRE(successfulInit);
    }

    SECTION("Random Content"){
        Root root = Root();
        fileStats rootArray[ROOT_ARRAY_SIZE];
        fileStats readArray[ROOT_ARRAY_SIZE];
        root.setAll(rootArray);
        root.getAll(readArray);
        REQUIRE(memcmp(rootArray, readArray, ROOT_ARRAY_SIZE) == 0);
    }

}

TEST_CASE("Root.createEntry / Root.get, Methods with filename as selector", "[Root]"){
    SECTION("Standard initialized Root"){
        Root root = Root();
        int ret = root.createEntry("TestEntry");
        REQUIRE(ret >= 0);
        fileStats entry;
        ret = root.get("TestEntry", &entry);
        REQUIRE(ret >= 0);
    }
    SECTION(""){

        REQUIRE(false); // todo write test
    }
    SECTION(""){

        REQUIRE(false); // todo write test
    }
}