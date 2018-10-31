//
// Created by hannes on 30.10.18.
//

#ifndef CLIONPROJECTS_FILESYSTEMIO_H
#define CLIONPROJECTS_FILESYSTEMIO_H
#include <stdlib.h>
#include "blockdevice.h"

class FilesystemIO {
private:
    BlockDevice blockdevice;

public:
    // todo ticket #17: create blockdevice or get blockdevice/path parameter?
    /**
     * Creates a new IO-Interface for a blockdevice.
     */
    FilesystemIO();

    /**
     * todo
     * @tparam T
     * @param block
     * @param data
     */
    template<class T>
    void writeDevice(size_t block, const T &data);

    /**
     * todo
     * @tparam T
     * @param block
     * @param data
     */
    template<class T>
    void readDevice(size_t block, T &data);

    /**
     * todo
     * @tparam T
     * @tparam N
     * @param block
     * @param data
     */
    template<class T, size_t N>
    void writeDevice(size_t block, const T (&data)[N]);

    /**
     * todo
     * @tparam T
     * @tparam N
     * @param block
     * @param data
     */
    template<class T, size_t N>
    void readDevice(size_t block, T (&data)[N]);

};

#endif //CLIONPROJECTS_FILESYSTEMIO_H
