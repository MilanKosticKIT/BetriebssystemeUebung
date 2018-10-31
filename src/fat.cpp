//FAT
void initializeFAT();
uint16_t iterateFAT(int firstBlock, std::list<int>* list);
uint16_t addToFAT(uint16_t firstBlock, uint16_t nextAddress);
void addLastToFAT(uint16_t lastAddress);

#include <constants.h>
#include <stdio.h>
#include <stdlib.h>
#include <fat.h>


FAT::FAT() {
	uint16_t fat[DATA_BLOCKS];
	for (uint16_t i = 0; i < DATA_BLOCKS; i++) {
		fat[i] = 0;
	}
}




//MARK: - FAT
/* creates the FAT and initialises it with default values
*  meaning behind values:
* -1 = terminator character
*  0 = free
*/



/* returns a list of all the datablocks used for a specific file in order
*  param: firstBlock: first datablock of a file
*         list: pointer to a list
*/
uint16_t FAT::iterateFAT(uint16_t firstBlock, std::list<int>* list) {
	uint16_t nextBlock = firstBlock;
	std::list<int> fileList; //creates a list to store all datablocks of a specific file
	fileList.push_back(nextBlock);
	while (fat[nextBlock] != -1) {
		fileList.push_back(fat[nextBlock]);
		nextBlock = fat[nextBlock];
	}
	*list = fileList;
	return 0;
}

//add the next address of a file to the FAT
uint16_t FAT::addToFAT(uint16_t firstBlock, uint16_t nextAddress) {
	std::list<int> *fatList;
	iterateFAT(firstBlock, fatList);
	fat[fatList->back()] = nextAddress;
}

//add the last block of a file to FAT
void FAT::addLastToFAT(uint16_t lastAddress) {
	fat[lastAddress] = -1;
}
