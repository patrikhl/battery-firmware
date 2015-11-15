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

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* The size of the packet buffer. */
#define PACKET_BUFFER_SIZE		4096

/* Application defined LUSB interrupt status  */
#define LUSB_DATA_PENDING		_BIT(0)

/* Packet buffer for processing */
static uint8_t g_rxBuff[PACKET_BUFFER_SIZE];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main()
{
	BatteryInfo batteryInfo;
	batteryInfo.fwVersInt = 0;
	batteryInfo.fwVersDec = 1;
	strcpy(batteryInfo.ID, "BAT123");
	strcpy(batteryInfo.ID, "Battery Name");
	batteryInfo.currentTime = 1447614357;
	batteryInfo.lastSyncTime = 1447614356;
	batteryInfo.capacity = 1500;
	batteryInfo.lastFullCap = 3000;
	batteryInfo.factoryFullCap = 3000;

	// initialize board and chip
	Board_Init();

	// enable clocks
	Chip_USB_Init();

	// init USB subsystem and LibUSBDevice
	USB_Init();

	while (1) {
//		__WFI();
		while (USB_Connected()) {
			if (libusbdev_QueueReadDone() != -1) {

				/* Dummy process read data ......*/
				/* requeue read request */
				libusbdev_QueueReadReq(g_rxBuff, PACKET_BUFFER_SIZE);
			}
		}
	}
}
