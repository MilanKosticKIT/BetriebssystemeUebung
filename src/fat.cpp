//FAT

#include "constants.h"
#include "fat.h"


FAT::FAT() {
    for (int i = 0; i < DATA_BLOCKS; i++) {
        fat[i] = FAT_TERMINATOR;
    }
}

FAT::~FAT() {
    delete[] fat;
}


//MARK: - FAT
/* creates the FAT and initialises it with default values
 *  meaning behind values:
 *  FAT_TERMINATOR = terminator character
 *  0 = free
 */



/* returns a list of all the datablocks used for a specific file in order
 *  param: firstBlock: first datablock of a file
 *         list: pointer to a list
 */
int FAT::iterateFAT(uint16_t firstBlock, std::list<uint16_t>* list) {
    uint16_t nextBlock = firstBlock;
    std::list<uint16_t> fileList; //creates a list to store all datablocks of a specific file
    fileList.push_back(nextBlock);
    while (fat[nextBlock] != FAT_TERMINATOR) {
        fileList.push_back(fat[nextBlock]);
        nextBlock = fat[nextBlock];
    }
    *list = fileList;
    return 0;
}

int FAT::deleteFromFAT(uint16_t firstBlock){
    
    std::list<uint16_t> fatList;
    iterateFAT(firstBlock, &fatList);
    std::list<uint16_t >::const_iterator iterator;
    for (iterator = fatList.begin(); iterator != fatList.end(); ++iterator){
        fat[*iterator] = FAT_TERMINATOR;
    }
    
    return 0;
    
    
}




//add the next address of a file to the FAT
int FAT::addToFAT(uint16_t firstBlock, uint16_t nextAddress) {
    std::list<uint16_t> fatList;
    iterateFAT(firstBlock, &fatList);
    fat[fatList.back()] = nextAddress;
    return 0;
}

int FAT::addNextToFAT(uint16_t currentLastBlock, uint16_t nextBlock) {
    if (!(currentLastBlock == nextBlock)){
        fat[currentLastBlock] = nextBlock;
    }
    return 0;
}

//add the last block of a file to FAT
void FAT::addLastToFAT(uint16_t lastAddress) {
    fat[lastAddress] = FAT_TERMINATOR;
}

void FAT::setAll(uint16_t* p){
    for (int i = 0; i < DATA_BLOCKS; i++){
        fat[i] = *(p + i);
    }
}

void FAT::getAll(uint16_t* p){
    for (int i = 0; i < DATA_BLOCKS; i++){
        *(p + i) = fat[i];
    }
}

int FAT::getNext(uint16_t currentBlock, uint16_t* nextBlock) {
    if(currentBlock == FAT_TERMINATOR) return -1;
    *nextBlock = fat[currentBlock];
    return 0;
}
