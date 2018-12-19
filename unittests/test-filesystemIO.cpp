//
//  test-myfs.cpp
//  testing
//
//  Created by Oliver Waldhorst on 15.12.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "catch.hpp"

#include <string.h>
#include <iostream>

#include "helper.hpp"
#include "constants.h"
#include "filesystemIO.h"
#include "fsIO-new.h"

#define BD_PATH "Test_Blockdevice.bin"

typedef struct {
    char name[NAME_LENGTH];
    off_t size;
    uid_t userID;
    gid_t groupID;
    time_t last_time;
    time_t modi_time;
    time_t change_time;
    uint16_t  first_block;
} fileStats;

typedef struct {
    uint64_t one;
    uint64_t two;
    uint64_t three;
    uint64_t four;
    uint64_t five;
    uint64_t six;
    uint64_t seven;
    uint64_t eight;
    uint64_t nine;
    uint64_t ten;
    uint64_t eleven;
} testNumbers;

typedef struct {
    testNumbers before;
    testNumbers first;
    testNumbers second;
    testNumbers third;
    testNumbers fourth;
    testNumbers fifth;
    testNumbers after;
} testData;

TEST_CASE( "filsystemIO.Read/Write struct, on stack", "[filsystemIO]" ) {
    remove(BD_PATH);
    BlockDevice blockdevice = BlockDevice();
    blockdevice.create(BD_PATH);
    NewFilesystemIO fsIO = NewFilesystemIO(blockdevice);
    
    SECTION("struct < BLOCK_SIZE") {
        fileStats input;
        fileStats output;
        input.size = 1024;
        
        fsIO.writeDevice(15, &input, sizeof(input));
        fsIO.readDevice(15, &output, sizeof(output));
        
        REQUIRE(memcmp(&input, &output, sizeof(fileStats)) == 0);
    }
    
    SECTION("struct > BLOCK_SIZE") {
        testData input;
        testData output;
        input.before = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        input.first = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        input.second = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
        input.third = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
        input.fourth = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
        input.fifth = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
        input.after = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        
        fsIO.writeDevice(2, &input, sizeof(input));
        fsIO.readDevice(2, &output, sizeof(output));
        
        REQUIRE(memcmp(&input, &output, sizeof(testData)) == 0);
    }
    
    remove(BD_PATH);
}

TEST_CASE( "filsystemIO.Read/Write array, on stack", "[filsystemIO]" ) {
    remove(BD_PATH);
    BlockDevice blockdevice = BlockDevice();
    blockdevice.create(BD_PATH);
    NewFilesystemIO fsIO = NewFilesystemIO(blockdevice);
    
    SECTION("array < BLOCK_SIZE") {
        uint16_t input[10];
        uint16_t output[10];
        for(int i = 0; i < 10; i++) {
            input[i] = i * i;
        }
        
        fsIO.writeDevice(21, &input, sizeof(input));
        fsIO.readDevice(21, &output, sizeof(output));
        
        REQUIRE(memcmp(input, output, sizeof(input)) == 0);
    }
    
    SECTION("array > BLOCK_SIZE") {
        uint64_t input[201];
        uint64_t output[201];
        for(int i = 0; i < 201; i++) {
            input[i] = i + 1;
        }
        
        fsIO.writeDevice(5, &input, sizeof(input));
        fsIO.readDevice(5, &output, sizeof(output));
        
        REQUIRE(memcmp(input, output, sizeof(input)) == 0);
    }
    
    remove(BD_PATH);
}

TEST_CASE( "filsystemIO.Read/Write struct, on heap", "[filsystemIO]" ) {
    remove(BD_PATH);
    BlockDevice blockdevice = BlockDevice();
    blockdevice.create(BD_PATH);
    NewFilesystemIO fsIO = NewFilesystemIO(blockdevice);
    
    SECTION("struct < BLOCK_SIZE") {
        fileStats* input = new fileStats;
        fileStats* output = new fileStats;
        input->size = 1024;
        
        fsIO.writeDevice(15, input, sizeof(*input));
        fsIO.readDevice(15, output, sizeof(*output));
        
        REQUIRE(memcmp(input, output, sizeof(fileStats)) == 0);
        delete input;
        delete output;
    }
    
    SECTION("struct > BLOCK_SIZE") {
        testData* putput = new testData;
        testData* output = new testData;
        putput->before = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        putput->first = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        putput->second = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
        putput->third = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
        putput->fourth = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
        putput->fifth = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
        putput->after = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        
        fsIO.writeDevice(2, putput, sizeof(*putput));
        fsIO.readDevice(2, output, sizeof(*output));
        
        REQUIRE(memcmp(putput, output, sizeof(testData)) == 0);
        delete putput;
        delete output;
    }
    
    remove(BD_PATH);
}

TEST_CASE( "filsystemIO.Read/Write array, on heap", "[filsystemIO]" ) {
    remove(BD_PATH);
    BlockDevice blockdevice = BlockDevice();
    blockdevice.create(BD_PATH);
    NewFilesystemIO fsIO = NewFilesystemIO(blockdevice);
    
    SECTION("array < BLOCK_SIZE") {
        uint16_t arraySize = 10;
        uint16_t* input = new uint16_t[arraySize];
        uint16_t* output = new uint16_t[arraySize];
        for(int i = 0; i < arraySize; i++) {
            input[i] = i * i;
        }
        
        fsIO.writeDevice(21, input, sizeof(*input) * arraySize);
        fsIO.readDevice(21, output, sizeof(*output) * arraySize);
        
        REQUIRE(memcmp(input, output, sizeof(*input)) == 0);
        delete[] input;
        delete[] output;
    }
    
    SECTION("array > BLOCK_SIZE") {
        uint16_t arraySize = 201;
        uint16_t* input = new uint16_t[arraySize];
        uint16_t* output = new uint16_t[arraySize];
        for(int i = 0; i < arraySize; i++) {
            input[i] = i + 1;
        }
        
        fsIO.writeDevice(5, input, sizeof(*input));
        fsIO.readDevice(5, output, sizeof(*output));
        
        REQUIRE(memcmp(input, output, sizeof(*input)) == 0);
        delete[] input;
        delete[] output;
    }
    
    remove(BD_PATH);
}

