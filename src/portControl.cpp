/*
 * gpioControl.cpp
 *
 *  Created on: Jul 29, 2015
 *      Author: odroid
 */

#include "portControl.h"

int testGPIO(void){

	initGPIO();
	// Print GPX1 configuration register.
	printf("GPX1CON register : 0x%08x\n",*(unsigned int *)(gpioGPX + (0x0c20 >> 2)));

	// Set direction of GPX1.2 configuration register as out.
	configGPIO(PERIPHEREL_GPX,1,2,MODE_OUTPUT);
	printf("GPX1CON register : 0x%08x\n",*(unsigned int *)(gpioGPX + (0x0c20 >> 2)));

	while(1){
		// GPX1.2 High
		writeGPIO(PERIPHEREL_GPX,1,2,GPIO_SET);
		printf("GPX1DAT register : 0x%08x\n",*(unsigned int *)(gpioGPX + (0x0c24 >> 2)));
		usleep(100);
		// GPX1.2 Low
		writeGPIO(PERIPHEREL_GPX,1,2,GPIO_RESET);
		printf("GPX1DAT register : 0x%08x\n",*(unsigned int *)(gpioGPX + (0x0c24 >> 2)));
		usleep(100);
	}

	return 0;
}

int testADC(void){
	cout << getADCvalue(0) << endl;

	return 0;
}

int initGPIO(void){
	int fd = openFileDiscriptor();
	if(fd == -1){
		return -1;
	}

	gpioGPX = mapMemory(fd,GPIO_GPX_CONFIG_BASE_ADDR);
	gpioGPA = mapMemory(fd,GPIO_GPA_CONFIG_BASE_ADDR);
	gpioGPB = mapMemory(fd,GPIO_GPB_CONFIG_BASE_ADDR);

	return 0;
}

int configGPIO(int peripherel, int indexMain, int indexSub, int mode){
	if(int errorNo = checkGPIOIndex(peripherel,indexMain,indexSub)){
		printf("Error: configGPIO Out of Index 0x%1X\n",errorNo);
		return -1;
	}

	if(checkGPIOMode(mode)){
		printf("Error: configGPIO no mode like 0x%1X\n",mode);
		return -1;
	}

	volatile uint32_t* gpio = getAddrGPIO(peripherel);
	int offset = getConfigAddrOffset(peripherel, indexMain, indexSub);
	forceMemory(gpio,offset,GPIO_CONFIG_NUM_BITS,indexSub,mode);

	return 0;
}

int writeGPIO(int peripherel, int indexMain, int indexSub, int bit){
	if(int errorNo = checkGPIOIndex(peripherel,indexMain,indexSub)){
		printf("Error: setGPIO Out of Index 0x%1X\n",errorNo);
		return -1;
	}

	if(bit == 0){
		bit = 0;
	}
	else{
		bit = 1;
	}

	volatile uint32_t* gpio = getAddrGPIO(peripherel);
	int offset = getDataAddrOffset(peripherel, indexMain, indexSub);
	forceMemory(gpio,offset,GPIO_DATA_NUM_BITS,indexSub,bit);

	return 0;
}

inline bool exists_test3 (const string& name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

int getADCvalue(int index){
	if(checkADCIndex(index)){
		cout << "Error: getADCvalue Out of Index " << index << endl;
		return -1;
	}

	// /sys/ devic  es/12 d1000  0.adc /iio\  :devi ce0/i  n_vol tage0  _raw
	// /sys/devices/12d10000.adc/iio\:device0/in_voltage0_raw
	// /sys/devices/12d10000.adc/iio\:device0/in_voltage0_raw
	// /sys/devices/12d10000.adc/iio\:device0/in_voltage0_raw

	char path[64] = {0};
	sprintf(path,"/sys/devices/12d10000.adc/iio\\:device0/in_voltage%d_raw",index);
	cout << "path: " << path << endl;

//	sprintf(path,"~/../../sys/devices/12d10000.adc/iio\\:device0/in_voltage%d_raw",index);
//	cout << "path: " << path << endl;
//
//	sprintf(path,"/home/odroid/git/configThreeLight/Debug/a");
//	cout << "path: " << path << endl;

	cout << "file exist: " << exists_test3(path) << endl;
	cout << "exam exist: " << exists_test3("./a") << endl;

	FILE* fd = NULL;

	fd = fopen(path, "r");
	if(fd == NULL){
		cout << "File open error: " << fd << endl;
		return -1;
	}

	char result[LEN_RESULT_STR] = {0};
	int valResult = 0;
	if(fgets(result, LEN_RESULT_STR, fd) > 0){
		 valResult = atoi(result);
	}

	fclose(fd);
	return valResult;
}


int openFileDiscriptor(void){
	int fd;

	if((fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0){
		printf("Unable to open /dev/mem\n");
		return -1;
	}

	return fd;
}

unsigned int* mapMemory(int fd, int addrBase){
//	int addrBase = 0;
//
//	switch(peripherel){
//	case PERIPHEREL_GPX:
//		addrBase = GPIO_GPX_CONFIG_BASE_ADDR;
//		break;
//	case PERIPHEREL_GPA:
//		addrBase = GPIO_GPA_CONFIG_BASE_ADDR;
//		break;
//	case PERIPHEREL_GPB:
//		addrBase = GPIO_GPB_CONFIG_BASE_ADDR;
//		break;
//	default:
//		return NULL;
//	}

	unsigned int* gpio = (unsigned int*)mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, addrBase);

	if(gpio < 0){
		printf("Mmap failed.\n");
		return NULL;
	}

	return gpio;
}

int forceMemory(volatile uint32_t* base,int offset,int numbits,int indexSub,int data){
	ResetRegister(base,offset,numbits,indexSub);
	SetRegister(base,offset,numbits,indexSub,data);

	return 0;
}

int getConfigAddrOffset(int peripherel, int indexMain, int indexSub){
	switch(peripherel){
	case PERIPHEREL_GPX:
		switch(indexMain){
		case 1:
			return GPIO_GPX_1_CONFIG_ADDR_OFFSET;
		case 2:
			return GPIO_GPX_2_CONFIG_ADDR_OFFSET;
		case 3:
			return GPIO_GPX_3_CONFIG_ADDR_OFFSET;
		}
		break;
	case PERIPHEREL_GPA:
		return GPIO_GPA_2_CONFIG_ADDR_OFFSET;
	case PERIPHEREL_GPB:
		return GPIO_GPB_3_CONFIG_ADDR_OFFSET;
	}
	return 0;
}

int getDataAddrOffset(int peripherel, int indexMain, int indexSub){
	switch(peripherel){
	case PERIPHEREL_GPX:
		switch(indexMain){
		case 1:
			return GPIO_GPX_1_DATA_ADDR_OFFSET;
		case 2:
			return GPIO_GPX_2_DATA_ADDR_OFFSET;
		case 3:
			return GPIO_GPX_3_DATA_ADDR_OFFSET;
		}
		break;
	case PERIPHEREL_GPA:
		return GPIO_GPA_2_DATA_ADDR_OFFSET;
	case PERIPHEREL_GPB:
		return GPIO_GPB_3_DATA_ADDR_OFFSET;
	}
	return 0;
}

volatile uint32_t* getAddrGPIO(int peripherel){
	switch(peripherel){
	case PERIPHEREL_GPX:
		return gpioGPX;
	case PERIPHEREL_GPA:
		return gpioGPA;
	case PERIPHEREL_GPB:
		return gpioGPB;
	default:
		return NULL;
	}
}

int checkGPIOIndex(int peripherel, int indexMain, int indexSub){
	switch(peripherel){
	case PERIPHEREL_GPX:
		if(indexMain > 0 && indexMain < 4){
			switch(indexMain){
			case 1:
				if(indexSub > 1 && indexSub < 8 && indexSub != 4){
					return 0;
				}
				else{
					return OUT_OF_INDEX_SUB;
				}
			case 2:
				if(indexSub > -1 && indexSub < 2 && indexSub > 3 && indexSub < 8){
					return 0;
				}
				else{
					return OUT_OF_INDEX_SUB;
				}
			case 3:
				if(indexSub == 1){
					return 0;
				}
				else{
					return OUT_OF_INDEX_SUB;
				}
			}
		}
		return OUT_OF_INDEX_MAIN;

	case PERIPHEREL_GPA:
		if(indexMain == 2){
			if(indexSub > 5 && indexSub < 8){
				return 0;
			}
			else{
				return OUT_OF_INDEX_SUB;
			}
		}
		return OUT_OF_INDEX_MAIN;

	case PERIPHEREL_GPB:
		if(indexMain == 3){
			if(indexSub > 1 && indexSub < 4){
				return 0;
			}
			else{
				return OUT_OF_INDEX_SUB;
			}
		}
		return OUT_OF_INDEX_MAIN;
	default:
		return OUT_OF_INDEX_PERIPHEREL;
	}
}

int checkGPIOMode(int mode){
	if(mode == MODE_INPUT){
		return 0;
	}
	else if(mode == MODE_OUTPUT){
		return 0;
	}
	else if(mode == MODE_TRACE_DATA){
		return 0;
	}
	else if(mode == MODE_EXTERNAL_INTERRUPT){
		return 0;
	}
	return NONE_MODE;
}

int checkADCIndex(int index){
	if(index != 0 && index != 3){
		return true;
	}
	return false;
}