//FAT

#include "constants.h"
#include "fat.h"


FAT::FAT() {
	uuint16_t16_t fat[DATA_BLOCKS];
	for (uuint16_t16_t i = 0; i < DATA_BLOCKS; i++) {
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
*         list: pouint16_ter to a list
*/
uint16_t FAT::iterateFAT(uuint16_t16_t firstBlock, std::list<uint16_t>* list) {
	uuint16_t16_t nextBlock = firstBlock;
	std::list<uint16_t> fileList; //creates a list to store all datablocks of a specific file
	fileList.push_back(nextBlock);
	while (fat[nextBlock] != -1) {
		fileList.push_back(fat[nextBlock]);
		nextBlock = fat[nextBlock];
	}
	*list = fileList;
	return 0;
}

//add the next address of a file to the FAT
uint16_t FAT::addToFAT(uuint16_t16_t firstBlock, uuint16_t16_t nextAddress) {
	std::list<uint16_t> *fatList;
	iterateFAT(firstBlock, fatList);
	fat[fatList->back()] = nextAddress;
}

//add the last block of a file to FAT
void FAT::addLastToFAT(uuint16_t16_t lastAddress) {
	fat[lastAddress] = -1;
}
