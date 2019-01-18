//
//  test-fat.cpp
//  unittests
//
//  Created by MK on 28.11.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//

#include "catch.hpp"

#include <string.h>
#include <iostream>

#include "helper.hpp"
#include "constants.h"
#include "fat.h"



TEST_CASE("FAT.setALL / FAT.getAll", "[FAT]"){
    SECTION("Normally initalized FAT used"){
        FAT fat = FAT();
        uint16_t fatArray[DATA_BLOCKS];
        uint16_t readArray[DATA_BLOCKS];
        
        
        for(int i = 0; i < DATA_BLOCKS; i++){
            fatArray[i] = FAT_TERMINATOR;
        }
        
        fat.getAll(readArray);
        REQUIRE(memcmp(fatArray, readArray, sizeof(fatArray)) == 0);
    }
    SECTION("Manual to FAT_TERMINATOR set FAT"){
        FAT fat = FAT();
        uint16_t fatArray[DATA_BLOCKS];
        uint16_t readArray[DATA_BLOCKS];
        
        for(int i = 0; i < DATA_BLOCKS - 1; i++){
            fatArray[i] = FAT_TERMINATOR;
        }
        
        fat.setAll(fatArray);
        fat.getAll(readArray);
        REQUIRE(memcmp(fatArray, readArray, sizeof(fatArray)) == 0);
    }
    SECTION("Manually added various address, with terminator"){
        FAT fat = FAT();
        uint16_t fatArray[DATA_BLOCKS];
        uint16_t readArray[DATA_BLOCKS];
        
        for (int i = 0; i < DATA_BLOCKS; i++) {
            fatArray[i] = FAT_TERMINATOR;
        }
        fatArray[1] = 5;
        fatArray[5] = 9;
        fatArray[9] = 20;
        fatArray[20] = FAT_TERMINATOR;
        
        fat.setAll(fatArray);
        fat.getAll(readArray);
        REQUIRE(memcmp(fatArray, readArray, sizeof(fatArray)) == 0);
    }
}

TEST_CASE("FAT.addlasttoFAT", "[FAT]"){
    SECTION("On empty FAT"){
        FAT fat = FAT();
        std::list<uint16_t> readList;
        
        fat.addLastToFAT(0);
        fat.iterateFAT(0, &readList);
        REQUIRE(readList.size() == 1);
    }
    SECTION("Override value"){
        FAT fat = FAT();
        std::list<uint16_t> readList;
        
        fat.addToFAT(0, 6458);
        fat.addLastToFAT(0);
        fat.iterateFAT(0, &readList);
        REQUIRE(readList.size() == 1);
    }
    SECTION("Multiple values"){
        FAT fat = FAT();
        
        fat.addToFAT(345, 765);
        fat.addToFAT(345, 34);
        fat.addToFAT(345, 8);
        fat.addToFAT(345, 9261);
        fat.addLastToFAT(9261);
        
        std::list<uint16_t> list;
        fat.iterateFAT(345, &list);
        REQUIRE(list.size() == 5);
    }
}

/*
 TEST_CASE("FAT.addToFAT", "[FAT]"){
 SECTION("On empty FAT"){
 FAT fat = FAT();
 uint16_t fatArray[DATA_BLOCKS];
 uint16_t readArray[DATA_BLOCKS];
 
 for (int i = 0; i< DATA_BLOCKS; i++) {
 readArray[i] = fatArray[i] = 0;
 }
 
 //       fat.setAll(<#char *p#>)
 REQUIRE(false); // todo finish test case
 }
 
 SECTION("Multiple values"){
 FAT fat = FAT();
 
 std::list<uint16_t> values;
 values.push_back(345);
 values.push_back(765);
 values.push_back(34);
 values.push_back(8);
 values.push_back(9261);
 fat.addToFAT(345, 765);
 fat.addToFAT(345, 34);
 fat.addToFAT(345, 8);
 fat.addToFAT(345, 9261);
 fat.addLastToFAT(9261);
 
 
 std::list<uint16_t> list;
 std::list<uint16_t >::const_iterator iterator;
 std::list<uint16_t >::const_iterator valIter;
 fat.iterateFAT(345, &list);
 bool allSame = true;
 for (iterator = list.begin(), valIter = values.begin(); iterator != list.end(); ++iterator, valIter++){
 if (*iterator != *valIter) {
 allSame = false;
 break;
 }
 }
 REQUIRE(allSame);
 }
 }
 */
TEST_CASE("FAT.addNextToFat / FAT.getNext", "[FAT]") {
    SECTION("One Value") {
        FAT fat = FAT();
        fat.addNextToFAT(0, 1);
        uint16_t nextBlock;
        fat.getNext(0, &nextBlock);
        REQUIRE(1 == nextBlock);
    }
    SECTION("Multiple values"){
        FAT fat = FAT();
        
        std::list<uint16_t> values;
        values.push_back(345);
        values.push_back(765);
        values.push_back(34);
        values.push_back(8);
        values.push_back(9261);
        fat.addNextToFAT(345, 765);
        fat.addNextToFAT(765, 34);
        fat.addNextToFAT(34, 8);
        fat.addNextToFAT(8, 9261);
        fat.addLastToFAT(9261);
        
        
        std::list<uint16_t> list;
        std::list<uint16_t >::const_iterator iterator;
        std::list<uint16_t >::const_iterator valIter;
        fat.iterateFAT(345, &list);
        bool allSame = true;
        for (iterator = list.begin(), valIter = values.begin(); iterator != list.end(); ++iterator, valIter++){
            if (*iterator != *valIter) {
                allSame = false;
                break;
            }
        }
        REQUIRE(allSame);
        uint16_t returnValue;
        fat.getNext(345, &returnValue);
        REQUIRE(returnValue == 765);
        fat.getNext(765, &returnValue);
        REQUIRE(returnValue == 34);
        fat.getNext(34, &returnValue);
        REQUIRE(returnValue == 8);
        fat.getNext(8, &returnValue);
        REQUIRE(returnValue == 9261);
        fat.getNext(9261, &returnValue);
        REQUIRE(returnValue == FAT_TERMINATOR);
    }
}
TEST_CASE("FAT.iterateFat","[FAT]") {
    SECTION("Empty FAT") {
        FAT fat = FAT();
        std::list<uint16_t> list;
        fat.iterateFAT(0, &list);
        REQUIRE(list.size() == 1);
    }
    SECTION("Multiple values"){
        FAT fat = FAT();
        
        std::list<uint16_t> values;
        values.push_back(345);
        values.push_back(765);
        values.push_back(34);
        values.push_back(8);
        values.push_back(9261);
        fat.addNextToFAT(345, 765);
        fat.addNextToFAT(765, 34);
        fat.addNextToFAT(34, 8);
        fat.addNextToFAT(8, 9261);
        fat.addLastToFAT(9261);
        
        
        std::list<uint16_t> list;
        std::list<uint16_t >::const_iterator iterator;
        std::list<uint16_t >::const_iterator valIter;
        fat.iterateFAT(345, &list);
        bool allSame = true;
        for (iterator = list.begin(), valIter = values.begin(); iterator != list.end(); ++iterator, valIter++){
            if (*iterator != *valIter) {
                allSame = false;
                break;
            }
        }
        REQUIRE(allSame);
        REQUIRE(list.size() == 5);
    }
}
TEST_CASE("FAT.deleteFromFAT", "[FAT]") {
    SECTION("Empty FAT") {
        FAT fat = FAT();
        fat.deleteFromFAT(314);
        FAT fat2 = FAT();
        REQUIRE(memcmp(&fat, &fat2, sizeof(FAT)));
    }
    SECTION("Multiple Values") {
        FAT fat = FAT();
        fat.addNextToFAT(345, 765);
        fat.addNextToFAT(765, 34);
        fat.addNextToFAT(34, 8);
        fat.addNextToFAT(8, 9261);
        fat.addLastToFAT(9261);
        fat.deleteFromFAT(345);
        FAT fat2 = FAT();
        REQUIRE(memcmp(&fat, &fat2, sizeof(FAT)));
    }
}

