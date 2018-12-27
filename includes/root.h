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
#define DIR_STATS (rootArray[ROOT_ARRAY_SIZE])

class Root {
private:
    fileStats *rootArray = new fileStats[ROOT_ARRAY_SIZE + 1];
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

    void get(int num, fileStats* filestats);
    int set(int num, char* filePath);
    bool exists(int index);
    int getName(int index, char** name);

};


#endif //MYFS_ROOT_H
