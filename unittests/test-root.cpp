//
//  test-root.cpp
//  unittests
//
//  Created by MK on 28.11.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#include "catch.hpp"

#include <string.h>
#include <iostream>

#include "myfs-structs.h"
#include "helper.hpp"
#include "constants.h"
#include "root.h"

TEST_CASE("Root.setAll / Root.getAll", "[Root]"){
    SECTION("Standard initialized Root"){
        Root root = Root();
        fileStats rootArray[ROOT_ARRAY_SIZE];
        root.getAll(rootArray);
        bool successfulInit = true;
        for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
            if (rootArray[i].size != -1) {
                successfulInit = false;
                break;
            }
        }
        REQUIRE(successfulInit);
    }

    SECTION("Random Content"){
        Root root = Root();
        fileStats rootArray[ROOT_ARRAY_SIZE];
        fileStats readArray[ROOT_ARRAY_SIZE];
        root.setAll(rootArray);
        root.getAll(readArray);
        REQUIRE(memcmp(rootArray, readArray, ROOT_ARRAY_SIZE) == 0);
    }
}

TEST_CASE("Root.createEntry", "[Root]"){
    SECTION("Created file exists"){
        Root root = Root();
        int ret = root.createEntry("TestEntry");
        REQUIRE(ret >= 0);
        fileStats entry;
        ret = root.get("TestEntry", &entry);
        REQUIRE(ret >= 0);
    }

    SECTION("Create same entry twice"){
        Root root = Root();
        int ret = root.createEntry("TestEntry");
        REQUIRE(ret >= 0);
        ret = root.createEntry("TestEntry");
        REQUIRE((ret < 0 && errno == EEXIST));
    }

    SECTION("Max number of files reached"){
        Root root = Root();
        char* name;

        int successfulCreat = true;
        for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
            name = (char*) malloc(i + 2);
            name[i + 1] = '\0';
            gen_random(name, i + 1);
            if (root.createEntry(name) < 0) {
                successfulCreat = false;
            }
            free(name);
        }
        REQUIRE(successfulCreat);

        name = (char*) malloc(ROOT_ARRAY_SIZE + 3);
        name[ROOT_ARRAY_SIZE + 2] = '\0';
        gen_random(name, ROOT_ARRAY_SIZE + 2);
        REQUIRE((root.createEntry(name) < 0 && errno == ENFILE));
        free(name);
    }

    SECTION("Max filename length reached") {
        Root root = Root();

        char* name = (char*) malloc(NAME_LENGTH);
        name[NAME_LENGTH - 1] = '\0';
        gen_random(name, NAME_LENGTH);
        REQUIRE((root.createEntry(name) >= 0));
        free(name);

        name = (char*) malloc(NAME_LENGTH + 1);
        name[NAME_LENGTH] = '\0';
        gen_random(name, NAME_LENGTH + 1);
        REQUIRE((root.createEntry(name) < 0 && errno == ENAMETOOLONG));
        free(name);
    }
}

TEST_CASE("Root.get", "[Root]") {
    // Get created file is tested in Root.createEntry

    SECTION("Get nonexisting file") {
        Root root = Root();
        fileStats stats;
        REQUIRE((root.get("TestEntry", &stats) < 0 && errno == ENOENT));
    }
}

TEST_CASE("Root.deleteEntry", "[Root]") {
    SECTION("Successful deletion") {
        Root root = Root();

        root.createEntry("TestEntry");
        REQUIRE(root.deleteEntry("TestEntry") >= 0); // deletion successful
        fileStats stats;
        REQUIRE(root.get("TestEntry", &stats) < 0); // entry not found
    }

    SECTION("Delete nonexisting file") {
        Root root = Root();
        REQUIRE((root.deleteEntry("TestEntry") < 0 && errno == ENOENT));
    }
}

TEST_CASE("Root.update", "[Root]") {
    SECTION("Successful update") {
        Root root = Root();
        fileStats stats;
        fileStats readStats;
        root.createEntry("TestEntry");
        root.get("TestEntry", &stats);

        // some changes
        stats.groupID = 132;
        time_t currentTime;
        time(&currentTime);
        stats.last_time = currentTime;

        REQUIRE(root.update(stats) >= 0); // update successful

        root.get("TestEntry", &readStats);
        REQUIRE(memcmp(&stats, &readStats, sizeof(stats)) == 0); // changed data was stored
    }

    SECTION("Update nonexisting file") {
        Root root = Root();

        fileStats stats = {};
        memcpy(stats.name, "TestEntry", 10);
        REQUIRE((root.update(stats) < 0 && errno == ENOENT));
    }
}
