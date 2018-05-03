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

#define SUPER_START 0
#define DMAP_START 1
#define FAT_START 17
#define ROOT_START 273
#define DATA_START 337

#define SUPER_SIZE 1
#define DMAP_SIZE = FAT_START - DMAP_START
#define FAT_SIZE = ROOT_START - FAT_START
#define ROOT_SIZE = DATA_START - ROOT_START
#define DATA_SIZE 65536


int main(int argc, char *argv[]) {

    // TODO: Implement file system generation & copying of files here
    return 0;
}
