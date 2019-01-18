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
        system("mkdir folder");
        system("cp Makefile ./folder/Makefile");
        int status = system("./mkfs.myfs Binary.bin Makefile ./file/Makefile");
        system("rmdir folder -rf");
        REQUIRE(status != 0);
    }
}
TEST_CASE("Creating Filesystem with a lot files", "[mkfs.myfs]") {
    //Create files
    system("touch 1.txt");
    system("touch 2.txt");
    system("touch 3.txt");
    system("touch 4.txt");
    system("touch 5.txt");
    system("touch 6.txt");
    system("touch 7.txt");
    system("touch 8.txt");
    system("touch 9.txt");
    system("touch 10.txt");
    system("touch 11.txt");
    system("touch 12.txt");
    system("touch 13.txt");
    system("touch 14.txt");
    system("touch 15.txt");
    system("touch 16.txt");
    system("touch 17.txt");
    system("touch 18.txt");
    system("touch 19.txt");
    system("touch 20.txt");
    system("touch 21.txt");
    system("touch 22.txt");
    system("touch 23.txt");
    system("touch 24.txt");
    system("touch 25.txt");
    system("touch 26.txt");
    system("touch 27.txt");
    system("touch 28.txt");
    system("touch 29.txt");
    system("touch 30.txt");
    system("touch 31.txt");
    system("touch 32.txt");
    system("touch 33.txt");
    system("touch 34.txt");
    system("touch 35.txt");
    system("touch 36.txt");
    system("touch 37.txt");
    system("touch 38.txt");
    system("touch 39.txt");
    system("touch 40.txt");
    system("touch 41.txt");
    system("touch 42.txt");
    system("touch 43.txt");
    system("touch 44.txt");
    system("touch 45.txt");
    system("touch 46.txt");
    system("touch 47.txt");
    system("touch 48.txt");
    system("touch 49.txt");
    system("touch 50.txt");
    system("touch 51.txt");
    system("touch 52.txt");
    system("touch 53.txt");
    system("touch 54.txt");
    system("touch 55.txt");
    system("touch 56.txt");
    system("touch 57.txt");
    system("touch 58.txt");
    system("touch 59.txt");
    system("touch 60.txt");
    system("touch 61.txt");
    system("touch 62.txt");
    system("touch 63.txt");
    system("touch 64.txt");
    system("touch 65.txt");
    
    SECTION("More than Max Files (65)") {
        REQUIRE(system("./mkfs.myfs Binary.bin 1.txt 2.txt 3.txt 4.txt 5.txt 6.txt 7.txt 8.txt 9.txt 10.txt 11.txt 12.txt 13.txt 14.txt 15.txt 16.txt 17.txt 18.txt 19.txt 20.txt 21.txt 22.txt 23.txt 24.txt 25.txt 26.txt 27.txt 28.txt 29.txt 30.txt 31.txt 32.txt 33.txt 34.txt 35.txt 36.txt 37.txt 38.txt 39.txt 40.txt 41.txt 42.txt 43.txt 44.txt 45.txt 46.txt 47.txt 48.txt 49.txt 50.txt 51.txt 52.txt 53.txt 54.txt 55.txt 56.txt 57.txt 58.txt 59.txt 60.txt 61.txt 62.txt 63.txt 64.txt 65.txt") == 0x1700);
    }
    
    SECTION("Max Files (64)") {
        REQUIRE(system("./mkfs.myfs Binary.bin 1.txt 2.txt 3.txt 4.txt 5.txt 6.txt 7.txt 8.txt 9.txt 10.txt 11.txt 12.txt 13.txt 14.txt 15.txt 16.txt 17.txt 18.txt 19.txt 20.txt 21.txt 22.txt 23.txt 24.txt 25.txt 26.txt 27.txt 28.txt 29.txt 30.txt 31.txt 32.txt 33.txt 34.txt 35.txt 36.txt 37.txt 38.txt 39.txt 40.txt 41.txt 42.txt 43.txt 44.txt 45.txt 46.txt 47.txt 48.txt 49.txt 50.txt 51.txt 52.txt 53.txt 54.txt 55.txt 56.txt 57.txt 58.txt 59.txt 60.txt 61.txt 62.txt 63.txt 64.txt ") == 0);
    }
    
    SECTION("63 Files (Max -1)") {
        REQUIRE(system("./mkfs.myfs Binary.bin 1.txt 2.txt 3.txt 4.txt 5.txt 6.txt 7.txt 8.txt 9.txt 10.txt 11.txt 12.txt 13.txt 14.txt 15.txt 16.txt 17.txt 18.txt 19.txt 20.txt 21.txt 22.txt 23.txt 24.txt 25.txt 26.txt 27.txt 28.txt 29.txt 30.txt 31.txt 32.txt 33.txt 34.txt 35.txt 36.txt 37.txt 38.txt 39.txt 40.txt 41.txt 42.txt 43.txt 44.txt 45.txt 46.txt 47.txt 48.txt 49.txt 50.txt 51.txt 52.txt 53.txt 54.txt 55.txt 56.txt 57.txt 58.txt 59.txt 60.txt 61.txt 62.txt 63.txt") == 0);
    }
    
    //Delete files
    system("rm 1.txt");
    system("rm 2.txt");
    system("rm 3.txt");
    system("rm 4.txt");
    system("rm 5.txt");
    system("rm 6.txt");
    system("rm 7.txt");
    system("rm 8.txt");
    system("rm 9.txt");
    system("rm 10.txt");
    system("rm 11.txt");
    system("rm 12.txt");
    system("rm 13.txt");
    system("rm 14.txt");
    system("rm 15.txt");
    system("rm 16.txt");
    system("rm 17.txt");
    system("rm 18.txt");
    system("rm 19.txt");
    system("rm 20.txt");
    system("rm 21.txt");
    system("rm 22.txt");
    system("rm 23.txt");
    system("rm 24.txt");
    system("rm 25.txt");
    system("rm 26.txt");
    system("rm 27.txt");
    system("rm 28.txt");
    system("rm 29.txt");
    system("rm 30.txt");
    system("rm 31.txt");
    system("rm 32.txt");
    system("rm 33.txt");
    system("rm 34.txt");
    system("rm 35.txt");
    system("rm 36.txt");
    system("rm 37.txt");
    system("rm 38.txt");
    system("rm 39.txt");
    system("rm 40.txt");
    system("rm 41.txt");
    system("rm 42.txt");
    system("rm 43.txt");
    system("rm 44.txt");
    system("rm 45.txt");
    system("rm 46.txt");
    system("rm 47.txt");
    system("rm 48.txt");
    system("rm 49.txt");
    system("rm 50.txt");
    system("rm 51.txt");
    system("rm 52.txt");
    system("rm 53.txt");
    system("rm 54.txt");
    system("rm 55.txt");
    system("rm 56.txt");
    system("rm 57.txt");
    system("rm 58.txt");
    system("rm 59.txt");
    system("rm 60.txt");
    system("rm 61.txt");
    system("rm 62.txt");
    system("rm 63.txt");
    system("rm 64.txt");
    system("rm 65.txt");
}


