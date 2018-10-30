//
// Created by hannes on 30.10.18.
//

#ifndef CLIONPROJECTS_FILESYSTEMIO_H
#define CLIONPROJECTS_FILESYSTEMIO_H

class FilesystemIO {
private:
    Blockdevice blockdevice;

public:
    /*
     * Write trivially copyable data to the given block. If the data doesn't fit into a single block, the next blocks are used for the remaining data.
     */
    template<class T>
    void writeDevice(std::size_t block, const T &data);

    /*
     * Read trivially copyable data from a single block and store it to "data". todo description
     */
    template<class T>
    void readDevice(std::size_t block, T &data);

    /*
     * Write an array of data to the given block. todo description
     */
    template<class T, std::size_t N>
    void writeDevice(std::size_t block, const T (&data)[N]);
    /*
     * todo description
     */
    template<class T, std::size_t N>
    void readDevice(std::size_t block, T (&data)[N]);

};

#endif //CLIONPROJECTS_FILESYSTEMIO_H
