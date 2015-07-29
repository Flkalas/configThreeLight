/*
 * bluetooth.cpp
 *
 *  Created on: Jul 26, 2015
 *      Author: odroid
 */

#include "bluetooth.h"

int testBlue(void){
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    hci_le_set_scan_parameters(sock, 0x01, htobs(0x0010), htobs(0x0010), 0x00, 0x00, 2000);
    hci_le_set_scan_enable(sock, 0x01, 0, 1000);
//    hci_le_set_advertise_enable(sock, 0, 1000);
//    hci_le_set_advertise_enable(sock, 1, 1000);

    while(1){
		num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
		if( num_rsp < 0 ) perror("hci_inquiry");

		for (i = 0; i < num_rsp; i++) {
			ba2str(&(ii+i)->bdaddr, addr);
			memset(name, 0, sizeof(name));
			if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
				name, 0) < 0)
			strcpy(name, "[unknown]");
			printf("%s  %s\n", addr, name);
		}
    }

    free( ii );
    close( sock );
    return 0;
}

int testBeaconOnOff(void){
	int sock = startBeacon();
//	for(int var = 0; var < 10; ++var) {
//		sleep(1);
//		cout << var << endl;
//	}
//	pauseBeacon(sock);
//	for(int var = 0; var < 10; ++var) {
//		sleep(1);
//		cout << var << endl;
//	}
//	resumeBeacon(sock);
//	for(int var = 0; var < 10; ++var) {
//		sleep(1);
//		cout << var << endl;
//	}
//	pauseBeacon(sock);
//	close(sock);

	return 0;
}

int watchBeacon(void){
	while(1){
		searchBeacon();
		cout << "------------------------------------------------" << endl;
	}

	return 0;
}

int searchBeacon(void){
	iBeaconInfo* listBeaconInfo = NULL;
	int sock = openBlueSocket();

	int numInfo = parseAdvertise(sock,listBeaconInfo);

	listBeaconInfo = (iBeaconInfo*)calloc(numInfo,sizeof(iBeaconInfo));
	parseAdvertise(sock,listBeaconInfo,numInfo);
	printBeaconInfo(listBeaconInfo,numInfo);

	return 0;
}

int startBeacon(void){
	//int sock = hci_get_route(NULL);
	int sock = openBlueSocket();

	setAdvertisingParameters(sock);
	changeAdvertiseState(sock,BEACON_ENABLE);
	setAdvertisingData(sock);

	return sock;
}

int pauseBeacon(int sock){
	changeAdvertiseState(sock,BEACON_DISABLE);
	return 0;
}

int resumeBeacon(int sock){
	changeAdvertiseState(sock,BEACON_ENABLE);
	setAdvertisingData(sock);
	return 0;
}


int openBlueSocket(void){
	int devID = 0, sock = 0;

	devID = hci_get_route(NULL);
	sock = hci_open_dev(devID);

	//hci_le_set_scan_parameters(sock, 0x01, htobs(0x0010), htobs(0x0010), 0x00, 0x00, 2000);
	//hci_le_set_scan_enable(sock, 0x01, 0, 1000);

	return sock;
}

int setAdvertisingParameters(int sock){
	le_set_advertising_parameters_cp paraAdvertising;

	//A0 00 / A0 00 / 03 / 00 / 00 / 00 00 00 00 00 00 / 07 / 00
	memset(&paraAdvertising, 0, sizeof(paraAdvertising));
	paraAdvertising.min_interval = 0xA000;
	paraAdvertising.max_interval = 0xA000;
	paraAdvertising.advtype = HCI_SCODATA_PKT;
	paraAdvertising.chan_map = 0x07;

	hci_request requestHCI;
	uint8_t status;

	//hci_send_cmd(sock,OGF_LE_CTL,OCF_LE_SET_ADVERTISING_PARAMETERS,LE_SET_ADVERTISING_PARAMETERS_CP_SIZE,&paraAdvertising);
	memset(&requestHCI, 0, sizeof(requestHCI));
	requestHCI.ogf = OGF_LE_CTL;
	requestHCI.ocf = OCF_LE_SET_ADVERTISING_PARAMETERS;
	requestHCI.cparam = &paraAdvertising;
	requestHCI.clen = LE_SET_ADVERTISING_PARAMETERS_CP_SIZE;
	requestHCI.rparam = &status;
	requestHCI.rlen = 1;

	if (hci_send_req(sock, &requestHCI, 1000) < 0){
		return -1;
	}

	if (status) {
		printf("Error: setAdvertisingParameters number 0x%02X\n", status);
		return -1;
	}

	return 0;
}

int changeAdvertiseState(int sock, int enable){
	le_set_advertise_enable_cp strEnableAdvertise;

	//01
	strEnableAdvertise.enable = enable;

	hci_request requestHCI;
	uint8_t status;

	//hci_send_cmd(sock,OGF_LE_CTL,OCF_LE_SET_ADVERTISE_ENABLE,LE_SET_ADVERTISE_ENABLE_CP_SIZE,&strEnableAdvertise);
	memset(&requestHCI, 0, sizeof(requestHCI));
	requestHCI.ogf = OGF_LE_CTL;
	requestHCI.ocf = OCF_LE_SET_ADVERTISE_ENABLE;
	requestHCI.cparam = &strEnableAdvertise;
	requestHCI.clen = LE_SET_ADVERTISE_ENABLE_CP_SIZE;
	requestHCI.rparam = &status;
	requestHCI.rlen = 1;

	if (hci_send_req(sock, &requestHCI, 1000) < 0){
		return -1;
	}

	if (status) {
		printf("Error: changeAdvertiseState number 0x%02X\n", status);
		return -1;
	}

	return 0;
}

int setAdvertisingData(int sock){
	le_set_advertising_data_cp dataAdvertising;

	//1E
	dataAdvertising.length = 0x1E; // 30byte

	// 02 01 / 1A / 1A
	//FF 4C 00 02 15
	dataAdvertising.data[0] = 0x02;
	dataAdvertising.data[1] = 0x01;
	dataAdvertising.data[2] = 0x1A;
	dataAdvertising.data[3] = 0x1A;
	dataAdvertising.data[4] = 0xFF;
	dataAdvertising.data[5] = 0x4C;
	dataAdvertising.data[6] = 0x00;
	dataAdvertising.data[7] = 0x02;
	dataAdvertising.data[8] = 0x15;

	//E2 C5 6D B5 DF FB 48 D2 B0 60 D0 F5 A7 10 96 E0
	dataAdvertising.data[9] = 0xE2;
	dataAdvertising.data[10] = 0xC5;
	dataAdvertising.data[11] = 0x6D;
	dataAdvertising.data[12] = 0xB5;
	dataAdvertising.data[13] = 0xDF;
	dataAdvertising.data[14] = 0xFB;
	dataAdvertising.data[15] = 0x48;
	dataAdvertising.data[16] = 0xD2;
	dataAdvertising.data[17] = 0xB0;
	dataAdvertising.data[18] = 0x60;
	dataAdvertising.data[19] = 0xD0;
	dataAdvertising.data[20] = 0xF5;
	dataAdvertising.data[21] = 0xA7;
	dataAdvertising.data[22] = 0x10;
	dataAdvertising.data[23] = 0x96;
	dataAdvertising.data[24] = 0xE0;

	/// 00 00
	dataAdvertising.data[25] = 0x00;
	dataAdvertising.data[26] = 0x00;

	// 00 00
	dataAdvertising.data[27] = 0x00;
	dataAdvertising.data[28] = 0x00;

	// C8 / 00
	dataAdvertising.data[29] = 0xC8;

	dataAdvertising.data[30] = 0x00;

	hci_request requestHCI;
	uint8_t status;

	//hci_send_cmd(sock,OGF_LE_CTL,OCF_LE_SET_ADVERTISING_DATA,LE_SET_ADVERTISING_DATA_CP_SIZE,&dataAdvertising);
	memset(&requestHCI, 0, sizeof(requestHCI));
	requestHCI.ogf = OGF_LE_CTL;
	requestHCI.ocf = OCF_LE_SET_ADVERTISING_DATA;
	requestHCI.cparam = &dataAdvertising;
	requestHCI.clen = LE_SET_ADVERTISING_DATA_CP_SIZE;
	requestHCI.rparam = &status;
	requestHCI.rlen = 1;

	if(hci_send_req(sock, &requestHCI, 1000) < 0){
		return -1;
	}

	if(status){
		printf("Error: setAdvertisingData number 0x%02X\n", status);
		return -1;
	}

	return 0;
}

int parseAdvertise(int sock, iBeaconInfo* listBeaconInfo, int numBeaconInfo){
	int oldFilter = 0, sizeFilter = 0;
	getsockopt(sock,SOL_HCI,HCI_FILTER,&oldFilter,(socklen_t*)&sizeFilter);

	struct hci_filter filterBlue;
	hci_filter_clear(&filterBlue);
	hci_filter_all_events(&filterBlue);
	hci_filter_set_ptype(HCI_EVENT_PKT,&filterBlue);

	setsockopt(sock,SOL_HCI,HCI_FILTER,&filterBlue,sizeof(filterBlue));

	char pkt[BUFFER_SIZE] = {0};
	recv(sock,(void*)pkt,BUFFER_SIZE,0);

	int codeEvent = pkt[1];
	switch(codeEvent){
	case EVT_LE_META_EVENT:
	{
		int codeSubEvent = pkt[3];
		if(codeSubEvent == EVT_LE_ADVERTISING_REPORT){
			if(listBeaconInfo == NULL){
				return pkt[4];
			}

			for(int i = 0; i < numBeaconInfo; i++){
				listBeaconInfo[i] = iBeaconInfo(pkt+4+40*i);
			}
		}
	}
	/* no break */
	case EVT_INQUIRY_RESULT_WITH_RSSI:
	case EVT_NUM_COMP_PKTS:
	case EVT_DISCONN_COMPLETE:
		break;
	}

	return 0;
}

int printBeaconInfo(iBeaconInfo* listBeaconInfo, int numBeaconInfo){
	for(int i = 0; i < numBeaconInfo; i++){
		cout << "Beacon " << i << endl;
		cout << listBeaconInfo[i] << endl;
		if(i != numBeaconInfo-1){
			cout << endl;
		}
	}

	return 0;
}
