/*
===============================================================================
 Name        : main.c
 Author      : Patrik H. Lindberg
 Version     : 0.1
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

// default includes
#include "board.h"
#include <cr_section_macros.h>
// used for usb communication
#include <stdio.h>
#include <string.h>
#include "app_usbd_cfg.h"
#include "cdc_vcom.h"

// public usb types/enumerations/variables
static USBD_HANDLE_T g_hUsb;
static uint8_t g_rxBuff[256];
const  USBD_API_T *g_pUsbApi;

/**
 * @brief	Handle interrupt from USB
 * @return	Nothing
 */
void USB_IRQHandler() {
	USBD_API->hw->ISR(g_hUsb);
}

/* Find the address of interface descriptor for given class type. */
USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass)
{
	USB_COMMON_DESCRIPTOR *pD;
	USB_INTERFACE_DESCRIPTOR *pIntfDesc = 0;
	uint32_t next_desc_adr;

	pD = (USB_COMMON_DESCRIPTOR *) pDesc;
	next_desc_adr = (uint32_t) pDesc;

	while (pD->bLength) {
		/* is it interface descriptor */
		if (pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) {

			pIntfDesc = (USB_INTERFACE_DESCRIPTOR *) pD;
			/* did we find the right interface descriptor */
			if (pIntfDesc->bInterfaceClass == intfClass) {
				break;
			}
		}
		pIntfDesc = 0;
		next_desc_adr = (uint32_t) pD + pD->bLength;
		pD = (USB_COMMON_DESCRIPTOR *) next_desc_adr;
	}

	return pIntfDesc;
}

/**
 * @brief	main routine
 * @return	Function should not exit.
 */
int main() {
	USBD_API_INIT_PARAM_T usb_param;
	USB_CORE_DESCS_T desc;
	ErrorCode_t ret = LPC_OK;
	uint32_t prompt = 0, rdCnt = 0;

    // read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();

    // usb example
    /* enable clocks */
	Chip_USB_Init();

	/* initialize USBD ROM API pointer. */
	g_pUsbApi = (const USBD_API_T *) LPC_ROM_API->pUSBD;

	/* initialize call back structures */
	memset((void *) &usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
	usb_param.usb_reg_base = LPC_USB0_BASE;
	/*	WORKAROUND for artf44835 ROM driver BUG:
		Code clearing STALL bits in endpoint reset routine corrupts memory area
		next to the endpoint control data. For example When EP0, EP1_IN, EP1_OUT,
		EP2_IN are used we need to specify 3 here. But as a workaround for this
		issue specify 4. So that extra EPs control structure acts as padding buffer
		to avoid data corruption. Corruption of padding memory doesn’t affect the
		stack/program behaviour.
	 */
	usb_param.max_num_ep = 3 + 1;
	usb_param.mem_base = USB_STACK_MEM_BASE;
	usb_param.mem_size = USB_STACK_MEM_SIZE;

	/* Set the USB descriptors */
	desc.device_desc = (uint8_t *) &USB_DeviceDescriptor[0];
	desc.string_desc = (uint8_t *) &USB_StringDescriptor[0];
	/* Note, to pass USBCV test full-speed only devices should have both
	   descriptor arrays point to same location and device_qualifier set to 0.
	 */
	desc.high_speed_desc = (uint8_t *) &USB_FsConfigDescriptor[0];
	desc.full_speed_desc = (uint8_t *) &USB_FsConfigDescriptor[0];
	desc.device_qualifier = 0;

	/* USB Initialization */
	ret = USBD_API->hw->Init(&g_hUsb, &desc, &usb_param);
	if (ret == LPC_OK) {

		/* Init VCOM interface */
		ret = vcom_init(g_hUsb, &desc, &usb_param);
		if (ret == LPC_OK) {
			/*  enable USB interrupts */
			NVIC_EnableIRQ(USB0_IRQn);
			/* now connect */
			USBD_API->hw->Connect(g_hUsb, 1);
		}

	}

	DEBUGSTR("USB CDC class based virtual Comm port example!\r\n");

	while (1) {
		// Check if host has connected and opened the VCOM port
		if ((vcom_connected() != 0)) {
			vcom_write("Hello World!!\r\n", 15);
			prompt = 1;
		}
		/*
		// Check if host has connected and opened the VCOM port
		if ((vcom_connected() != 0) && (prompt == 0)) {
			vcom_write("Hello World!!\r\n", 15);
			prompt = 1;
		}
		// If VCOM port is opened echo whatever we receive back to host.
		if (prompt) {
			rdCnt = vcom_bread(&g_rxBuff[0], 256);
			if (rdCnt) {
				vcom_write(&g_rxBuff[0], rdCnt);
			}
		}*/
		// Sleep until next IRQ happens
		__WFI();
	}

    // loop forever and ever to prevent main from exiting
    while(1);

    return 0;
}
