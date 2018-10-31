//fat header

#ifndef fat_h
#define fat_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <list>

class FAT {
private:
	uint16_t fat[];

public:
	FAT();
	int iterateFAT(uint16_t firstBlock, std::list<int>* list);
	int addToFAT(uint16_t firstBlock, uint16_t nextAddress);
	void addLastToFAT(uint16_t lastAddress);

};

#endif 
