//
// Created by katze on 14.12.18.
//

#ifndef MYFS_LOG_H
#define MYFS_LOG_H

#include <cstdio>
#include <fstream>


template<class T> void log(T x) {
    std::ofstream myfile;
    myfile.open ("log.txt", std::ios::app);
    myfile << x << "\n";
    myfile.close();
}

template<class T> void log(std::string s, T x) {
    std::ofstream myfile;
    myfile.open ("log.txt", std::ios::app);
    myfile << s << x << "\n";
    myfile.close();
}



#endif //MYFS_LOG_H
