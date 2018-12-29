//
//  test-myfs-time.cpp
//  unittests
//
//  Created by MK on 29.12.18.
//  Copyright © 2018 Oliver Waldhorst. All rights reserved.
//


#include "catch.hpp"

#include <string.h>
#include <iostream>
#include <chrono>

#include "myfs-structs.h"
#include "helper.hpp"
#include "constants.h"

int fib(int n);

TEST_CASE("TESTTIME"){
    using namespace std::chrono;
    //milliseconds now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    auto timeStart = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//    time_t timeStart = time(nullptr);
    system("/Users/mk/Library/Developer/Xcode/DerivedData/BSUe-Template-ddgmmbcnyqafpudaoinqpvbhqmbp/Build/Products/Debug/mkfs.myfs /Users/mk/Library/Developer/Xcode/DerivedData/BSUe-Template-ddgmmbcnyqafpudaoinqpvbhqmbp/Build/Products/Debug/Dateisystem.txt /Users/mk/Library/Developer/Xcode/DerivedData/BSUe-Template-ddgmmbcnyqafpudaoinqpvbhqmbp/Build/Products/Debug/numbers.txt");
//    time_t timeEnd = time(nullptr);
    
    
//    fib(42);
    
    
    
        auto timeEnd = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    duration<double> elapsed_seconds = timeEnd-timeStart;
    duration<double> notcasted = elapsed_seconds;
    using namespace std;
    cout << "Elapsed Miliseconds: " << duration_cast<milliseconds>(elapsed_seconds).count() << endl;
    REQUIRE(-1 < 0);
}

int fib(int n){
    if (n < 2) return n;
    return fib(n-1) + fib(n-2);
}
