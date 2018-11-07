//
// Created by test on 05.11.2018.
//

#ifndef MYFS_ROOT_H
#define MYFS_ROOT_H

typedef struct {
    char name[NAME_LENGTH];
    off_t size;
    uid_t userID;
    gid_t groupID;
    time_t last_time;
    time_t modi_time;
    time_t change_time;
} fileStats;

class root {
private:
    fileStats rootArray[];
public:
    root();
    fileStats[]* getAll();
    void initialize(fileStats[]*);
    fileStats* get (uint16_t);
    uint16_t set (uint16_t, char*);

};


#endif //MYFS_ROOT_H