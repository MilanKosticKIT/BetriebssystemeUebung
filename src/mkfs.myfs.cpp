//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include "stdio.h"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <iostream>
#include <cstring>
#include <type_traits>

typedef struct{
char name[NAME_LENGTH];
off_t size;
uid_t userID;
gid_t groupID;
time_t last_time;
time_t modi_time;
time_t change_time;
}fileStats;

//MARK: - Methodenheader
//DMAP
bool checkSpace(uint64_t blockCount);
void clearPointInDMAP(u_int16_t clearAddress);
void setPointInDMAP(u_int16_t setAddress);
bool isAdressFull(u_int16_t blockNo);
int findFreeBlockDMAP(uint32_t* freeBlock);
int getFreeBlck(uint32_t* freeBlock);
//TODO: Root?
MetaData getMetaData(u_int8_t indexOfFile);
void setMetaData(MetaData metaData, u_int8_t indexOfFile);
void convertBlockToMetaData(MetaData* data, char* block);
void convertMetaDataToBlock(MetaData* data, char* block);
void getBLockFromAddress(u_int32_t address, u_int32_t* blockNo, u_int32_t* byteNo);
//Superblock
void getSuperBlock(SuperBlock* superblock);
void setEmptySpaceSizeInSuperBlock(uint32_t emptySpaceSize);
void createSuperBlock(struct SuperBlock superBlock);
//FAT
void initializeFAT();
int iterateFAT(int firstBlock, std::list<int>* list);
int addToFAT(int32_t firstBlock, int32_t nextAddress);
void addLastToFAT(int32_t lastAddress);
//TODO: Get these lines to a fitting header file.
//MARK: -

BlockDevice blockDevice = *new BlockDevice();


bool getStats(fileStats *status, char *path, blkcnt_t *blockCount){
struct stat sb;
stat(path,&sb);
    char* filename = basename(path);
    if (strlen(filename) > NAME_LENGTH) {
        return false;
    }
    strcpy(status->name, filename);//TODO: Pointer?!?!
    status->size = sb.st_size;
    status->userID = geteuid();
    status->groupID = getegid();
    status->modi_time = sb.st_mtime;
    time(&(status->last_time));
    time(&(status->change_time));
    *blockCount = sb.st_blocks;
    return true;
}
void updateRoot(fileStats *status){
fileStats root[NUM_DIR_ENTRIES];




}

template <class T> void writeDevice(std::size_t block, const T& data) {
    static_assert(std::is_trivially_copyable<T>::value, "Can't operate on complex types!");

	const char* rawData = reinterpret_cast<const char*>(&data);
	
	static char buffer[BLOCK_SIZE];
	std::size_t blockCount = sizeof(T) / BLOCK_SIZE;
	std::size_t currentBlock = block;
	for(; currentBlock < block + blockCount; ++currentBlock) {

        blockDevice.write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
	}
	std::memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), sizeof(T) % BLOCK_SIZE);
    blockDevice.write(currentBlock, buffer);
}

template <class T, std::size_t N> void writeDevice(std::size_t block, const T (&data)[N]) {
	static_assert(std::is_trivially_copyable<T>::value, "Can't operate on complex types!");
    static_assert(sizeof(T) * N <= BLOCK_SIZE, ""); //TODO sizeOF(T)*N or (T * N)

	static char buffer[BLOCK_SIZE];
	std::memcpy(buffer, &data, sizeof(T) * N);
	blockDevice.write(block, buffer);
}

template<class T> void readDevice(std::size_t block, T* data) {
    static_assert(std::is_trivially_constructible<T>::value, "");
	static_assert(sizeof(T) <= BLOCK_SIZE, "Can't operate on complex types!");

	static char buffer[BLOCK_SIZE];
	blockDevice.read(block, buffer);
	std::memcpy(data, buffer, sizeof(T));
}

template<class T, std::size_t N> void readDevice(std::size_t block, T (*data)[N]) {
	static_assert(std::is_trivially_constructible<T>::value, "Can't operate on complex types!");
    static_assert(sizeof(T) * N <= BLOCK_SIZE, "");

	static char buffer[BLOCK_SIZE];
	blockDevice.read(block, buffer);
	std::memcpy(data, buffer, sizeof(T) * N);
}



int main(int argc, char *argv[]) {

    blockDevice.create("/Users/milank/Documents/Studium/SS 2018/Betriebsysteme/Labor/BetriebssystemeUebung/Blockdevice.bin");

	fileStats foobar;
	foobar.size = 1024;
	writeDevice(15, foobar);

	fileStats bar;
	readDevice(15, &bar);

	std::cout << bar.size << std::endl;
		
int foo [5] = { 16, 2, 77, 40, 12071 }; 
int fuu [5];
writeDevice(10, foo);
readDevice(10,fuu);
std::cout << fuu << std::endl;
    
    // TODO: Implement file system generation & copying of files here
    return 0;
}










//MARK: - Our Methods

//MARK: - DMAP
//Called to check if there is enough space (blockCount) in the fileSystem
bool checkFreeSpace(uint64_t blockCount){
    SuperBlock* superblock;
    getSuperBlock(superblock);
    return (*superblock).emptySpaceSize >= blockCount;
}

//Marks an address as empty.
void clearPointInDMAP(u_int16_t clearAddress) {
    int byte = clearAddress / 8;
    int bit = clearAddress % 8;
    
    char* byteToChange = dMap[byte];
    //TODO: Check whether this version 1
    *byteToChange &= ~(1 << (7 - bit));
}

//Marks an address as full
void setPointInDMAP(u_int16_t setAddress) {
    int byte = setAddress / 8;
    int bit = setAddress % 8;
    
    char* byteToChange = dMap + byte;
    //TODO: Or this version 2 is correct
    *byteToChange |= (1 << (7 - bit));
}

//Called to check wehther an address is full
bool isAdressFull(u_int16_t blockNo) {
    int byte = blockNo / 8;
    int bit = blockNo % 8;
    
    char* byteToChange = &dMap[byte];
    //TODO: Or this version 3
    bool isSet (*byteToRead & (1 << (7 - bit)));
    return isSet;
}

//Returns -1 when nothing found, >=0  when something found
int findFreeBlockDMAP(uint32_t* freeBlock){
    //uint8_t currentByteChar;// = (char) malloc(1);
    uint16_t currentByte = 0;
    uint8_t currentBit = 0;
    
    
    
    
    
    //TODO: Change 16 to DMAP_SIZE when working
    while (currentBlock < 16) {//run over blocks
        blockDevice.read(DMAP_START + currentBlock, block);
        currentByte = 0;
        //TODO: Change 512 to BLOCK_SIZE when working
        while (currentByte < 512) {//run over byte
            currentByteChar = block[currentByte];
            currentBit = 0;
            if (currentByteChar != 0xFF) {
                while (currentBit < 8) {//run over bit
                    if (~(currentByteChar | (1 << (7 - currentBit)))) {
                        *freeBlock = currentBlock * 512 + currentByte * 8 + currentBit;
                        return 0;
                    }
                    currentBit++;
                }
            }
            currentByte++;
        }
        currentBlock++;
    }
    return -1;
}

//TODO: Write Comment
//Returns the first empty block.
//When an error occurs -1 is returned, else 0.
int getFreeBlock(uint32_t* freeBlock){
    if (nextFreeBlock <= MAX_FILE_SYSTEM_SIZE) {// TODO: MAX_SIZE = numer of blocks in file system
        *freeBlock = nextFreeBlock;
        return 0;
    } else {
        return -1;
    }
}

//MARK: - Root
//Returns the metaData of the file behind the index.
MetaData getMetaData(u_int8_t indexOfFile) {
    u_int32_t blockNo = ROOT_START + indexOfFile;
    char* block;
    blockDevice.read(blockNo, block);
    MetaData data;
    convertBlockToMetaData(&data, block);       //TODO: Pls check deferensation, (and spelling).
    return data;
}

//Sets the MetaData of an file, which is refrenced via the index.
void setMetaData(MetaData metaData, u_int8_t indexOfFile) {
    u_int32_t blockNo = ROOT_START + indexOfFile;
    char* block;
    convertBlockToMetaData(&metaData, block);   //TODO: Pls check deferensation, (and spelling).
    blockDevice.write(blockNo, block);
}

//Casts the given block to a given MetaData instance
void convertBlockToMetaData(MetaData* data, char* block){
    //TODO: Implement this.
}

//Converts the given MetaData into a block, formatted as char*/String
void convertMetaDataToBlock(MetaData* data, char* block){
    //TODO: Implement this.
}

//Gets the block and the byte number of the given address
void getBLockFromAddress(u_int32_t address, u_int32_t* blockNo, u_int32_t* byteNo) {
    //TODO: Check whether needed
    *blockNo = address / BLOCK_SIZE;
    *byteNo = address % BLOCK_SIZE;
}

//MARK: - FAT
/* creates the FAT and initialises it with default values
 *  meaning behind values:
 * -2 = systemdata (unavailable)
 * -1 = terminator character
 *  0 = free
 */
void initializeFAT(){
    for (int32_t i = 0; i < systemdata; i++){
        fat[i] = -2;
    }
    for (int32_t i = systemdata; i < datablocks; i++){
        fat[i] = 0;
    }
}


/* returns a list of all the datablocks used for a specific file in order
*  param: firstBlock: first datablock of a file
*         list: pointer to a list 
*/
int iterateFAT(int firstBlock, std::list<int>* list){
    int32_t nextBlock = firstBlock;
    std::list<int> fileList; //creates a list to store all datablocks of a specific file
    fileList.push_back(nextBlock);
    while (fat[nextBlock] != -1){
        fileList.push_back(fat[nextBlock]);
        nextBlock = fat[nextBlock];
    }
    *list = fileList;
    return 0;
}

//add the next address of a file to the FAT
int addToFAT(int32_t firstBlock, int32_t nextAddress){
    std::list<int> *fatList;
    iterateFAT(firstBlock, fatList);
    fat[fatList->back()] = nextAddress;
}

//add the last block of a file to FAT
void addLastToFAT(int32_t lastAddress){
    fat[lastAddress] = -1;
}

//MARK: - Superblock
//Returns the Superblock for the data system
void getSuperBlock(SuperBlock* superblock){
    //    TODO: Implement this.
    char* block = (char*) malloc(BLOCK_SIZE);
    blockDevice.read(0, block);
    
    uint32_t* data = (uint32_t*) block;
    superblock->fileSystemSize = *data;
    data++;
    superblock->emptySpaceSize = *data;
    data++;
    superblock->maximumStorageSize = *data;
    data++;
    superblock->dmapStart = *data;
    data++;
    superblock->fatStart = *data;
    data++;
    superblock->rootStart = *data;
    data++;
    superblock->dataStart = *data;
    data++;
    superblock->dmapSize = *data;
    data++;
    superblock->fatSize = *data;
    data++;
    superblock->rootSize = *data;
    data++;
    superblock->dataSize = *data;
    free(data);
}

//Sets the emptySpaceSize arttibute in the superblock
void setEmptySpaceSizeInSuperBlock(uint32_t emptySpaceSize){
    char* block = (char*) malloc(BLOCK_SIZE);
    uint32_t* data = (uint32_t*) block;
    
    data++;
    *data = emptySpaceSize;
    
    blockDevice.write(0, block);
    free(data);
}

//Writes the part of the filesystem for the superblock for the first time. In particular: The not changing values are written.
void createSuperBlock(struct SuperBlock superBlock){
    char* block = (char*) malloc(BLOCK_SIZE);
    uint32_t* data = (uint32_t*) block;
    
    *data = superBlock.fileSystemSize;
    data++;
    *data = superBlock.emptySpaceSize;
    data++;
    *data = superBlock.maximumStorageSize;
    data++;
    *data = superBlock.dmapStart;
    data++;
    *data = superBlock.fatStart;
    data++;
    *data = superBlock.rootStart;
    data++;
    *data = superBlock.dataStart;
    data++;
    *data = superBlock.dmapSize;
    data++;
    *data = superBlock.fatSize;
    data++;
    *data = superBlock.rootSize;
    data++;
    *data = superBlock.dataSize;
    data++;
    
    blockDevice.write(0, block);
    free(data);
}
