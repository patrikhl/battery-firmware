/*
 * @brief HID generic example's callabck routines
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
#include <stdint.h>
#include <string.h>
#include "usbd/usbd_rom_api.h"
#include "usb.h"
#include "batteryinfo.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

// endpoint 0 patch that prevents nested NAK event processing
static uint32_t g_ep0RxBusy = 0;		// flag indicating whether EP0 OUT/RX buffer is busy
static USB_EP_HANDLER_T g_Ep0BaseHdlr;	// variable to store the pointer to base EP0 handler

LUSB_CTRL_T g_lusb;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
const USBD_API_T *g_pUsbApi;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

// handle USB RESET event
ErrorCode_t USB_ResetEvent(USBD_HANDLE_T hUsb)
{
	// reset the control structure
	memset(&g_lusb, 0, sizeof(LUSB_CTRL_T));
	g_lusb.hUsb = hUsb;
	// set eeprom read counter to 0
	readCounter = 0;

	return LPC_OK;
}

// USB interrupt EP_IN endpoint handler (sends battery info to host)
ErrorCode_t USB_StatusIN_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) data;

	if(event == USB_EVT_IN || event == USB_EVT_IN_NAK)
	{
		// update current time
		// TODO: update current time

		// write status to host
		USBD_API->hw->WriteEP(pUSB->hUsb, USB_IN_EP1, &infoBuf.buffer, sizeof(infoBuf));
	}

	return LPC_OK;
}

// USB interrupt EP_OUT endpoint handler (reads current time from host)
ErrorCode_t USB_TimeOUT_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) data;

	// received a transfer from the USB host
	if (event == USB_EVT_OUT)
	{
		// read data from host
		USBD_API->hw->ReadEP(pUSB->hUsb, USB_OUT_EP1, (uint8_t*) &infoBuf.info.currentTime);

		// set RTC value
		// TODO: set RTC value
	}

	return LPC_OK;
}

// USB bulk EP_IN endpoint handler (sends eeprom data to the host)
ErrorCode_t USB_DataIN_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) data;

	if (event == USB_EVT_IN || event == USB_EVT_IN_NAK) {
		pUSB->txBuffLen = 0;
		pUSB->pTxBuf = 0;
		pUSB->read = 1;
	}

	return LPC_OK;
}

// handler for WCID USB device requests
ErrorCode_t WCID_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	USB_CORE_CTRL_T *pCtrl = (USB_CORE_CTRL_T *) hUsb;
	ErrorCode_t ret = ERR_USBD_UNHANDLED;

	// received a setup event from host
	if (event == USB_EVT_SETUP)
	{
		switch (pCtrl->SetupPacket.bmRequestType.BM.Type)
		{
		case REQUEST_STANDARD:
			if ((pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_DEVICE) &&
				(pCtrl->SetupPacket.bRequest == USB_REQUEST_GET_DESCRIPTOR) &&
				(pCtrl->SetupPacket.wValue.WB.H == USB_STRING_DESCRIPTOR_TYPE) &&
				(pCtrl->SetupPacket.wValue.WB.L == 0x00EE)) {
				pCtrl->EP0Data.pData = (uint8_t *) WCID_String_Descriptor;
				pCtrl->EP0Data.Count = pCtrl->SetupPacket.wLength;
				USBD_API->core->DataInStage(pCtrl);
				ret = LPC_OK;
			}
			break;

		case REQUEST_VENDOR:
			switch (pCtrl->SetupPacket.bRequest)
			{
			// libusbK benchmark test
			case 0x0E:
				pCtrl->EP0Buf[0] = 0x02;
				pCtrl->EP0Data.pData = (uint8_t *) &pCtrl->EP0Buf[0];
				pCtrl->EP0Data.Count = pCtrl->SetupPacket.wLength;
				USBD_API->core->DataInStage(pCtrl);
				ret = LPC_OK;
				break;

			// libusbK benchmark test
			case 0x11:
				pCtrl->EP0Buf[0] = 'A';
				pCtrl->EP0Buf[1] = 'B';
				pCtrl->EP0Buf[2] = 'C';
				pCtrl->EP0Data.pData = (uint8_t *) &pCtrl->EP0Buf[0];
				pCtrl->EP0Data.Count = pCtrl->SetupPacket.wLength;
				USBD_API->core->DataInStage(pCtrl);
				ret = LPC_OK;
				break;

			// libusbK benchmark test
			case 0x10:
				pCtrl->EP0Data.pData = pCtrl->EP0Buf;	/* data to be received */
				ret = LPC_OK;
				break;

			// usb product id
			case USB_PID:
				switch (pCtrl->SetupPacket.bmRequestType.BM.Recipient)
				{
				case REQUEST_TO_DEVICE:
					if (pCtrl->SetupPacket.wIndex.W == 0x0004)
					{
						pCtrl->EP0Data.pData = (uint8_t *) WCID_CompatID_Descriptor;
						pCtrl->EP0Data.Count = pCtrl->SetupPacket.wLength;
						USBD_API->core->DataInStage(pCtrl);
						ret = LPC_OK;
					}
					break;
				}
				break;
			}
		}
	}
	else if ((event == USB_EVT_OUT) && (pCtrl->SetupPacket.bmRequestType.BM.Type == REQUEST_VENDOR))
	{
		if (pCtrl->SetupPacket.bRequest == 0x10)
		{
			USBD_API->core->StatusInStage(pCtrl);
			ret = LPC_OK;
		}
	}

	return ret;
}

// EP0_patch part of WORKAROUND for artf45032
static ErrorCode_t EP0_patch(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	switch (event) {
	case USB_EVT_OUT_NAK:
		if (g_ep0RxBusy) {
			// we already queued the buffer so ignore this NAK event
			return LPC_OK;
		}
		else {
			// mark EP0_RX buffer as busy and allow base handler to queue the buffer
			g_ep0RxBusy = 1;
		}
		break;

	// reset the flag when new setup sequence starts
	case USB_EVT_SETUP:
	case USB_EVT_OUT:
		// we received the packet so clear the flag
		g_ep0RxBusy = 0;
		break;
	}
	return g_Ep0BaseHdlr(hUsb, data, event);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

// handle interrupt from USB
void USB_IRQHandler(void)
{
	USBD_API->hw->ISR(g_lusb.hUsb);
}

// initialize USB interface
int USB_Init()
{
	USBD_API_INIT_PARAM_T usb_param;
	USB_CORE_DESCS_T desc;
	ErrorCode_t ret = LPC_OK;
	USB_CORE_CTRL_T *pCtrl;
	uint32_t ep_index;

	// init USB API structure
	g_pUsbApi = (const USBD_API_T *) LPC_ROM_API->pUSBD;

	// initialize call back structures
	memset((void *) &usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
	usb_param.usb_reg_base = LPC_USB_BASE;
	usb_param.max_num_ep = USB_MAX_EP_NUM;
	usb_param.mem_base = USB_STACK_MEM_BASE;
	usb_param.mem_size = USB_STACK_MEM_SIZE;
	usb_param.USB_Reset_Event = USB_ResetEvent;

	// set the USB descriptors
	desc.device_desc = (uint8_t *) USB_DeviceDescriptor;
	desc.string_desc = (uint8_t *) USB_StringDescriptor;

	/* Note, to pass USBCV test full-speed only devices should have both
	 * descriptor arrays point to same location and device_qualifier set
	 * to 0.
	 */
	desc.high_speed_desc = (uint8_t *) USB_FsConfigDescriptor;
	desc.full_speed_desc = (uint8_t *) USB_FsConfigDescriptor;
	desc.device_qualifier = 0;

	// initialize usb hardware
	ret = USBD_API->hw->Init(&g_lusb.hUsb, &desc, &usb_param);

	// return if there was an error
	if (ret != LPC_OK)
		return ret;

	/*	WORKAROUND for artf45032 ROM driver BUG:
		Due to a race condition there is the chance that a second NAK event will
		occur before the default endpoint0 handler has completed its preparation
		of the DMA engine for the first NAK event. This can cause certain fields
		in the DMA descriptors to be in an invalid state when the USB controller
		reads them, thereby causing a hang.
	 */

	// convert the handle to control structure
	pCtrl = (USB_CORE_CTRL_T *) g_lusb.hUsb;
	// retrieve the default EP0_OUT handler
	g_Ep0BaseHdlr = pCtrl->ep_event_hdlr[0];
	// set patch routine as EP0_OUT handler
	pCtrl->ep_event_hdlr[0] = EP0_patch;

	// enable USB_IN_EP endpoint and USB_EVT_IN and USB_EVT_IN_NAK events
	USBD_API->hw->EnableEP(g_lusb.hUsb, USB_IN_EP1);
	USBD_API->hw->EnableEvent(g_lusb.hUsb, USB_IN_EP1, USB_EVT_IN, 1);
	USBD_API->hw->EnableEvent(g_lusb.hUsb, USB_IN_EP1, USB_EVT_IN_NAK, 1);

	USBD_API->hw->EnableEP(g_lusb.hUsb, USB_IN_EP2);
	USBD_API->hw->EnableEvent(g_lusb.hUsb, USB_IN_EP2, USB_EVT_IN, 1);
	USBD_API->hw->EnableEvent(g_lusb.hUsb, USB_IN_EP2, USB_EVT_IN_NAK, 1);

	// register WCID handler
	ret = USBD_API->core->RegisterClassHandler(g_lusb.hUsb, WCID_hdlr, &g_lusb);

	// return if there was an error
	if (ret != LPC_OK)
		return ret;

	// register endpoint 1 out handler
	ep_index = ((USB_OUT_EP1 & 0x0F) << 1);
	ret = USBD_API->core->RegisterEpHandler(g_lusb.hUsb, ep_index, USB_TimeOUT_Hdlr, &g_lusb);

	// return if there was an error
	if (ret != LPC_OK)
		return ret;

	// register endpoint 1 in handler
	ep_index = (((USB_IN_EP1 & 0x0F) << 1) + 1);
	ret = USBD_API->core->RegisterEpHandler(g_lusb.hUsb, ep_index, USB_StatusIN_Hdlr, &g_lusb);

	// return if there was an error
	if (ret != LPC_OK)
		return ret;

	// register endpoint 2 in handler
	ep_index = (((USB_IN_EP2 & 0x0F) << 1) + 1);
	ret = USBD_API->core->RegisterEpHandler(g_lusb.hUsb, ep_index, USB_DataIN_Hdlr, &g_lusb);

	// return if there was an error
	if (ret != LPC_OK)
		return ret;

	// enable USB interrupts
	NVIC_EnableIRQ(LPC_USB_IRQ);

	// connect
	USBD_API->hw->Connect(g_lusb.hUsb, 1);

	return ret;
}

// check if the usb is connected to a host application
bool USB_Connected()
{
	return USB_IsConfigured(g_lusb.hUsb);
}

ErrorCode_t USB_QueueSendReq(uint8_t *pBuf, uint32_t buf_len)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) &g_lusb;
	ErrorCode_t ret = ERR_FAILED;

	// check if a read request is pending
	if (pUSB->read == 1)
	{
		// queue the read request
		pUSB->pTxBuf = pBuf;
		pUSB->txBuffLen = USBD_API->hw->WriteEP(pUSB->hUsb, USB_IN_EP2, pBuf, buf_len);
		pUSB->read = 0;
		ret = LPC_OK;
	}

	return ret;
}

// check if queued send request is done
int32_t USB_QueueSendDone()
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) &g_lusb;

	// return remaining length
	return pUSB->txBuffLen;
}

// send the logged data to the host
int32_t USB_SendLoggedData()
{
	ErrorCode_t ret = LPC_OK;
	uint8_t *buf;

	for(; readCounter < infoBuf.info.eepromPages; readCounter++)
	{
		// read data from eeprom

		// test data
		switch(readCounter)
		{
		case 0:
			buf = buf1;
			break;
		case 1:
			buf = buf2;
			break;
		case 2:
			buf = buf3;
			break;
		}

		// send data
		do
		{
			ret = USB_QueueSendReq(buf, 128);
		} while(ret != LPC_OK);

		// wait for transmission to complete
		while(!USB_QueueSendDone());
	}

	// clear eeprom ? - kan evt. gøres i et write ep
	// TODO: clear eeprom

	// reset new data indicator and set data sent indicator
	g_lusb.newData = false; // TODO: set this somewhere
	g_lusb.dataSent = true;

	return LPC_OK;
}
