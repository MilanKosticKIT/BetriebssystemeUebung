//
//  helper.cpp
//  testing
//
//  Created by Oliver Waldhorst on 15.12.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include "helper.hpp"

void gen_random(char *s, const int len) {
    static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
}

int createFile(char* name, off_t fileSize) {
    std::cout << "Creating file " << name << " (size : " << fileSize << ")" << std::endl;
    int fd = open(name, O_CREAT | 0666);
    if (fd < 0) return -1;
    size_t size = 10;
    char buffer[size];
    for (int i = 0; i < (int)size; i++) {
        buffer[i] = '0';
    }
    buffer[size - 1] = '\n';
    for (int i = 0; i < (int)(fileSize / size); i++) {
        write(fd, buffer, size);
        for (int j = size - 2; j >= 0; j--) {
            if (buffer[j] == '9') {
                buffer[j] = '0';
            } else {
                buffer[j]++;
                break;
            }
        }
    }
    write(fd, buffer, (fileSize % size));

    close(fd);
    return 0;
}

// TODO: Implement you helper funcitons here
