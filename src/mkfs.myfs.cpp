//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <type_traits>
#include <stdint.h>
#include "constants.h"

typedef struct {
    char name[NAME_LENGTH];
    off_t size;
    uid_t userID;
    gid_t groupID;
    time_t last_time;
    time_t modi_time;
    time_t change_time;
} fileStats;

//MARK: - Methodenheader
//DMAP
//TODO: Root?
MetaData getMetaData(u_int8_t indexOfFile);

void setMetaData(MetaData metaData, u_int8_t indexOfFile);

void convertBlockToMetaData(MetaData *data, char *block);

void convertMetaDataToBlock(MetaData *data, char *block);

//TODO: Get these lines to a fitting header file.
//MARK: -

BlockDevice blockDevice = BlockDevice();


bool getStats(fileStats *status, char *path, blkcnt_t *blockCount) {
    struct stat sb;
    stat(path, &sb);
    char *filename = basename(path);
    if (strlen(filename) > NAME_LENGTH) {
        return false;
    }
    strcpy(status->name, filename);//TODO: Pointer?!?!
    status->size = sb.st_size;
    status->userID = geteuid();
    status->groupID = getegid();
    status->modi_time = sb.st_mtime;
    time(&(status->last_time));
    time(&(status->change_time));
    *blockCount = sb.st_blocks;
    return true;
}

void updateRoot(fileStats *status) {
    fileStats root[NUM_DIR_ENTRIES];


}


int main(int argc, char *argv[]) {

    blockDevice.open("./Blockdevice.bin");

    /*
    fileStats foobar;
    foobar.size = 1024;
    writeDevice(15, foobar);
    
    fileStats bar;
    readDevice(15, &bar);
    
    std::cout << bar.size << std::endl;
     */

    /* Test for read/writeDevice for one block with char (1 byte)
    char input[6] = {'a', 'b', 'c', 'd', 'e', '\0'};
    char putput[BLOCK_SIZE];
    char output[6];
    char* outputString = (char*) output;
    */
    /* Test for read/writeDevice for one block with int (>1 byte)
    int input[6] = {0, 1, 2, 3, 4, 5};
    int output[6];
    
    writeDevice(10, input);
    readDevice(10, output);
    for(int i = 0; i< 6; i++){
        std::cout << input[i] << std::endl;
    }
    for(int i = 0; i< 6; i++){
        std::cout << output[i] << std::endl;
    }
    */

    /* Test for read/writeDevice for multiple blocks with struct
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

    testData input;
    testData output;
    input.before = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    input.first = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    input.second = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    input.third = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    input.fourth = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    input.fifth = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    input.after = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    writeDevice(2, input);
    readDevice(2, output);

    uint64_t size = sizeof(testData) / 8;
    uint64_t *outputArray;
    outputArray = reinterpret_cast<uint64_t *>(&output);
    std::cout << "Read size: " << sizeof(testData) << std::endl;
    for (uint64_t i = 0; i < size; i++) {
        std::cout << outputArray[i];
    }
    std::cout << std::endl;
    */
    /* Test for read/writeDevice with multiple blocks and array.
    uint64_t input[101];
    for (int i = 0; i < 101; i++) {
        input[i] = 0;
    }
    input[0] = 132456;
    input[2] = 11111111111111;
    input[54] = 321321654;
    input[67] = 987654321;
    input[91] = 999999999999;

    uint64_t output[101];
    writeDevice(5, input);
    readDevice(5, output);

    for (int i = 0; i < 101; i++) {
        if (output[i] == 0) {
            std::cout << output[i];
        } else {
            std::cout << '\n' << output[i] << '\n';
        }
    }
    std::cout << std::endl;
    */



    // TODO: Implement file system generation & copying of files here
    return 0;

}


//MARK: - Our Methods

//MARK: - DMAP

//MARK: - Root
//Returns the metaData of the file behind the index.
MetaData getMetaData(u_int8_t indexOfFile) {
    u_int32_t blockNo = ROOT_START + indexOfFile;
    char *block;
    blockDevice.read(blockNo, block);
    MetaData data;
    convertBlockToMetaData(&data, block);       //TODO: Pls check deferensation, (and spelling).
    return data;
}

//Sets the MetaData of an file, which is refrenced via the index.
void setMetaData(MetaData metaData, u_int8_t indexOfFile) {
    u_int32_t blockNo = ROOT_START + indexOfFile;
    char *block;
    convertBlockToMetaData(&metaData, block);   //TODO: Pls check deferensation, (and spelling).
    blockDevice.write(blockNo, block);
}

//Casts the given block to a given MetaData instance
void convertBlockToMetaData(MetaData *data, char *block) {
    //TODO: Implement this.
}

//Converts the given MetaData into a block, formatted as char*/String
void convertMetaDataToBlock(MetaData *data, char *block) {
    //TODO: Implement this.
}
