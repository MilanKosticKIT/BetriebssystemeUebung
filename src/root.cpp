    //
// Created by test on 05.11.2018.
//
#include <errno.h>

#include "root.h"


//create new empty filestats Array
Root::Root() {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        rootArray[i].size = -1; //todo: set to no entry?
    }
}

Root::~Root() {
}

//return full filestats array (for writing to hard driver)
void Root::getAll(fileStats* filestats) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        * (filestats + i) = rootArray[i];
    }
}
//set filestats array (for reading from hard driver)
void Root::setAll(fileStats* filestats) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++){
        rootArray[i] = *(filestats + i);
    }
}


// deletes the filestats with the given name.
int Root::deleteEntry(const char *name) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        fileStats currentFileStats = rootArray[i];
        if (strcmp(currentFileStats.name, name) == 0) {
            rootArray[i] = {};
            rootArray[i].size = -1;
            return 0;
        }
    }
    errno = ENOENT;
    return -1;
}

// creates a new root entry for the file with the given name.
int Root::createEntry(const char *name) {
    if (strlen(name) > NAME_LENGTH) {
        errno = ENAMETOOLONG;
        return -1;
    }
    fileStats stats = {};
    strcpy(stats.name, name);
    stats.userID = geteuid();
    stats.groupID = getegid();
    time_t currentTime;
    time(&currentTime);
    stats.modi_time = currentTime;
    stats.last_time = currentTime;
    stats.change_time = currentTime;

    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (strcmp(rootArray[i].name, name) == 0) {
            errno = EEXIST;
            return -1;
        }
    }
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        if (rootArray[i].size < 0) { //todo: which flag indicates no entry?
            rootArray[i] = stats;
            return 0;
        }

    }
    errno = ENFILE;
    return -1;
}

// get the filestats of the given file
int Root::get(const char* name, fileStats* filestats) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        fileStats currentFileStats = rootArray[i];
        if (strcmp(currentFileStats.name, name) == 0) {
            *filestats = currentFileStats;
            return 0;
        }
    }
    errno = ENOENT;
    return -1;
}

// set the filestats of the given file to the given values, if it exists (names are compared).
int Root::update(fileStats filestats) {
    for (int i = 0; i < ROOT_ARRAY_SIZE; i++) {
        fileStats currentFileStats = rootArray[i];
        if (strcmp(currentFileStats.name, filestats.name) == 0) {
            rootArray[i] = filestats;
            return 0;
        }
    }
    errno = ENOENT;
    return -1;
}


//return filestats of the file under given number
void Root::get(uint16_t num, fileStats* filestats) {
    *filestats = rootArray[num];
}

//get filestats info from new file and add it to given position
//in array
int Root::set(uint16_t num, char* filePath) {
    struct stat sb;
    stat(filePath, &sb);
    char *filename = basename(filePath);
    if (strlen(filename) > NAME_LENGTH) {
        return -1;
    }
    fileStats* status = new fileStats;
    strcpy(status->name, filename);
    status->size = sb.st_size;
    status->userID = geteuid();
    status->groupID = getegid();
    status->modi_time = sb.st_mtime;
    time(&(status->last_time));
    time(&(status->change_time));
    rootArray[num] = *status;

    delete status;
    return 0;
}
