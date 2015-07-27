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

int watchBeacon(void){
	while(1){
		performBeacon();
		cout << "------------------------------------------------" << endl;
	}

	return 0;
}

int performBeacon(void){
	iBeaconInfo* listBeaconInfo = NULL;
	int sock = openBlueSocket();
	int numInfo = parseAdvertise(sock,listBeaconInfo);

	listBeaconInfo = (iBeaconInfo*)calloc(numInfo,sizeof(iBeaconInfo));
	parseAdvertise(sock,listBeaconInfo,numInfo);
	printBeaconInfo(listBeaconInfo,numInfo);

	return 0;
}

int openBlueSocket(void){
	int devID = 0, sock = 0;

	devID = hci_get_route(NULL);
	sock = hci_open_dev(devID);

	hci_le_set_scan_parameters(sock, 0x01, htobs(0x0010), htobs(0x0010), 0x00, 0x00, 2000);
	hci_le_set_scan_enable(sock, 0x01, 0, 1000);

	return sock;
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
