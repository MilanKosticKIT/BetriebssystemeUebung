//
// Created by hannes on 30.10.18.
//

#include "filesystemIO.h"

FilesystemIO::FilesystemIO(BlockDevice &blockdevice) {
    blockDevice = blockdevice;
}

template<class T>
void FilesystemIO::writeDevice(size_t block, const T &data) {
    static_assert(std::is_trivially_copyable<T>::value, "Can't operate on complex types!");

    const char *rawData = reinterpret_cast<const char *>(&data);

    static char buffer[BLOCK_SIZE];
    size_t blockCount = sizeof(T) / BLOCK_SIZE;
    size_t currentBlock = block;
    for (; currentBlock < block + blockCount; ++currentBlock) {
        blockDevice.write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
    }
    memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), sizeof(T) % BLOCK_SIZE);
    blockDevice.write(currentBlock, buffer);
}

template<class T, size_t N>
void FilesystemIO::writeDevice(size_t block, const T (&data)[N]) {
    static_assert(std::is_trivially_copyable<T>::value, "Can't operate on complex types!");

    const char *rawData = reinterpret_cast<const char *>(&data);

    static char buffer[BLOCK_SIZE];
    size_t blockCount = (sizeof(T) * N)/ BLOCK_SIZE;
    size_t currentBlock = block;
    for (; currentBlock < block + blockCount; ++currentBlock) {
        blockDevice.write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
    }
    memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), (sizeof(T) * N) % BLOCK_SIZE);
    blockDevice.write(currentBlock, buffer);
}

template<class T>
void FilesystemIO::readDevice(size_t block, T &data) {
    static_assert(std::is_trivially_constructible<T>::value, "Can't operate on complex types!");

    char *rawData = reinterpret_cast<char *>(&data);
    static char buffer[BLOCK_SIZE];
    std::size_t blockCount = sizeof(T) / BLOCK_SIZE;
    std::size_t currentBlock = block;
    for (; currentBlock < block + blockCount; ++currentBlock) {
        blockDevice.read(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
    }
    blockDevice.read(currentBlock, buffer);
    memcpy(rawData + ((currentBlock - block) * BLOCK_SIZE), buffer, sizeof(T) % BLOCK_SIZE);
}

template<class T, size_t N>
void FilesystemIO::readDevice(std::size_t block, T (&data)[N]) {
    static_assert(std::is_trivially_constructible<T>::value, "Can't operate on complex types!");

    char *rawData = reinterpret_cast<char *>(&data);
    static char buffer[BLOCK_SIZE];
    std::size_t blockCount = (sizeof(T) * N) / BLOCK_SIZE;
    std::size_t currentBlock = block;
    for (; currentBlock < block + blockCount; ++currentBlock) {
        blockDevice.read(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
    }
    blockDevice.read(currentBlock, buffer);
    memcpy(rawData + ((currentBlock - block) * BLOCK_SIZE), buffer, (sizeof(T) * N) % BLOCK_SIZE);
}
