//fat header

#ifndef fat_h
#define fat_h

#include "constants.h"
#include "macros.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <list>

class FAT {
private:
	uint16_t *fat = new uint16_t[DATA_BLOCKS];

public:
	FAT();
	~FAT();
	int iterateFAT(uint16_t firstBlock, std::list<uint16_t>* list);
	int deleteFromFAT(uint16_t firstBlock);
	int addToFAT(uint16_t firstBlock, uint16_t nextAddress);
	void addLastToFAT(uint16_t lastAddress);
	void setAll(uint16_t* p);
	void getAll(uint16_t* p);

};

#endif
