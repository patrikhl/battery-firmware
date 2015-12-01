/*
===============================================================================
 Name        : main.c
 Author      : P&N
 Version     : 0.1
 Description : usb function prototypes
===============================================================================
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
