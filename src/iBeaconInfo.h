/*
 * iBeaconInfo.h
 *
 *  Created on: Jul 28, 2015
 *      Author: odroid
 */

#ifndef IBEACONINFO_H_
#define IBEACONINFO_H_

#define LENGTH_ADDR_BYTE 6
#define LENGTH_UUID_BYTE 16

#define LENGTH_BLUE_ADDR 18
#define LENGTH_UUID 48

#include <stdio.h>
#include <string.h>

#include <iostream>

using namespace std;

class iBeaconInfo {
public:
	iBeaconInfo();
	iBeaconInfo(char* buffer);
	~iBeaconInfo();

	friend ostream& operator<<(ostream& os, const iBeaconInfo& self);

	int convertAddr(char* srcAddrStart);
	int convertUUID(char* srcAddrStart);
	int convertTwosComplement(char numDesibel);

	char addrBlue[LENGTH_BLUE_ADDR];
	char uuid[LENGTH_UUID];
	int major;
	int minor;
	int powerOrigin;
	int powerRecv;
};

#endif /* IBEACONINFO_H_ */
