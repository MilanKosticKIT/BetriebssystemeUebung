//FAT
void initializeFAT();
int iterateFAT(int firstBlock, std::list<int>* list);
int addToFAT(int32_t firstBlock, int32_t nextAddress);
void addLastToFAT(int32_t lastAddress);


#include <stdio.h>
#include <stdlib.h>
#include <fat.h>


FAT::FAT() {
	int32_t fat[SYSTEM_BLOCKS];
	for (int32_t i = 0; i < SYSTEM_BLOCKS; i++) {
		fat[i] = -2;
	}
	for (int32_t i = SYSTEM_BLOCKS; i < DATA_BLOCKS; i++) {
		fat[i] = 0;
	}
}




//MARK: - FAT
/* creates the FAT and initialises it with default values
*  meaning behind values:
* -2 = systemdata (unavailable)
* -1 = terminator character
*  0 = free
*/



/* returns a list of all the datablocks used for a specific file in order
*  param: firstBlock: first datablock of a file
*         list: pointer to a list
*/
int FAT::iterateFAT(int firstBlock, std::list<int>* list) {
	int32_t nextBlock = firstBlock;
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
int FAT::addToFAT(int32_t firstBlock, int32_t nextAddress) {
	std::list<int> *fatList;
	iterateFAT(firstBlock, fatList);
	fat[fatList->back()] = nextAddress;
}

//add the last block of a file to FAT
void FAT::addLastToFAT(int32_t lastAddress) {
	fat[lastAddress] = -1;
}