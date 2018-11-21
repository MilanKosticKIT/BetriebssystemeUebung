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

        uint16_t fatArray[DATA_BLOCKS];
        for (int i = 0; i < DATA_BLOCKS; i++) {
            fatArray[i] = 0;
        }
        uint8_t dMapArray[DATA_BLOCKS / 8];
        for (int i = 0; i < DATA_BLOCKS; i++) {
            dMapArray[i] = 0;
        }
        fileStats rootArray[DATA_BLOCKS];
        fileStats stats = {};
        for (int i = 0; i < DATA_BLOCKS; i++) {
            rootArray[i] = stats;
        }

        fat.setAll((char *) fatArray);
        dmap.setAll((char *) dMapArray);
        root.setAll(rootArray);

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
                }
                int retClose = close(fileDescriptor);
            }
        }
    } else {
        // error: name of containerfile missing
    }

    return 0;
}


//MARK: - Our Methods
