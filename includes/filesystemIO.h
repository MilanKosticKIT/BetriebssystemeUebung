//
// Created by hannes on 30.10.18.
//

#ifndef CLIONPROJECTS_FILESYSTEMIO_H
#define CLIONPROJECTS_FILESYSTEMIO_H
#include <stdint.h>
#include <stdlib.h>
#include <type_traits> //Neccessary for is_trivially_copyable
#include <string.h> //Neccessary for memcopy

#include "blockdevice.h"
#include "constants.h"

class FilesystemIO {
private:
    BlockDevice blockDevice;

public:
    
    /**
     * Creates a new IO-Interface for a blockdevice.
     */
    FilesystemIO(BlockDevice blockdevice);

    /**
     * Writes a generic type of data into the blockdevice. Writes to the position block sets.
     * @tparam T The generic type of the data that should be written.
     * @param block The number of the block where the data should be written to.
     * @param data The data that should be written.
     */
    template<class T>
    void writeDevice(size_t block, const T &data);

    /**
     * Reads a generic type of data from the blockdevice. The position which should be read from is block.
     * @tparam T The generic type of the data that should be read.
     * @param block The number of the block where the data should be read from.
     * @param data Return parameter,the data that was be read.
     */
    template<class T>
    void readDevice(size_t block, T &data);

    /**
     * Writes a generic type of data-array into the blockdevice. Writes to the position block sets.
     * @tparam T The generic type of the data should be written.
     * @tparam N The size of the data array, that should be written.
     * @param block The number of the block where the data should be written to.
     * @param data The data, an array, that should be written.
     */
    template<class T, size_t N>
    void writeDevice(size_t block, const T (&data)[N]);

    /**
     * Reads a generic type of data-array from the blockdevice. The position which should be read from is block.
     * @tparam T The generic type of the data should be read.
     * @tparam N The size of the data array, that should be read.
     * @param block The number of the block where the data should be read from.
     * @param data Return parameter, the data, an array, that was be read.
     */
    template<class T, size_t N>
    void readDevice(size_t block, T (&data)[N]);

};

#endif //CLIONPROJECTS_FILESYSTEMIO_H
