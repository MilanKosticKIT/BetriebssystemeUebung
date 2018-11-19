//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <iostream>
#include <string.h>
#include <root.h>

#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include "constants.h"

#include "dmap.h"
#include "fat.h"
#include "root.h"
#include "filesystemIO.h"

//MARK: - Methodenheader

//MARK: -

BlockDevice blockDevice = BlockDevice();
FilesystemIO fsIO = FilesystemIO(blockDevice);
FAT fat = FAT();
DMap dmap = DMap();
Root root = Root();
struct SuperBlock superblock;

int main(int argc, char *argv[]) {

    if (argc > 1) {
        blockDevice.create(argv[1]);
        //blockDevice.create("./Blockdevice.bin");

        // write empty filesystem
        uint16_t fatArray[DATA_BLOCKS];
        uint8_t dMapArray[DATA_BLOCKS / 8];
        fileStats rootArray[DATA_BLOCKS];

        fat.getAll((char *) fatArray);
        dmap.getAll((char *) dMapArray);
        root.getAll(rootArray);

        fsIO.writeDevice(SUPERBLOCK_START, superblock);
        fsIO.writeDevice(DMAP_START, dMapArray);
        fsIO.writeDevice(FAT_START, fatArray);
        fsIO.writeDevice(ROOT_START, rootArray);

        if (argc > 2) {
            for (int i = 2; i < argc; i++) { // copy files
                char *filename = argv[i];
                int fileDescriptor = open(filename, O_RDONLY);
                if (fileDescriptor >= 0) {
                    ssize_t retRead;
                    char buffer[BLOCK_SIZE];

                    uint16_t currentBlock;
                    dmap.getFreeBlock(&currentBlock);
                    uint16_t lastBlock = currentBlock;
                    root.createEntry(filename); // "create file"
                    fileStats stats;
                    root.get(filename, &stats);
                    stats.first_block = currentBlock;

                    retRead = read(fileDescriptor, buffer, BLOCK_SIZE);
                    blockDevice.write(DATA_START + currentBlock, buffer); // write data
                    dmap.set(currentBlock);

                    while (retRead > 0) {
                        dmap.getFreeBlock(&currentBlock);

                        retRead = read(fileDescriptor, buffer, BLOCK_SIZE);
                        blockDevice.write(DATA_START + currentBlock, buffer);
                        dmap.set(currentBlock);
                        fat.addToFAT(lastBlock, currentBlock);
                        lastBlock = currentBlock;
                        if (retRead > 0) {
                            stats.size += retRead;
                        }
                    }
                    fat.addLastToFAT(lastBlock);
                    superblock.emptySpaceSize -= stats.size; //todo
                }
                close(fileDescriptor);
                //int retClose = close(fileDescriptor);
            }
        }
    } else {
        // error: name of containerfile missing
    }


    /*
    fileStts foobar;
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

    return 0;
}


//MARK: - Our Methods
