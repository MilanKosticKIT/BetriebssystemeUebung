//
// Created by hannes on 30.10.18.
//

#ifndef CLIONPROJECTS_FILESYSTEMIO_H
#define CLIONPROJECTS_FILESYSTEMIO_H

class FilesystemIO {
private:
    Blockdevice blockdevice;

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
    void writeDevice(std::size_t block, const T &data);

    /**
     * todo
     * @tparam T
     * @param block
     * @param data
     */
    template<class T>
    void readDevice(std::size_t block, T &data);

    /**
     * todo
     * @tparam T
     * @tparam N
     * @param block
     * @param data
     */
    template<class T, std::size_t N>
    void writeDevice(std::size_t block, const T (&data)[N]);

    /**
     * todo
     * @tparam T
     * @tparam N
     * @param block
     * @param data
     */
    template<class T, std::size_t N>
    void readDevice(std::size_t block, T (&data)[N]);

};

#endif //CLIONPROJECTS_FILESYSTEMIO_H
