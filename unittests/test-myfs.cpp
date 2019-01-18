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

// TODO: Write tests

TEST_CASE("MyFS.Methode", "[MyFS]") {
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " Makefile");
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    SECTION("Beschreibung") {
        // TODO: Implement test. This is a template for tests.
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}

TEST_CASE("MyFS.open, MyFS.close", "[MyFS]") {
#define TEST_FILE "Makefile"
#define NONEXISTING_FILE "Nonexisting"
#define BUFFER_SIZE 100
    MyFS *myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);
    fuse_file_info fileInfo = {};

    SECTION("Open one file read-only, try reading/writing and close file") {
        fileInfo.flags = O_RDONLY;
        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        char buffer[BUFFER_SIZE];
        size_t size = BUFFER_SIZE;
        off_t offset = 0;
        ret = myfs->fuseRead(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseWrite(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == -9); // EBADF
        ret = myfs->fuseRelease(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
    }
    SECTION("Open one file write-only, try reading/writing and close file") {
        fileInfo.flags = O_WRONLY;
        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        char buffer[BUFFER_SIZE];
        size_t size = BUFFER_SIZE;
        off_t offset = 0;
        ret = myfs->fuseRead(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == -9); // EBADF
        ret = myfs->fuseWrite(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseRelease(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
    }
    SECTION("Open one file read-write and close file") {
        fileInfo.flags = O_RDWR;
        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseRelease(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
    }

    SECTION("Open nonexisting file read-only") {
        fileInfo.flags = O_RDONLY;
        int ret = myfs->fuseOpen(NONEXISTING_FILE, &fileInfo);
        REQUIRE(ret == -2); //ENOENT
        ret = myfs->fuseRelease(NONEXISTING_FILE, &fileInfo);
        REQUIRE(ret == -9); //EBADF
    }
    SECTION("Open nonexisting file write-only") {
        fileInfo.flags = O_WRONLY;
        int ret = myfs->fuseOpen(NONEXISTING_FILE, &fileInfo);
        REQUIRE(ret == -2); //ENOENT
        ret = myfs->fuseRelease(NONEXISTING_FILE, &fileInfo);
        REQUIRE(ret == -9); //EBADF
    }
    SECTION("Open nonexisting file read-write") {
        fileInfo.flags = O_RDWR;
        int ret = myfs->fuseOpen(NONEXISTING_FILE, &fileInfo);
        REQUIRE(ret == -2); //ENOENT
        ret = myfs->fuseRelease(NONEXISTING_FILE, &fileInfo);
        REQUIRE(ret == -9); //EBADF
    }

    SECTION("Close existing not opened file file") {
        int ret = myfs->fuseRelease(TEST_FILE, &fileInfo);
        REQUIRE(ret == -9); //EBADF
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}
