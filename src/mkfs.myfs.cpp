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
#include "log.h"

#include "dmap.h"
#include "fat.h"
#include "root.h"
#include "filesystemIO.h"

//MARK: - Methodenheader

//MARK: -

int main(int argc, char *argv[]) {
    BlockDevice* blockDevice = new BlockDevice();
    FilesystemIO fsIO = FilesystemIO(blockDevice);
    FAT fat = FAT();
    DMap dmap = DMap();
    Root root = Root();
    SuperBlock superblock = {};

    if (argc > 1) {
        int ret = 0; //return value for error handling

        std::cout << "Creating Blockdevice: " << argv[1] << std::endl;
        remove(argv[1]);
        blockDevice->create(argv[1]);

        // write empty filesystem
        uint16_t* fatArray = new uint16_t[DATA_BLOCKS];
        uint8_t* dMapArray = new uint8_t[(DATA_BLOCKS + 1) / 8];
        fileStats* rootArray = new fileStats[ROOT_ARRAY_SIZE];

        fat.getAll(fatArray);
        dmap.getAll(dMapArray);
        root.getAll(rootArray);
        std::cout << "Initializing Filesystem" << std::endl;

        ret = fsIO.writeDevice(SUPERBLOCK_START, &superblock, sizeof(superblock));
        if (ret < 0) {
            std::cout << "Error at blockdevice.write() (writing superblock):" << ret << " (description: " << errno << ")" << std::endl;
        }
        ret = fsIO.writeDevice(DMAP_START, dMapArray, sizeof(*dMapArray) * (DATA_BLOCKS + 1) / 8);
        if (ret < 0) {
            std::cout << "Error at blockdevice.write() (writing dmap):" << ret << " (description: " << errno << ")" << std::endl;
        }
        ret = fsIO.writeDevice(FAT_START, fatArray, sizeof(*fatArray) * DATA_BLOCKS);
        if (ret < 0) {
            std::cout << "Error at blockdevice.write() (writing fat):" << ret << " (description: " << errno << ")" << std::endl;
        }
        ret = fsIO.writeDevice(ROOT_START, rootArray, sizeof(*rootArray) * ROOT_ARRAY_SIZE);
        if (ret < 0) {
            std::cout << "Error at blockdevice.write() (writing root):" << ret << " (description: " << errno << ")" << std::endl;
        }

        if (argc > 2) {
            //When the files to copy are too large for our filesystem
            //the operation will be aborted
            std::cout << "\nChecking if files fit in filesystem: ";
            int freeSpace = DATA_BYTES;
            struct stat buffer1;
            bool sizeOK = true;
            for (int i = 1; i < argc; i++) {
                stat(argv[i], &buffer1);
                freeSpace = freeSpace - (int)buffer1.st_size;
                if (freeSpace < 0) {
                    sizeOK = false;
                    break;
                }
            }
            if (sizeOK) {   //The files fit in the filesystem
                std::cout << "The files fit in the filesystem.\n" << std::endl;
                for (int i = 2; i < argc; i++) { // copy files
                    char *filename = argv[i];
                    std::cout << "Copying file: " << argv[i] << std::endl;
                    int fileDescriptor = open(filename, O_RDONLY);
                    if (fileDescriptor < 0) {
                        std::cout << "errno: " << errno << std::endl;
                        return errno;
                    } else {
                        ssize_t retRead = 1;
                        char buffer[BLOCK_SIZE];

                        ret = root.createEntry(filename, 0444); // "create file"
                        if (ret < 0) {
                            std::cout << "Root.createEntry errno: " << errno << std::endl;
                            return errno;
                        }
                        fileStats stats;
                        ret = root.get(filename, &stats); // get file stats
                        if (ret < 0) {
                            std::cout << "Root.get errno: " << errno << std::endl;
                            return errno;
                        }
                        uint16_t currentBlock;
                        uint16_t nextBlock;
                        ret = dmap.getFreeBlock(&nextBlock);
                        if (ret < 0) {
                            std::cout << "DMap.getFreeBlock errno: " << errno << std::endl;
                            return errno;
                        }
                        stats.first_block = nextBlock;
                        currentBlock = nextBlock;

                        std::cout << "Writing blocks ";
                        while (retRead > 0) {
                            currentBlock = nextBlock;

                            retRead = read(fileDescriptor, buffer, BLOCK_SIZE);
                            std::cout << currentBlock << ",";
                            ret = blockDevice->write(DATA_START + currentBlock, buffer);
                            if (ret < 0) {
                                std::cout << "BlockDevice.write errno: " << errno << std::endl;
                                return errno;
                            }
                            dmap.set(currentBlock);
                            stats.size += retRead;
                            ret = dmap.getFreeBlock(&nextBlock);
                            if (ret < 0) {
                                std::cout << "DMap.getFreeBlock errno: " << errno << std::endl;
                                return errno;
                            }
                            ret = fat.addNextToFAT(currentBlock, nextBlock);
                            if (ret < 0) {
                                std::cout << "FAT.addToFAT errno: " << errno << std::endl;
                                return errno;
                            }
                        }
                        std::cout << std::endl;
                        fat.addLastToFAT(currentBlock);
                        ret = root.update(stats);
                        if (ret < 0) {
                            std::cout << "Root.update errno: " << errno << std::endl;
                            return errno;
                        }
                        superblock.emptySpaceSize -= stats.size;

                        close(fileDescriptor);
                        std::cout << "Finished writing file " << filename << std::endl;

                    }

                }
                std::cout << std::endl;

                std::cout << "Closing blockdevice: ";
                ret = blockDevice->close();
                if (ret < 0) {
                    std::cout << "failed" << std::endl;
                    std::cout << "Error at blockdevice.close():" << ret << " (description: " << errno << ")" << std::endl;
                } else {
                    std::cout << "successful!" << std::endl;
                }

                std::cout << "Opening blockdevice: ";
                ret = blockDevice->open(argv[1]);
                if (ret < 0) {
                    std::cout << "failed" << std::endl;
                    std::cout << "Error at blockdevice.open(" << argv[1] << "):" << ret << " (description: " << errno << ")" << std::endl;
                } else {
                    std::cout << "successful!" << std::endl;
                }

                std::cout << "\nWriting Filesystem data" << std::endl;

                dmap.getAll(dMapArray);
                fat.getAll(fatArray);
                root.getAll(rootArray);

                std::cout << "--------------------------------------------------------------------------------" << std::endl;
                std::cout << "First contents of data structures:" << std::endl;
                std::cout << "dmap:" << std::endl;
                for(int i = 0; i < 10; i++) {
                    std::cout << (int)dMapArray[i] << " ";
                }
                std::cout << std::endl;
                std::cout << "fat:" << std::endl;
                for(int i = 0; i < 10; i++) {
                    std::cout << fatArray[i] << " ";
                }
                std::cout << std::endl;
                std::cout << "root (size):" << std::endl;
                for(int i = 0; i < 10; i++) {
                    std::cout << rootArray[i].size << " ";
                }
                std::cout << std::endl;

                ret = fsIO.writeDevice(SUPERBLOCK_START, &superblock, sizeof(superblock));
                if (ret < 0) {
                    std::cout << "Error at blockdevice.write() (writing superblock):" << ret << " (description: " << errno << ")" << std::endl;
                }
                ret = fsIO.writeDevice(DMAP_START, dMapArray, sizeof(*dMapArray) * (DATA_BLOCKS + 1) / 8);
                if (ret < 0) {
                    std::cout << "Error at blockdevice.write() (writing dmap):" << ret << " (description: " << errno << ")" << std::endl;
                }
                ret = fsIO.writeDevice(FAT_START, fatArray, sizeof(*fatArray) * DATA_BLOCKS);
                if (ret < 0) {
                    std::cout << "Error at blockdevice.write() (writing fat):" << ret << " (description: " << errno << ")" << std::endl;
                }
                ret = fsIO.writeDevice(ROOT_START, rootArray, sizeof(*rootArray) * ROOT_ARRAY_SIZE);
                if (ret < 0) {
                    std::cout << "Error at blockdevice.write() (writing root):" << ret << " (description: " << errno << ")" << std::endl;
                }

                for (int i = 0; i < (DATA_BLOCKS + 1) / 8; i++) {
                    dMapArray[i] = 0;
                }
                for (int i = 0; i < DATA_BLOCKS; i++) {
                    fatArray[i] = 0;
                }
                for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
                    rootArray[i] = fileStats {};
                }

                std::cout << "--------------------------------------------------------------------------------" << std::endl;
                std::cout << "Reading out first contents of data structures from blockDevice:" << std::endl;

                ret = fsIO.readDevice(SUPERBLOCK_START, &superblock, sizeof(superblock));
                if (ret < 0) {
                    std::cout << "Error at blockdevice.read() (Reading superblock):" << ret << " (description: " << errno << ")" << std::endl;
                }
                ret = fsIO.readDevice(DMAP_START, dMapArray, sizeof(*dMapArray) * (DATA_BLOCKS + 1) / 8);
                if (ret < 0) {
                    std::cout << "Error at blockdevice.read() (Reading dmap):" << ret << " (description: " << errno << ")" << std::endl;
                }
                ret = fsIO.readDevice(FAT_START, fatArray, sizeof(*fatArray) * DATA_BLOCKS);
                if (ret < 0) {
                    std::cout << "Error at blockdevice.read() (Reading fat):" << ret << " (description: " << errno << ")" << std::endl;
                }
                ret = fsIO.readDevice(ROOT_START, rootArray, sizeof(*rootArray) * ROOT_ARRAY_SIZE);
                if (ret < 0) {
                    std::cout << "Error at blockdevice.read() (Reading root):" << ret << " (description: " << errno << ")" << std::endl;
                }
                std::cout << "dmap:" << std::endl;
                for(int i = 0; i < 10; i++) {
                    std::cout << (int)dMapArray[i] << " ";
                }
                std::cout << std::endl;
                std::cout << "fat:" << std::endl;
                for(int i = 0; i < 10; i++) {
                    std::cout << fatArray[i] << " ";
                }
                std::cout << std::endl;
                std::cout << "root (size):" << std::endl;
                for(int i = 0; i < 10; i++) {
                    std::cout << rootArray[i].size << " ";
                }
                std::cout << std::endl;

                delete[] dMapArray;
                delete[] fatArray;
                delete[] rootArray;

            } else {
                std::cout << "The files to copy are to big for the filesystem. Aborting" << std::endl;
                return errno;
            }
        }

        blockDevice->close();
    } else {
        std::cout << "Invalid Arguments: Name of Containerfile missing!" << std::endl;
        return errno = 666;
    }
    return 0;
}


//MARK: - Our Methods
