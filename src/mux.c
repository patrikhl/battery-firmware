/*
 * mux.c
 *
 *  Created on: 22/10/2015
 *      Author: P&N
 */
#include "mux.h"

void Mux_Init()
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, PIN_S1);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, PIN_S2);
}

void selectBatteryCell(uint8_t cellNumber)
{
	switch(cellNumber){
		case 1:
			Chip_GPIO_SetPinState(LPC_GPIO, MUXPORT, PIN_S1, false);
			Chip_GPIO_SetPinState(LPC_GPIO, MUXPORT, PIN_S2, false);
			break;
		case 2:
			Chip_GPIO_SetPinState(LPC_GPIO, MUXPORT, PIN_S1, true);
			Chip_GPIO_SetPinState(LPC_GPIO, MUXPORT, PIN_S2, false);
			break;
		case 3:
			Chip_GPIO_SetPinState(LPC_GPIO, MUXPORT, PIN_S1, false);
			Chip_GPIO_SetPinState(LPC_GPIO, MUXPORT, PIN_S2, true);
			break;
		case 4:
			Chip_GPIO_SetPinState(LPC_GPIO, MUXPORT, PIN_S1, true);
			Chip_GPIO_SetPinState(LPC_GPIO, MUXPORT, PIN_S2, true);
			break;
	}

}
