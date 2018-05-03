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


int main(int argc, char *argv[]) {

    // TODO: Implement file system generation & copying of files here
    return 0;
}

//MARK: - Our Methods

//Called to say an Address got empty
void clearPointInDMAP(u_int16_t deleteAddress) {
    //TODO: Implement this.
}

//Called to say an Address was filled
void setPointInDMAP(u_int16_t filledAddress) {
    //TODO: Implement this.
}

//Called to check wehther an address is full
bool getDMAP(u_int16_t askedAddress) {
    //TODO: Implement this.
}

//Called to get the next address
u_int16_t getAddress(u_int16_t currentAddress) {
    //TODO: Implement this.
}

//Called to set the address for an datablock
void setAddress(u_int16_t currentAddress, u_int16_t nextAddress) {
    //TODO: Implement this.
}

//
MetaData getMetaData(u_int8_t indexOfFile) {
    //TODO: Implement this.
}

//Sets the
void setMetaData(MetaData metaData) {
    //TODO: Implement this.
}

