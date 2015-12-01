/*
===============================================================================
 Name        : main.c
 Author      : P&N
 Version     : 0.1
 Description : main definition
===============================================================================
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
