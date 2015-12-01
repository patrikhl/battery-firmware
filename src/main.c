/*
 * @brief HID generic example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "app_usbd_cfg.h"
#include "usb.h"
#include "batteryinfo.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* The size of the packet buffer. */
#define PACKET_BUFFER_SIZE		64//4096

/* Application defined LUSB interrupt status  */
#define LUSB_DATA_PENDING		_BIT(0)

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
// global battery info buffer
BatteryInfoBuf infoBuf;

// counter to count the number of bytes read and sent from to eeprom to the usb host
uint16_t readCounter = 0;

// temp buffers to test usb communication
uint8_t buf1[128];
uint8_t buf2[128];
uint8_t buf3[128];


/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine
 * @return	Function should not exit.
 */
int main()
{
	BatteryInfo batteryInfo;
	batteryInfo.fwVersInt = 0;
	batteryInfo.fwVersDec = 1;
	strcpy(batteryInfo.ID, "BAT123");
	memset(batteryInfo.name, 0, NAME_LENGTH);
	strcpy(batteryInfo.name, "Random Name Here");
	batteryInfo.currentTime = 1447614357;
	batteryInfo.lastSyncTime = 1447614356;
	batteryInfo.capacity = 1500;
	batteryInfo.lastFullCap = 3000;
	batteryInfo.factoryCap = 3000;
	batteryInfo.eepromPages = 3;

	// assign batteryInfo to global info buffer
	infoBuf.info = batteryInfo;

	// initialize board and chip
	Board_Init();

	// enable clocks
	Chip_USB_Init();

	// init USB subsystem and LibUSBDevice
	USB_Init();

	// set usb test buffers
	uint8_t i = 0;
	for(i=0; i<128; i++) {
		buf1[i] = i + 1;
		buf2[i] = i + 2;
		buf3[i] = i + 3;
	}

	while(1) {
		while(USB_Connected()) {
			// send new data
			if(!g_lusb.dataSent)
				USB_SendLoggedData();
		}
	}
}
