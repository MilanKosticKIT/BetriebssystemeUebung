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

#include "myfs.h"
#include "myfs-structs.h"
#include "blockdevice.h"
#include "macros.h"
#include "constants.h"

#include "dmap.h"
#include "fat.h"
#include "root.h"
#include "filesystemIO.h"
#include "log.h"

//MARK: - Methodenheader

//MARK: -

int main(int argc, char *argv[]) {
    BlockDevice blockDevice = BlockDevice();
    FilesystemIO fsIO = FilesystemIO(blockDevice);
    FAT fat = FAT();
    DMap dmap = DMap();
    Root root = Root();
    SuperBlock superblock = {};

    if (argc > 1) {
        std::cout << "Creating Blockdevice: " << argv[1] << std::endl;
        remove(argv[1]);
        blockDevice.create(argv[1]);
        //blockDevice.create("./Blockdevice.bin");

        // write empty filesystem
        uint16_t fatArray[DATA_BLOCKS];
        uint8_t dMapArray[(DATA_BLOCKS + 1) / 8];
        fileStats rootArray[ROOT_ARRAY_SIZE];

        std::cout << "Creating Filesystem" << std::endl;
        fat.getAll(fatArray);
        dmap.getAll(dMapArray);
        root.getAll(rootArray);
        std::cout << "Initializing Filesystem" << std::endl;
        fsIO.writeDevice(SUPERBLOCK_START, superblock);
        fsIO.writeDevice(DMAP_START, dMapArray);
        fsIO.writeDevice(FAT_START, fatArray);
        fsIO.writeDevice(ROOT_START, rootArray);

        if (argc > 2) {
            //When the files to copy are too large for our filesystem
            //the operation will be aborted
            std::cout << "Checking if files fit in filesystem: ";
            int freeSpace = DATA_BYTES;
            struct stat buffer1;
            log(argc);
            log(argc);
            log(argc);
            bool sizeOK = true;
            for (int i = 1; i < argc; i++) {
                stat(argv[i], &buffer1);
                freeSpace = freeSpace - buffer1.st_size;
                if (freeSpace < 0) {
                    sizeOK = false;
                    break;
                }
            }
            if (sizeOK) {   //The files fit in the filesystem
                std::cout << "The files fit in the filesystem." << std::endl;
            for (int i = 2; i < argc; i++) { // copy files
                char *filename = argv[i];
                std::cout << "Copying file: " << argv[i] << std::endl;
                int fileDescriptor = open(filename, O_RDONLY);
                if (fileDescriptor < 0) {
                    std::cout << "errno: " << errno << std::endl;
                } else {
                    ssize_t retRead = 1;
                    char buffer[BLOCK_SIZE];
                    int ret = 0;

                    ret = root.createEntry(filename); // "create file"
                    if (ret < 0) {
                        std::cout << "Root.createEntry errno: " << errno << std::endl;
                    }
                    fileStats stats;
                    ret = root.get(filename, &stats); // get file stats
                    if (ret < 0) {
                        std::cout << "Root.get errno: " << errno << std::endl;
                    }
                    uint16_t currentBlock;
                    uint16_t nextBlock;
                    ret = dmap.getFreeBlock(&currentBlock);
                    if (ret < 0) {
                        std::cout << "DMap.getFreeBlock errno: " << errno << std::endl;
                    }
                    stats.first_block = currentBlock;



                    while (retRead > 0) {
                        retRead = read(fileDescriptor, buffer, BLOCK_SIZE);
                        std::cout << currentBlock << ",";
                        ret = blockDevice.write(DATA_START + currentBlock, buffer);
                        if (ret < 0) {
                            std::cout << "BlockDevice.write errno: " << errno << std::endl;
                        }
                        dmap.set(currentBlock);
                        stats.size += retRead;
                        ret = dmap.getFreeBlock(&nextBlock);
                        if (ret < 0) {
                            std::cout << "DMap.getFreeBlock errno: " << errno << std::endl;
                        }
                        ret = fat.addNextToFAT(currentBlock, nextBlock);
                        if (ret < 0) {
                            std::cout << "FAT.addToFAT errno: " << errno << std::endl;
                        }
                        currentBlock = nextBlock;


                    }
                    std::cout << std::endl;
                    fat.addLastToFAT(currentBlock);
                    ret = root.update(stats);
                    if (ret < 0) {
                        std::cout << "Root.update errno: " << errno << std::endl;
                    }
                    superblock.emptySpaceSize -= stats.size;

                    close(fileDescriptor);
                    std::cout << "Finished writing file " << filename << std::endl;
                }

            }
            std::cout << "Writing Filesystem data" << std::endl;
            dmap.getAll(dMapArray);
            fat.getAll(fatArray);
            root.getAll(rootArray);
            fsIO.writeDevice(SUPERBLOCK_START, superblock);
            fsIO.writeDevice(DMAP_START, dMapArray);
            fsIO.writeDevice(FAT_START, fatArray);
            fsIO.writeDevice(ROOT_START, rootArray);
            }else {
                std::cout << "The files to copy are to big for the filesystem. Aborting" << std::endl;
            }
            
        }
    } else {
        // error: name of containerfile missing
    }
    return errno;
}


//MARK: - Our Methods
