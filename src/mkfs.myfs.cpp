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

typedef struct{
char[NAME_LENGTH] name;
off_t size;
uid_t userID;
gid_t groupID;
time_t last_time;
time_t modi_time;
time_t change_time;
}fileStats;



bool checkSpace(uint64_t blockCount);
void clearPointInDMAP(u_int16_t clearAddress);
void setPointInDMAP(u_int16_t setAddress);
bool isAdressFull(u_int16_t blockNo);
int findFreeBlockDMAP(uint32_t* freeBlock);
MetaData getMetaData(u_int8_t indexOfFile);
void setMetaData(MetaData metaData, u_int8_t indexOfFile);
void convertBlockToMetaData(MetaData* data, char* block);
void convertMetaDataToBlock(MetaData* data, char* block);
void getBLockFromAddress(u_int32_t address, u_int32_t* blockNo, u_int32_t* byteNo);
void getSuperBlock(SuperBlock* superblock);
void setEmptySpaceSizeInSuperBlock(uint32_t emptySpaceSize);
void createSuperBlock(struct SuperBlock superBlock);
//TODO: Get these lines to a fitting header file.ç
BlockDevice blockDevice = *new BlockDevice();


void getStats(fileStats *status, char *filename, uint64_t *blockCount){
struct stat sb;
stat(filename,&sb);
status.name(Path::GetFileName(filename).c_str());
status.size = sb.st_size;
status.userID = geteuid(); 
status.groupID = getegid();
status.modi_time = sb.st_mtime;
status.last_time = time();
status.change_time = time();
blockCount = sb.st_blocks;
}
void updateRoot(fileStats *status){
fileStats[NUM_DIR_ENTRIES] root;




}

#include <cstring>
#include <type_traits>
template <class T> void writeDevice(std::size_t block, const T& data) {
	std::static_assert(std::is_trivially_copyable<T>::value);

	char* rawData = std::reinterpret_cast<char*>(&data);
	
	static char buffer[BLOCK_SIZE];
	std::size_t blockCount = sizeof(T) / BLOCK_SIZE;
	std::size_t currentBlock = block;
	for(; currentBlock < block + blockCount; ++currentBlock) {
		BlockDevice::write(currentBlock, rawData + ((currentBlock - block) * BLOCK_SIZE));
	}
	std::memcpy(buffer, rawData + ((currentBlock - block) * BLOCK_SIZE), sizeof(T) % BLOCK_SIZE);
	BlockDevice::write(currentBlock, buffer);
}

template <class T, std::size_t N> void writeDevice(std::size_t block, const T (&data)[N]) {
	std::static_assert(std::is_trivially_copyable<T>::value);
	std::static_assert(sizeof(T * N) <= BLOCK_SIZE);

	static char buffer[BLOCK_SIZE];
	std::memcpy(buffer, &data, sizeof(T * N));
	BlockDevice::write(block, buffer);
}

template<class T> void readDevice(std::size_t block, T* data) {
	std::static_assert(std::is_trivially_constructible<T>::value);
	std::static_assert(sizeof(T) <= BLOCK_SIZE);

	static char buffer[BLOCK_SIZE];
	BlockDevice::read(block, buffer);
	std::memcpy(data, buffer, sizeof(T));
}

template<class T> void readDevice(std::size_t block, T (*data)[N]) {
	std::static_assert(std::is_trivially_constructible<T>::value);
	std::static_assert(sizeof(T * N) <= BLOCK_SIZE);

	static char buffer[BLOCK_SIZE];
	BlockDevice::read(block, buffer);
	std::memcpy(data, buffer, sizeof(T * N));
}



int main(int argc, char *argv[]) {
    
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

//Called to check if there is enough space (blockCount) in the fileSystem
bool checkFreeSpace(uint64_t blockCount){
    SuperBlock* superblock;
    getSuperBlock(superblock);
    return (*superblock).emptySpaceSize >= blockCount;
}

//Called to say an Address got empty
void clearPointInDMAP(u_int16_t clearAddress) {
    uint32_t blockNoDMAP = DMAP_START + (clearAddress / (BLOCK_SIZE * 8));
    char* block = (char*) malloc(BLOCK_SIZE);
    blockDevice.read(blockNoDMAP, block);
    int bit = clearAddress % 8;
    int byte = clearAddress % BLOCK_SIZE;
    char* byteToChange = block + byte;
    *byteToChange &= ~(1 << (7 - bit));
    blockDevice.write(blockNoDMAP, block);
    free(block);
}

//Called to say an Address was filled
void setPointInDMAP(u_int16_t setAddress) {
    uint32_t blockNoDMAP = DMAP_START + (setAddress / (BLOCK_SIZE * 8));
    char* block = (char*) malloc(BLOCK_SIZE);
    blockDevice.read(blockNoDMAP, block);
    int bit = setAddress % 8;
    int byte = setAddress % BLOCK_SIZE;
    char* byteToChange = block + byte;
    *byteToChange |= (1 << (7 - bit));
    blockDevice.write(blockNoDMAP, block);
    free(block);
}

//Called to check wehther an address is full
bool isAdressFull(u_int16_t blockNo) {
    uint32_t blockNoDMAP = DMAP_START + (blockNo / (BLOCK_SIZE * 8));
    char* block = (char*) malloc(BLOCK_SIZE);
    blockDevice.read(blockNoDMAP, block);
    int bit = blockNo % 8;
    int byte = blockNo % BLOCK_SIZE;
    char* byteToRead = block + byte;
    bool isSet (*byteToRead & (1 << (7 - bit)));
    free(block);
    return isSet;
}

//Returns -1 when nothing found, >=0  when something found
int findFreeBlockDMAP(uint32_t* freeBlock){
    char* block = (char*) malloc(BLOCK_SIZE);
    uint8_t currentByteChar;// = (char) malloc(1);
    uint16_t currentBlock = 0;
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

//Called to get the next address (from 0 to 65535)
/*
u_int16_t getAddress(u_int16_t currentAddress) {
    u_int32_t blockNo, byteNo;
    getBLockFromAddress(currentAddress, &blockNo, &byteNo);
    blockNo += FAT_START;
    char* buffer;
    blockDevice.read(blockNo, buffer);
    u_int16_t* content = buffer;
    return content[byteNo];
}

//Called to set the address for an datablock (from 0 to 65535)
void setAddress(u_int16_t currentAddress, u_int16_t nextAddress) {
    u_int32_t blockNo, byteNo;
    getBLockFromAddress(currentAddress, &blockNo, &byteNo);
    blockNo += FAT_START;
    char* buffer;
    blockDevice.read(blockNo, buffer);
    u_int16_t* content = buffer;
    content[byteNo] = nextAddress;
    blockDevice.write(blockNo, buffer);
}*/

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
    *blockNo = address / BLOCK_SIZE;
    *byteNo = address % BLOCK_SIZE;
}


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
