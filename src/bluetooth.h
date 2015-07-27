/*
 * bluetooth.h
 *
 *  Created on: Jul 26, 2015
 *      Author: odroid
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#include <iostream>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "iBeaconInfo.h"

#define BUFFER_SIZE 255

using namespace std;

int testBlue(void);

int watchBeacon(void);

int performBeacon(void);

int openBlueSocket(void);
int parseAdvertise(int sock, iBeaconInfo* listBeaconInfo, int numBeaconInfo=0);
int printBeaconInfo(iBeaconInfo* listBeaconInfo, int numBeaconInfo);

#endif /* BLUETOOTH_H_ */
