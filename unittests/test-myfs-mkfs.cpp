//
//  test-myfs-mkfs.cpp
//  unittests
//
//  Created by MK on 05.12.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.


#include "catch.hpp"

#include <string.h>
#include <iostream>

#include "myfs-structs.h"
#include "helper.hpp"
#include "constants.h"

//TODO: Get working
TEST_CASE("Creating simple Filsystem","[mkfs.myfs]"){
    SECTION("Creating empty filesystem"){
        REQUIRE(system("./mkfs.myfs Binary.bin") == 0);
    }
    SECTION("Creating FS with small file"){
        REQUIRE(system("./mkfs.myfs Binary.bin Makefile") == 0);
    }
    SECTION("Using not exsitng File to copy into FS"){
        REQUIRE(system("./mkfs.myfs Binary.bin neverExisting.bin") != 0);
    }
    SECTION("Using same file to copy twice") {
        REQUIRE(system("./mkfs.myfs Binary.bin Makefile Makefile") != 0);
    }
    SECTION("Using files with same name from different directory") {
        REQUIRE(system("./mkfs.myfs Binary.bin Makefile ./file/Makefile") != 0);
    }
}
