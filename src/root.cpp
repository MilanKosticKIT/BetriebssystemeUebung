    //
// Created by test on 05.11.2018.
//
#include <errno.h>
#include "constants.h"
#include "root.h"



//create new empty filestats Array

Root::Root() {
}

Root::~Root() {
}

//return full filestats array (for writing to hard driver)
void Root::getAll(fileStats* filestats) {
    filestats = rootArray;
}
//set filestats array (for reading from hard driver)
void Root::setAll(fileStats* filestats) {
    for (int i = 0; i < DATA_BLOCKS; i++){
        rootArray[i] = *(filestats + i);
    }
}
//return filestats of the file under given number
void Root::get(uint16_t num, fileStats* filestats) {
    *filestats = rootArray[num];
}

// Gets the filestats with the given name.
int Root::get(const char* name, fileStats* filestats) {
    for (int i = 0; i < DATA_BLOCKS; i++) {
        fileStats currentFileStats = rootArray[i];
        if (strcmp(currentFileStats.name, name) == 0) {
            *filestats = currentFileStats;
            return 0;
        }
    }
    errno = ENOENT;
    return -1;
}

//get filestats info from new file and add it to given position
//in array
uint16_t Root::set(uint16_t num, char* filePath) {
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
return 0;
}
