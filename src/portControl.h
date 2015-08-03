/*
 * gpioControl.h
 *
 *  Created on: Jul 29, 2015
 *      Author: odroid
 */

#ifndef GPIOCONTROL_H_
#define GPIOCONTROL_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>

#include <iostream>

#define PERIPHEREL_GPX 0x0
#define PERIPHEREL_GPA 0x1
#define PERIPHEREL_GPB 0x2

#define GPIO_GPX_CONFIG_BASE_ADDR 0x13400000
#define GPIO_GPA_CONFIG_BASE_ADDR 0x14010000
#define GPIO_GPB_CONFIG_BASE_ADDR 0x14010000

#define GPIO_GPX_1_CONFIG_ADDR_OFFSET 0x0C20
#define GPIO_GPX_2_CONFIG_ADDR_OFFSET 0x0C40
#define GPIO_GPX_3_CONFIG_ADDR_OFFSET 0x0C60
#define GPIO_GPA_2_CONFIG_ADDR_OFFSET 0x0040
#define GPIO_GPB_3_CONFIG_ADDR_OFFSET 0x00C0

#define GPIO_GPX_1_DATA_ADDR_OFFSET 0x0C24
#define GPIO_GPX_2_DATA_ADDR_OFFSET 0x0C44
#define GPIO_GPX_3_DATA_ADDR_OFFSET 0x0C64
#define GPIO_GPA_2_DATA_ADDR_OFFSET 0x0044
#define GPIO_GPB_3_DATA_ADDR_OFFSET 0x00C4

#define GPIO_CONFIG_NUM_BITS 	4
#define GPIO_DATA_NUM_BITS		1

#define MODE_INPUT 					0x0
#define MODE_OUTPUT 				0x1
#define MODE_TRACE_DATA 			0x4
#define MODE_EXTERNAL_INTERRUPT 	0xF

#define GPIO_RESET 0
#define GPIO_SET 1

#define OUT_OF_INDEX_PERIPHEREL 0x1
#define OUT_OF_INDEX_MAIN 		0x2
#define OUT_OF_INDEX_SUB 		0x3

#define NONE_MODE 0x1

#define MAX_STR_LEN 64

#define ResetRegister(base,offset,numBits,indexSub) 		(*((base)+((offset)>>2)) &= ~((int)((pow(2,(numBits)))-1) << ((indexSub)*(numBits))))
#define SetRegister(base,offset,numBits,indexSub,value) 	(*((base)+((offset)>>2)) |= (value) << ((indexSub)*(numBits)))

using namespace std;

static volatile uint32_t* gpioGPX;
static volatile uint32_t* gpioGPA;
static volatile uint32_t* gpioGPB;

int testGPIO(void);
int testADC(void);

int initGPIO(void);
int configGPIO(int peripherel, int indexMain, int indexSub, int mode);
int writeGPIO(int peripherel, int indexMain, int indexSub, int bit);
int getADCvalue(int index);

int openFileDiscriptor(void);
unsigned int* mapMemory(int fd, int addrBase);
int forceMemory(volatile uint32_t* base,int offset, int numbits, int indexSub, int data);

int getConfigAddrOffset(int peripherel, int indexMain, int indexSub);
int getDataAddrOffset(int peripherel, int indexMain, int indexSub);
volatile uint32_t* getAddrGPIO(int peripherel);

int checkADCIndex(int index);
int checkGPIOIndex(int peripherel, int indexMain, int indexSub);
int checkGPIOMode(int mode);

#endif /* GPIOCONTROL_H_ */
