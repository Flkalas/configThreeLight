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
    hci_le_set_advertise_enable(sock, 0, 1000);
    hci_le_set_advertise_enable(sock, 1, 1000);

//    while(1){
//		num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
//		if( num_rsp < 0 ) perror("hci_inquiry");
//
//		for (i = 0; i < num_rsp; i++) {
//			ba2str(&(ii+i)->bdaddr, addr);
//			memset(name, 0, sizeof(name));
//			if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
//				name, 0) < 0)
//			strcpy(name, "[unknown]");
//			printf("%s  %s\n", addr, name);
//		}
//    }

    free( ii );
    close( sock );
    return 0;
}
