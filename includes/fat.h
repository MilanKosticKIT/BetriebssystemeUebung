//fat header

#ifndef fat_h
#define fat_h

#include <stdio.h>
#include <stdint.h>
#include <cstdint>



class FAT {
private:
	int32_t fat[];

public:
	FAT();
    int iterateFAT(int firstBlock, list<int>* list);
	int addToFAT(int32_t firstBlock, int32_t nextAddress);
	void addLastToFAT(int32_t lastAddress);

};

#endif 
