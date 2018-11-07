//
// Created by test on 05.11.2018.
//
#include "constants.h"
#include "root.h"



//create new empty filestats Array

root::root() {
    this->rootArray = new fileStats[DATA_BLOCKS];
}

//return filestats of the file under given number
fileStats* root::get(uint16_t num) {
    return &this->rootArray[num];
}
//return full filestats array (for writing to hard driver)
fileStats* root::getAll() {
    return &this->rootArray;
}
//copy given filestats Array (for take data from Hard Disk)
void root::initialize(fileStats[]* newData) {
    this->rootArray = &newData;
}
//get filestats info from new file and add it to given position
//in array
uint26_t root::set(uint16_t num, char* filePath) {
    struct stat sb;
    stat(path, &sb);
    char *filename = basename(path);
    if (strlen(filename) > NAME_LENGTH) {
        return -1;
    }
    fileStats status = new fileStats;
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