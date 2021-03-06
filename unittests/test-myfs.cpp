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
#define TESTFILE_SIZE 2640

// TODO: Write tests

TEST_CASE("MyFS.Methode", "[MyFS]") {
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    fuse_file_info fileInfo = {};

    SECTION("Beschreibung") {
        //createFile((char*)"Test.max", DATA_BYTES);
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

    SECTION("Try writing at the beginning of a file"){
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

        REQUIRE ((memcmp(readbuffer, writebuffer, size) == 0));
    }

    SECTION("Try writing a not opened file") {
        fileInfo.flags = O_RDONLY;
        size_t size = 100;
        off_t offset = 0;
        const char buffer[100] = "test test test";
        int ret = myfs->fuseWrite(TEST_FILE, buffer, size , offset, &fileInfo);
        REQUIRE(ret == -EBADF);
    }

    SECTION("Try writing at the end of a file"){
        const char writebuffer[] = {"Test test test"};
        size_t size = sizeof(writebuffer);
        char readbuffer[size];
        off_t offset = TESTFILE_SIZE;
        fileInfo.flags = O_RDWR;

        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseWrite(TEST_FILE, writebuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseRead(TEST_FILE, readbuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);

        REQUIRE ((memcmp(readbuffer, writebuffer, size) == 0));
    }

    SECTION("Try writing in the middle of a file"){
        const char writebuffer[] = {"Test test test"};
        size_t size = sizeof(writebuffer);
        char readbuffer[size];
        off_t offset = TESTFILE_SIZE/2;
        fileInfo.flags = O_RDWR;

        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseWrite(TEST_FILE, writebuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseRead(TEST_FILE, readbuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);

        REQUIRE ((memcmp(readbuffer, writebuffer, size) == 0));
    }

    SECTION("Try writing with a negative offset"){
        const char writebuffer[] = {"Test test test"};
        size_t size = sizeof(writebuffer);
        char readbuffer[size];
        off_t offset = -100;
        fileInfo.flags = O_RDWR;

        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseWrite(TEST_FILE, writebuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseRead(TEST_FILE, readbuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
    }

    SECTION("Try writing over two blocks"){
        const char writebuffer[] = {"Test test test Test test test Test test test"};
        size_t size = sizeof(writebuffer);
        char readbuffer[size];
        off_t offset = BLOCK_SIZE - 1;
        fileInfo.flags = O_RDWR;

        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseWrite(TEST_FILE, writebuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseRead(TEST_FILE, readbuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);

        REQUIRE ((memcmp(readbuffer, writebuffer, size) == 0));

    }

    SECTION("Try writing with an offset greater than the filesize"){
        const char writebuffer[] = {"Test test test Test test test Test test test"};
        size_t size = sizeof(writebuffer);
        char readbuffer[size];
        off_t offset = TESTFILE_SIZE + 50;
        fileInfo.flags = O_RDWR;

        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
        ret = myfs->fuseWrite(TEST_FILE, writebuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        ret = myfs->fuseRead(TEST_FILE, readbuffer, size, offset, &fileInfo);
        REQUIRE(ret == size);
        char zeroBuffer[10];
        char testZeroBuffer[10] = {0,0,0,0,0,0,0,0,0};
        myfs->fuseRead(TEST_FILE, zeroBuffer, 10, TESTFILE_SIZE + 5, &fileInfo);
        REQUIRE(memcmp(zeroBuffer, testZeroBuffer, 9) == 0);

        REQUIRE ((memcmp(readbuffer, writebuffer, size) == 0));

    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);

}


TEST_CASE("MyFS.open / MyFS.close", "[MyFS]") {
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

    SECTION("Compare with test file outside of filesystem (small pieces)") {
        int fd = open((char*)TEST_FILE, O_RDONLY);
        REQUIRE(fd >= 0);
        fileInfo.flags = O_RDONLY;
        REQUIRE(myfs->fuseOpen(TEST_FILE, &fileInfo) == 0);

        size_t size = 10;
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

    SECTION("Repeatable read") {
        fileInfo.flags = O_RDONLY;
        REQUIRE(myfs->fuseOpen(TEST_FILE, &fileInfo) == 0);

        size_t size = 100;
        off_t offset = 0;
        char buffer[size];
        char otherBuffer[size];

        int ret = 0;
        int otherRet = 0;
        bool sameValue = true;

        ret = myfs->fuseRead(TEST_FILE, buffer, size, offset, &fileInfo);
        otherRet = myfs->fuseRead(TEST_FILE, otherBuffer, size, offset, &fileInfo);

        REQUIRE(ret == otherRet);
        REQUIRE(memcmp(buffer, otherBuffer, (size_t) ret) == 0);

        size = 47;
        offset = 321;
        ret = myfs->fuseRead(TEST_FILE, buffer, size, offset, &fileInfo);
        otherRet = myfs->fuseRead(TEST_FILE, otherBuffer, size, offset, &fileInfo);
        REQUIRE(ret == otherRet);
        REQUIRE(memcmp(buffer, otherBuffer, (size_t) ret) == 0);

        size = 6;
        offset = 867;
        ret = myfs->fuseRead(TEST_FILE, buffer, size, offset, &fileInfo);
        otherRet = myfs->fuseRead(TEST_FILE, otherBuffer, size, offset, &fileInfo);
        REQUIRE(ret == otherRet);
        REQUIRE(memcmp(buffer, otherBuffer, (size_t) ret) == 0);

        myfs->fuseRelease((char*)TEST_FILE, &fileInfo);

        REQUIRE(sameValue);
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}

TEST_CASE("MyFS, various tests with large files", "[MyFS]") {
    std::cout << "Tests with large files. Please be patient." << std::endl;
    MyFS* myfs = new MyFS();

    createFile((char*)"TestDatei", DATA_BYTES);
    system("./mkfs.myfs " TEST_FILESYSTEM " " "TestDatei");
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    fuse_file_info fileInfo = {};
    struct stat stats;

    SECTION("Compare with large test file outside of filesystem, chunks of 100 byte") {
        int fd = open((char*)"TestDatei", O_RDONLY);
        REQUIRE(fd >= 0);
        fileInfo.flags = O_RDONLY;
        REQUIRE(myfs->fuseOpen((char*)"TestDatei", &fileInfo) == 0);

        size_t size = 100;
        off_t offset = 0;
        char buffer[size];
        char otherBuffer[size];

        ssize_t readBytes = 0;
        int ret = 0;
        bool sameValue = true;
        do {
            readBytes = read(fd, buffer, size);
            ret = myfs->fuseRead((char*)"TestDatei", otherBuffer, size, offset, &fileInfo);
            offset += size;
            if(readBytes != ret || memcmp(buffer, otherBuffer, (size_t)readBytes) != 0) {
                sameValue = false;
                break;
            }
        } while(readBytes != 0);

        close(fd);
        myfs->fuseRelease((char*)"TestDatei", &fileInfo);

        REQUIRE(sameValue);
    }

    SECTION("Compare with large test file outside of filesystem, chunks of 8192 byte") {
        int fd = open((char*)"TestDatei", O_RDONLY);
        REQUIRE(fd >= 0);
        fileInfo.flags = O_RDONLY;
        REQUIRE(myfs->fuseOpen((char*)"TestDatei", &fileInfo) == 0);

        size_t size = 8192;
        off_t offset = 0;
        char buffer[size];
        char otherBuffer[size];

        ssize_t readBytes = 0;
        int ret = 0;
        bool sameValue = true;
        do {
            readBytes = read(fd, buffer, size);
            ret = myfs->fuseRead((char*)"TestDatei", otherBuffer, size, offset, &fileInfo);
            offset += size;
            if(readBytes != ret || memcmp(buffer, otherBuffer, (size_t)readBytes) != 0) {
                sameValue = false;
                break;
            }
        } while(readBytes != 0);

        close(fd);
        myfs->fuseRelease((char*)"TestDatei", &fileInfo);

        REQUIRE(sameValue);
    }

    SECTION("Compare with large test file outside of filesystem, chunks of 512 byte") {
        int fd = open((char*)"TestDatei", O_RDONLY);
        REQUIRE(fd >= 0);
        fileInfo.flags = O_RDONLY;
        REQUIRE(myfs->fuseOpen((char*)"TestDatei", &fileInfo) == 0);

        size_t size = 512;
        off_t offset = 0;
        char buffer[size];
        char otherBuffer[size];

        ssize_t readBytes = 0;
        int ret = 0;
        bool sameValue = true;
        do {
            readBytes = read(fd, buffer, size);
            ret = myfs->fuseRead((char*)"TestDatei", otherBuffer, size, offset, &fileInfo);
            offset += size;
            if(readBytes != ret || memcmp(buffer, otherBuffer, (size_t)readBytes) != 0) {
                sameValue = false;
                break;
            }
        } while(readBytes != 0);

        close(fd);
        myfs->fuseRelease((char*)"TestDatei", &fileInfo);

        REQUIRE(sameValue);
    }

    SECTION("Create file with full filesystem") {
        REQUIRE(myfs->fuseCreate((char*)NONEXISTENT_FILE, 0777, &fileInfo) == -ENOSPC);
    }

    SECTION("Delete file with full filesystem and then create new file") {
        REQUIRE(myfs->fuseUnlink("TestDatei") == 0);
        REQUIRE(myfs->fuseGetattr("TestDatei", &stats) == -ENOENT);
        REQUIRE(myfs->fuseCreate((char*)NONEXISTENT_FILE, 0777, &fileInfo) >= 0);
        REQUIRE(myfs->fuseUnlink((char*)NONEXISTENT_FILE) == 0);
    }

    SECTION("Write file and compare files, chunks of 100 byte") {
        REQUIRE(myfs->fuseUnlink("TestDatei") == 0);
        REQUIRE(myfs->fuseMknod("TestDatei", 0666, 0) == 0);

        int fd = open((char*)"TestDatei", O_RDWR);
        REQUIRE(fd >= 0);
        fileInfo.flags = O_RDWR;
        REQUIRE(myfs->fuseOpen((char*)"TestDatei", &fileInfo) == 0);

        size_t size = 100;
        off_t offset = 0;
        char buffer[size];

        ssize_t readBytes = 0;
        int ret = 0;
        bool success = true;
        do {
            readBytes = read(fd, buffer, size);
            if (readBytes < 0) {
                success = false;
                break;
            }
            ret = myfs->fuseWrite((char*)"TestDatei", buffer, (size_t)readBytes, offset, &fileInfo);
            offset += size;
            if (ret < 0) {
                success = false;
                break;
            }
        } while(readBytes != 0);
        REQUIRE(success);

        offset = 0;
        char otherBuffer[size];
        bool sameValue = true;
        do {
            readBytes = read(fd, buffer, size);
            ret = myfs->fuseRead((char*)"TestDatei", otherBuffer, size, offset, &fileInfo);
            offset += size;
            if(readBytes != ret || memcmp(buffer, otherBuffer, (size_t)readBytes) != 0) {
                sameValue = false;
                break;
            }
        } while(readBytes != 0);

        close(fd);
        myfs->fuseRelease((char*)"TestDatei", &fileInfo);

        REQUIRE(sameValue);
    }

    SECTION("Write file and compare files, chunks of 512 byte") {
        REQUIRE(myfs->fuseUnlink("TestDatei") == 0);
        REQUIRE(myfs->fuseMknod("TestDatei", 0666, 0) == 0);

        int fd = open((char*)"TestDatei", O_RDWR);
        REQUIRE(fd >= 0);
        fileInfo.flags = O_RDWR;
        REQUIRE(myfs->fuseOpen((char*)"TestDatei", &fileInfo) == 0);

        size_t size = 512;
        off_t offset = 0;
        char buffer[size];

        ssize_t readBytes = 0;
        int ret = 0;
        bool success = true;
        do {
            readBytes = read(fd, buffer, size);
            if (readBytes < 0) {
                success = false;
                break;
            }
            ret = myfs->fuseWrite((char*)"TestDatei", buffer, (size_t)readBytes, offset, &fileInfo);
            offset += size;
            if (ret < 0) {
                success = false;
                break;
            }
        } while(readBytes != 0);
        REQUIRE(success);

        offset = 0;
        char otherBuffer[size];
        bool sameValue = true;
        do {
            readBytes = read(fd, buffer, size);
            ret = myfs->fuseRead((char*)"TestDatei", otherBuffer, size, offset, &fileInfo);
            offset += size;
            if(readBytes != ret || memcmp(buffer, otherBuffer, (size_t)readBytes) != 0) {
                sameValue = false;
                break;
            }
        } while(readBytes != 0);

        close(fd);
        myfs->fuseRelease((char*)"TestDatei", &fileInfo);

        REQUIRE(sameValue);
    }

    SECTION("Write file and compare files, chunks of 8192 byte") {
        REQUIRE(myfs->fuseUnlink("TestDatei") == 0);
        REQUIRE(myfs->fuseMknod("TestDatei", 0666, 0) == 0);

        int fd = open((char*)"TestDatei", O_RDWR);
        REQUIRE(fd >= 0);
        fileInfo.flags = O_RDWR;
        REQUIRE(myfs->fuseOpen((char*)"TestDatei", &fileInfo) == 0);

        size_t size = 8192;
        off_t offset = 0;
        char buffer[size];

        ssize_t readBytes = 0;
        int ret = 0;
        bool success = true;
        do {
            readBytes = read(fd, buffer, size);
            if (readBytes < 0) {
                success = false;
                break;
            }
            ret = myfs->fuseWrite((char*)"TestDatei", buffer, (size_t)readBytes, offset, &fileInfo);
            offset += size;
            if (ret < 0) {
                success = false;
                break;
            }
        } while(readBytes != 0);
        REQUIRE(success);

        offset = 0;
        char otherBuffer[size];
        bool sameValue = true;
        do {
            readBytes = read(fd, buffer, size);
            ret = myfs->fuseRead((char*)"TestDatei", otherBuffer, size, offset, &fileInfo);
            offset += size;
            if(readBytes != ret || memcmp(buffer, otherBuffer, (size_t)readBytes) != 0) {
                sameValue = false;
                break;
            }
        } while(readBytes != 0);

        close(fd);
        myfs->fuseRelease((char*)"TestDatei", &fileInfo);

        REQUIRE(sameValue);
    }

    std::cout << "Tests with large files completed!" << std::endl;

    delete myfs;
    remove((char*)"TestDatei");
    remove((char*) TEST_FILESYSTEM);
}

TEST_CASE("MyFS.getAttr, Existing File", "[MyFS]"){
    system("touch Test_File_1.txt");
    system("chmod 0444 Test_File_1.txt");
    system("touch Test_File_2.txt");
    system("chmod 0666 Test_File_2.txt");
    system("touch Test_File_3.txt");
    system("chmod 0777 Test_File_3.txt");
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " Test_File_1.txt Test_File_2.txt Test_File_3.txt");
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);
    struct stat buf;

    SECTION("File has All Read Permissions") {
        buf = {};
        REQUIRE(myfs->fuseGetattr("Test_File_1.txt", &buf) == 0);
        REQUIRE(buf.st_mode == (0444 | S_IFREG));
    }
    SECTION("Files has All Write Permissions") {
        buf = {};
        REQUIRE(myfs->fuseGetattr("Test_File_2.txt", &buf) == 0);
        REQUIRE(buf.st_mode == (0666 | S_IFREG));
    }
    SECTION("Files has All Permissions") {
        buf = {};
        REQUIRE(myfs->fuseGetattr("Test_File_3.txt", &buf) == 0);
        REQUIRE(buf.st_mode == (0777 | S_IFREG));
    }

    SECTION("Dir Permissions") {
        buf = {};
        REQUIRE(myfs->fuseGetattr("/",&buf) == 0);
        REQUIRE(buf.st_mode == (0775 | S_IFDIR));
    }

    remove("Test_File_1.txt");
    remove("Test_File_2.txt");
    remove("Test_File_3.txt");
    remove(TEST_FILESYSTEM);
}

TEST_CASE("MyFS.getAttr, Error codes", "[MyFS]") {
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    fuse_file_info fileInfo = {};
    struct stat stats;

    SECTION("Existing File") {
        REQUIRE(myfs->fuseGetattr(TEST_FILE, &stats) == 0);
    }

    SECTION("Nonexistent File") {
        REQUIRE(myfs->fuseGetattr(NONEXISTENT_FILE, &stats) == -ENOENT);
    }

    SECTION("Directory") {
        REQUIRE(myfs->fuseGetattr("/", &stats) == 0);
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}

TEST_CASE("MyFS.Mknod, MyFS.Unlink", "[MyFS]") {
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    fuse_file_info fileInfo = {};
    struct stat stats;

    SECTION("Create file") {
        myfs->fuseMknod((char*) "NeueDatei", 0777, 0);
        REQUIRE(myfs->fuseGetattr("NeueDatei", &stats) == 0);
    }

    SECTION("Create already existing file") {
        int ret = myfs->fuseMknod((char*) TEST_FILE, 0777, 0);
        REQUIRE(ret == -EEXIST);
    }

    SECTION("Create same file twice") {
        REQUIRE(myfs->fuseMknod((char*) "NeueDatei", 0777, 0) == 0);
        REQUIRE(myfs->fuseGetattr("NeueDatei", &stats) == 0);
        int ret = myfs->fuseMknod((char*) "NeueDatei", 0777, 0);
        REQUIRE(ret == -EEXIST);
    }

    SECTION("Delete existing file") {
        REQUIRE(myfs->fuseUnlink((char*) TEST_FILE) == 0);
        REQUIRE(myfs->fuseGetattr(TEST_FILE, &stats) == -ENOENT);
    }

    SECTION("Delete nonexistent file") {
        REQUIRE(myfs->fuseUnlink((char*) NONEXISTENT_FILE) == -ENOENT);
    }

    SECTION("Create and delete file") {
        REQUIRE(myfs->fuseGetattr("NeueDatei", &stats) == -ENOENT);
        REQUIRE(myfs->fuseMknod((char*) "NeueDatei", 0777, 0) == 0);
        REQUIRE(myfs->fuseGetattr("NeueDatei", &stats) == 0);
        REQUIRE(myfs->fuseUnlink((char*)"NeueDatei") == 0);
        REQUIRE(myfs->fuseGetattr("NeueDatei", &stats) == -ENOENT);
    }


    delete myfs;
    remove((char*) TEST_FILESYSTEM);
}

TEST_CASE("Lots of files" , "[MyFS]") {
    MyFS* myfs = new MyFS();
    system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
    myfs->initializeFilesystem((char*) TEST_FILESYSTEM);

    fuse_file_info fileInfo = {};

    SECTION("Creating 64 files") {
        // TODO: Implement test. This is a template for tests.
        REQUIRE(0 == 0); // TODO: add assertions
    }

    SECTION("Creating 65 files") {
        // TODO: Implement test. This is a template for tests.
        REQUIRE(0 == 0); // TODO: add assertions
    }

    SECTION("Creating 63 files") {
        // TODO: Implement test. This is a template for tests.
        REQUIRE(0 == 0); // TODO: add assertions
    }

    SECTION("Creating 12 files in addition, to big") {
        // TODO: Implement test. This is a template for tests.
        REQUIRE(0 == 0); // TODO: add assertions
    }

    SECTION("Switching files, all ones would be too big") {
        // TODO: Implement test. This is a template for tests.
        REQUIRE(0 == 0); // TODO: add assertions
    }

    delete myfs;
    remove((char*) TEST_FILESYSTEM);

}
