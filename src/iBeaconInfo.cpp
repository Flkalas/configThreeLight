/*
 * iBeaconInfo.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: odroid
 */

#include "iBeaconInfo.h"

iBeaconInfo::iBeaconInfo() {
	memset(this->addrBlue,0,LENGTH_BLUE_ADDR);
	memset(this->uuid,0,LENGTH_UUID);
	this->major = 0;
	this->minor = 0;
	this->powerOrigin = 0;
	this->powerRecv = 0;
}

iBeaconInfo::iBeaconInfo(char* buffer){
	this->convertAddr(buffer+3);
	this->convertUUID(buffer+19);
	this->major = buffer[35]*256+buffer[36];
	this->minor = buffer[37]*256+buffer[38];
	this->powerOrigin = (int)buffer[39]-256;
	this->powerRecv = (int)buffer[40]-256;
}

iBeaconInfo::~iBeaconInfo() {
	// TODO Auto-generated destructor stub
}

int iBeaconInfo::convertAddr(char* srcAddrStart){
	for(int i = 0; i < LENGTH_ADDR_BYTE; i++){
		sprintf(this->addrBlue+3*i,"%02X",srcAddrStart[LENGTH_ADDR_BYTE-i-1]);
		if(i != LENGTH_ADDR_BYTE-1){
			sprintf(this->addrBlue+3*i+2,":");
		}
	}
	this->addrBlue[LENGTH_BLUE_ADDR-1] = '\0';
	return 0;
}

int iBeaconInfo::convertUUID(char* srcAddrStart){
	for(int i = 0; i < LENGTH_UUID_BYTE; i++){
		sprintf(this->uuid+3*i,"%02X ",srcAddrStart[i]);
	}
	this->addrBlue[LENGTH_BLUE_ADDR-1] = '\0';
	return 0;
}

int iBeaconInfo::convertTwosComplement(char numDesibel){
	//-128 ~ 127
	int temp = (int)numDesibel-256;

	return temp;
}

ostream& operator<<(ostream& os, const iBeaconInfo& self){
    os << "Addr: " << self.addrBlue << endl <<
    		"UUID: " << self.uuid << endl <<
    		"Major: " << self.major << ", Minor: " << self.minor << endl <<
    		"OSS: " << self.powerOrigin << "dB, RSS: " << self.powerRecv << "dB";
    return os;
}

