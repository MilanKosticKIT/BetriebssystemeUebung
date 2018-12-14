//
// Created by katze on 14.12.18.
//

#ifndef MYFS_LOG_H
#define MYFS_LOG_H

#include <cstdio>

void log() {
    static FILE *logfile = fopen("logfile.txt", "w+");
}


#endif //MYFS_LOG_H
