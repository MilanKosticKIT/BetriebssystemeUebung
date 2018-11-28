//
// Created by test on 05.11.2018.
//

#ifndef MYFS_ROOT_H
#define MYFS_ROOT_H

#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <libgen.h>  //basename
#include <unistd.h>  //getuid /getgid
#include <ctime>  //Time()

#include "constants.h"
#include "myfs-structs.h"

#define ROOT_ARRAY_SIZE NUM_DIR_ENTRIES

class Root {
private:
    fileStats rootArray[ROOT_ARRAY_SIZE];
public:
    Root();
    ~Root();
    void getAll(fileStats* filestats);
    void setAll(fileStats* filestats);

    //Methods with filename:

    int get(const char* name, fileStats* filestats);
    int update(fileStats filestats);
    int deleteEntry(const char* name);
    int createEntry(const char* name);

    //Methods with position(--> file descriptor):

    void get(uint16_t num, fileStats* filestats);
    int set(uint16_t num, char* filePath);

};


#endif //MYFS_ROOT_H
