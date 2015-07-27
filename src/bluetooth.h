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

using namespace std;

int testBlue(void);

int parseAdvertise(int sock);

#endif /* BLUETOOTH_H_ */
