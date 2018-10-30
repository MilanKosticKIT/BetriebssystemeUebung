//
// Created by hannes on 30.10.18.
//

#include "../includes/FilesystemIO.h"

void FilesystemIO::writeDevice(std::size_t block, const T &data) {
    static_assert(std::is_trivially_copyable<T>::value, "Can't operate on complex types!");

    const char *rawData = reinterpret_cast<const char *>(&data);

    static char buffer[BLOCK_SIZE];
    std::size_t blockCount = sizeof(T) / BLOCK_SIZE;
    std::size_t currentBlock = block;
    for (; currentBlock < block + blockCount; ++currentBlock) {
        blockDevice.write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
    }
    std::memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), sizeof(T) % BLOCK_SIZE);
    blockDevice.write(currentBlock, buffer);
}

void FilesystemIO::writeDevice(std::size_t block, const T (&data)[N]) {
    static_assert(std::is_trivially_copyable<T>::value, "Can't operate on complex types!");
    static_assert(sizeof(T) * N <= BLOCK_SIZE, "");

    static char buffer[BLOCK_SIZE];
    std::memcpy(buffer, data, sizeof(T) * N);
    blockDevice.write(block, buffer);
}

void FilesystemIO::readDevice(std::size_t block, T &data) {
    static_assert(std::is_trivially_constructible<T>::value, "Can't operate on complex types!");
    static_assert(sizeof(T) <= BLOCK_SIZE, "");

    static char buffer[BLOCK_SIZE];
    blockDevice.read(block, buffer);
    std::memcpy(data, buffer, sizeof(T));
}

void FilesystemIO::readDevice(std::size_t block, T (&data)[N]) {
    static_assert(std::is_trivially_constructible<T>::value, "Can't operate on complex types!");
    static_assert(sizeof(T) * N <= BLOCK_SIZE, "");

    static char buffer[BLOCK_SIZE];
    blockDevice.read(block, buffer);
    std::memcpy(data, buffer, sizeof(T) * N);
}