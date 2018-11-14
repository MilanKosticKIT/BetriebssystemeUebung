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

typedef struct {
    char name[NAME_LENGTH];
    off_t size;
    uid_t userID;
    gid_t groupID;
    time_t last_time;
    time_t modi_time;
    time_t change_time;
    uint16_t  first_block;
} fileStats;

class Root {
private:
    fileStats rootArray[DATA_BLOCKS];
public:
    Root();
    ~Root();
    void getAll(fileStats* filestats);
    void setAll(fileStats* filestats);
    int deleteFromRoot(const char* name);
    void get (uint16_t num, fileStats* filestats);
    int get(const char* name, fileStats* filestats);
    int setFistBLock(const char* name, uint16_t firstBlock);
    uint16_t set (uint16_t num, char* filePath);

};


#endif //MYFS_ROOT_H
