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

class NewFilesystemIO {
private:
    BlockDevice blockDevice;

public:

    /**
     * Creates a new IO-Interface for a blockdevice.
     */
    NewFilesystemIO(BlockDevice &blockdevice) {
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
    void writeDevice(size_t block, const T *data, size_t size) {
        const char *rawData = reinterpret_cast<const char *>(data);
        static char buffer[BLOCK_SIZE];
        size_t blockCount = size / BLOCK_SIZE;
        size_t currentBlock = block;
        for (; currentBlock < block + blockCount; ++currentBlock) {
            blockDevice.write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
        }
        memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), size % BLOCK_SIZE);
        blockDevice.write(currentBlock, buffer);
    }

    /** todo clean
     * Reads a generic type of data-array from the blockdevice. The position which should be read from is block.
     * @tparam T The generic type of the data should be read.
     * @tparam N The size of the data array, that should be read.
     * @param block The number of the block where the data should be read from.
     * @param data Return parameter, the data, an array, that was to be read.
     * todo size
     */
    template<class T>
    void readDevice(size_t block, T* data, size_t size) {
        char *rawData = reinterpret_cast<char *>(data);
        static char buffer[BLOCK_SIZE];
        size_t blockCount = size / BLOCK_SIZE;
        size_t currentBlock = block;
        for (; currentBlock < block + blockCount; ++currentBlock) {
            blockDevice.read(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
        }
        blockDevice.read(currentBlock, buffer);
        memcpy(rawData + ((currentBlock - block) * BLOCK_SIZE), buffer, size % BLOCK_SIZE);
    }

};

#endif //FILESYSTEMIO_H
