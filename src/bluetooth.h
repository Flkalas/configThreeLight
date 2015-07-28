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
#include <string.h>
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

int searchBeacon(void);
int startBeacon(void);

int openBlueSocket(void);
int enableAdvertise(int sock);
int setAdvertisingParameters(int sock);
int setAdvertisingData(int sock);
int parseAdvertise(int sock, iBeaconInfo* listBeaconInfo, int numBeaconInfo=0);
int printBeaconInfo(iBeaconInfo* listBeaconInfo, int numBeaconInfo);

#endif /* BLUETOOTH_H_ */
