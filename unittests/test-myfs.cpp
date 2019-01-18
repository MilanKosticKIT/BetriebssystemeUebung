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
    SECTION("Beschreibung") {
        MyFS* myfs = new MyFS();
        system("./mkfs.myfs " TEST_FILESYSTEM " Makefile");
        myfs->initializeFilesystem(TEST_FILESYSTEM);

    }

    // TODO: Implement test. This is a template for tests.
}

TEST_CASE("MyFS.open", "[MyFS]") {
    SECTION("Funktioniert das so?") {
#define TEST_FILE "Makefile"
        MyFS *myfs = new MyFS();
        system("./mkfs.myfs " TEST_FILESYSTEM " " TEST_FILE);
        myfs->initializeFilesystem(TEST_FILESYSTEM);

        Root root = myfs->getRoot();
        for (int i = 0; i < 5; i++) {
            if (root.exists(i)) {
                char* name;
                root.getName(i, &name);
                std::cout << "Eintrag " << i << " Name: " << name << std::endl;
            } else {
                std::cout << "Eintrag " << i << " leer" << std::endl;
            }
        }

        fuse_file_info fileInfo = {};
        fileInfo.flags = O_RDONLY;
        int ret = myfs->fuseOpen(TEST_FILE, &fileInfo);
        REQUIRE(ret == 0);
    }
}
