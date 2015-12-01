/*
 * @brief Programming API used with HID example
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

#ifndef __USB_H_
#define __USB_H_

#include "app_usbd_cfg.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _LUSB_CTRL_ {
	USBD_HANDLE_T hUsb;
	uint8_t *pTxBuf;
	uint32_t txBuffLen;
	uint8_t read;
	bool newData;
	bool dataSent;
	volatile uint8_t connected;
} LUSB_CTRL_T;

extern USBD_HANDLE_T g_hUsb;
extern LUSB_CTRL_T g_lusb;

// counter to count the number of bytes read and sent from to eeprom to the usb host
extern uint16_t readCounter;

// temp buffers to test usb communication
extern uint8_t buf1[128];
extern uint8_t buf2[128];
extern uint8_t buf3[128];

/** @ingroup EXAMPLES_USBDROM_15XX_HID_GENERIC
 * @{
 */

/**
 * @brief	Initialize USB interface.
 * @return	If found returns the address of requested interface else returns NULL.
 */
extern ErrorCode_t USB_Init();

/**
 * @brief	Check if libusbdev is connected USB host application.
 * @return	Returns non-zero value if connected.
 */
extern bool USB_Connected();

/**
 * @brief	Queue the given buffer for transmission to USB host application.
 * @param	pBuf	: Pointer to buffer to be written
 * @param	buf_len	: Length of the buffer passed
 * @return	Returns LPC_OK on success.
 */
extern ErrorCode_t USB_QueueSendReq(uint8_t *pBuf, uint32_t buf_len);

/**
 * @brief	Check if queued send is done.
 * @return	Returns length of remaining data to be sent.
 *			0 indicates transfer done.
 */
extern int32_t USB_QueueSendDone();

/**
 * @brief	Sends logged data to the host.
 * @return	Returns LPC_OK on success.
 */
extern int32_t USB_SendLoggedData();

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __USB_H_ */
