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
#define TEST_FILE "Makefile"
#define NONEXISTENT_FILE "Nonexistent"

// TODO: Write tests

TEST_CASE("MyFS.Methode", "[MyFS]") {
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    fuse_file_info fileInfo = {};

    SECTION("Beschreibung") {
        // TODO: Implement test. This is a template for tests.
        REQUIRE(0 == 0); // TODO: add assertions
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}


TEST_CASE("MyFS.write", "[MyFS]") {

    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*)TEST_FILESYSTEM);
    fuse_file_info fileInfo = {};

    SECTION("Anfang der Datei schreiben"){
        const char writebuffer[] = {"Test test test"};
        size_t size = sizeof(writebuffer);
        char readbuffer[size];
        off_t offset = 0;
        fileInfo.flags = O_RDWR;

        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseWrite(TEST_FILE, writebuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseRead(TEST_FILE, readbuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}


TEST_CASE("MyFS.open, MyFS.close", "[MyFS]") {
    MyFS *myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);
    fuse_file_info fileInfo = {};

    SECTION("Open one file read-only, try reading/writing and close file") {
        fileInfo.flags = O_RDONLY;
        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        size_t size = 100;
        char buffer[size];
        off_t offset = 0;
        ret = myfs->fuseRead(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseWrite(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == -EBADF);
        ret = myfs->fuseRelease(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
    }
    SECTION("Open one file write-only, try reading/writing and close file") {
        fileInfo.flags = O_WRONLY;
        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        size_t size = 100;
        char buffer[size];
        off_t offset = 0;
        ret = myfs->fuseRead(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == -EBADF);
        ret = myfs->fuseWrite(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseRelease(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
    }
    SECTION("Open one file read-write, try reading/writing and close file") {
        fileInfo.flags = O_RDWR;
        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        size_t size = 100;
        char buffer[size];
        off_t offset = 0;
        ret = myfs->fuseRead(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseWrite(TEST_FILE, buffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseRelease(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
    }

    SECTION("Open nonexistent file read-only") {
        fileInfo.flags = O_RDONLY;
        int ret = myfs->fuseOpen(NONEXISTENT_FILE, &fileInfo);
        REQUIRE(ret == -ENOENT);
        ret = myfs->fuseRelease(NONEXISTENT_FILE, &fileInfo);
        REQUIRE(ret == -EBADF);
    }
    SECTION("Open nonexistent file write-only") {
        fileInfo.flags = O_WRONLY;
        int ret = myfs->fuseOpen(NONEXISTENT_FILE, &fileInfo);
        REQUIRE(ret == -ENOENT);
        ret = myfs->fuseRelease(NONEXISTENT_FILE, &fileInfo);
        REQUIRE(ret == -EBADF);
    }
    SECTION("Open nonexistent file read-write") {
        fileInfo.flags = O_RDWR;
        int ret = myfs->fuseOpen(NONEXISTENT_FILE, &fileInfo);
        REQUIRE(ret == -ENOENT);
        ret = myfs->fuseRelease(NONEXISTENT_FILE, &fileInfo);
        REQUIRE(ret == -EBADF);
    }

    SECTION("Close existing not opened file file") {
        int ret = myfs->fuseRelease(TEST_FILE, &fileInfo);
        REQUIRE(ret == -EBADF);
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}

TEST_CASE("MyFS.read", "[MyFS]") {
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    fuse_file_info fileInfo = {};

    SECTION("Compare with test file outside of filesystem") {
        int fd = open((char*)TEST_FILE, O_RDONLY);
        REQUIRE(fd >= 0);
        fileInfo.flags = O_RDONLY;
        REQUIRE(myfs->fuseOpen(TEST_FILE, &fileInfo) == 0);

        size_t size = 100;
        off_t offset = 0;
        char buffer[size];
        char otherBuffer[size];

        ssize_t readBytes = 0;
        int ret = 0;
        bool sameValue = true;
        do {
            readBytes = read(fd, buffer, size);
            ret = myfs->fuseRead(TEST_FILE, otherBuffer, size, offset, &fileInfo);
            offset += size;
            if(readBytes != ret || memcmp(buffer, otherBuffer, (size_t)readBytes) != 0) {
                sameValue = false;
                break;
            }
        } while(readBytes != 0);

        close(fd);
        myfs->fuseRelease((char*)TEST_FILE, &fileInfo);

        REQUIRE(sameValue);
    }

    SECTION("Try reading not opened file") {
        fileInfo.flags = O_RDONLY;
        size_t size = 100;
        off_t offset = 0;
        char buffer[size];
        int ret = myfs->fuseRead(TEST_FILE, buffer, size , offset, &fileInfo);
        REQUIRE(ret == -EBADF);
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}

TEST_CASE("MyFS.getAttr, Existing File", "[MyFS]"){
    system("touch Test_File_2.txt");
    system("chmod 0444 Test_File_2.txt");
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " Test_File_2.txt");
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    SECTION("File has All Read Permission") {
        struct stat *buf = new struct stat();
        REQUIRE(buf->st_mode == (0444 | S_IFREG));
        delete(buf);
    }


    system("rm Test_File_2.txt");
}
