//
// Created by hannes on 30.10.18.
//

#ifndef FILESYSTEMIO_H
#define FILESYSTEMIO_H
#include <stdint.h>
#include <stdlib.h>
#include <type_traits> //Neccessary for is_trivially_copyable
#include <string.h> //Neccessary for memcopy

#include "blockdevice.h"
#include "constants.h"

class FilesystemIO {
private:
    BlockDevice* blockDevice;

public:

    /**
     * Creates a new IO-Interface for a blockdevice.
     */
    FilesystemIO(BlockDevice* blockdevice) {
        blockDevice = blockdevice;
    }

    /**
     * Writes a generic type of data-array into the blockdevice. Writes to the position block sets.
     * @tparam T The generic type of the data should be written.
     * @tparam N The size of the data array, that should be written.
     * @param block The number of the block where the data should be written to.
     * @param data The data, an array, that should be written.
     */
    template<class T>
    int writeDevice(size_t block, const T *data, size_t size) {
        const char *rawData = reinterpret_cast<const char *>(data);
        static char buffer[BLOCK_SIZE];
        size_t blockCount = size / BLOCK_SIZE;
        size_t currentBlock = block;
        int ret = 0;
        for (; currentBlock < block + blockCount; ++currentBlock) {
            ret = blockDevice->write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
            if (ret < 0) return ret;
        }
        memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), size % BLOCK_SIZE);
        ret = blockDevice->write(currentBlock, buffer);
        if (ret < 0) return ret;
        return 0;
    }

    /** todo clean comments
     * Reads a generic type of data-array from the blockdevice. The position which should be read from is block.
     * @tparam T The generic type of the data should be read.
     * @tparam N The size of the data array, that should be read.
     * @param block The number of the block where the data should be read from.
     * @param data Return parameter, the data, an array, that was to be read.
     *
     */
    template<class T>
    int readDevice(size_t block, T* data, size_t size) {
        char *rawData = reinterpret_cast<char *>(data);
        static char buffer[BLOCK_SIZE];
        size_t blockCount = size / BLOCK_SIZE;
        size_t currentBlock = block;
        int ret = 0;
        for (; currentBlock < block + blockCount; ++currentBlock) {
            ret = blockDevice->read(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
            if (ret < 0) return ret;
        }
        ret = blockDevice->read(currentBlock, buffer);
        if (ret < 0) return ret;
        memcpy(rawData + ((currentBlock - block) * BLOCK_SIZE), buffer, size % BLOCK_SIZE);
        return 0;
    }

};

#endif //FILESYSTEMIO_H
