//
// Created by test on 05.11.2018.
//
#include "constants.h"
#include "root.h"



//create new empty filestats Array

Root::root() {
    rootArray = new fileStats[DATA_BLOCKS];
}

Root::~root() {
    delete[] rootArray;
}

//return full filestats array (for writing to hard driver)
void Root::getAll(fileStats* filestats) {
    filestats = rootArray;
}
//set filestats array (for reading from hard driver)
void Root::setAll(fileStats* filestats) {
    rootArray = filestats;
}
//return filestats of the file under given number
void Root::get(uint16_t num, fileStats* filestats) {
    *filestats = rootArray[num];
}
//get filestats info from new file and add it to given position
//in array
uint16_t Root::set(uint16_t num, char* filePath) {
    struct stat sb;
    stat(path, &sb);
    char *filename = basename(path);
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
    this->rootArray[num] = status;
return 0;
}

};
