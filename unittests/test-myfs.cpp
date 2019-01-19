//
//  test-myfs.cpp
//  testing
//
//  Created by Oliver Waldhorst on 15.12.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//
#include <iostream>

#include "catch.hpp"

#include "helper.hpp"
#include "myfs.h"

#define TEST_FILESYSTEM "Test-Filesystem"
#define TEST_FILE "Testfile"

// TODO: Write tests

TEST_CASE("MyFS.Methode", "[MyFS]") {
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " Makefile");
    myfs->initializeFilesystem(TEST_FILESYSTEM);

    SECTION("Beschreibung") {


    }

    // TODO: Implement test. This is a template for tests.
}

TEST_CASE("MyFS.write", "[MyFS]") {

    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM TEST_FILE);
    myfs->initializeFilesystem(TEST_FILESYSTEM);
    fuse_file_info fileInfo = {};

    SECTION("Anfang der Datei schreiben"){
        const char* writebuffer= {"Test test test"};
        size_t size = 14;
        char* readbuffer = (char*) malloc(14);
        off_t offset = 0;
        fileInfo.flags = O_RDWR;

        myfs->fuseOpen(TEST_FILE, &fileInfo);
        myfs->fuseWrite(TEST_FILE, writebuffer, size, offset, &fileInfo);

        myfs->fuseRead(TEST_FILE, readbuffer, size, offset, &fileInfo);

        std::cout << readbuffer << std::endl;
        std::cout << writebuffer << std::endl;
        REQUIRE(memcmp(readbuffer, writebuffer, size) == 0);


    }


}






