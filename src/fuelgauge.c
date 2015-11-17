/*
 * fuelgauge.c
 *
 *  Created on: 13/11/2015
 *      Author: Nils
 */

#include "fuelgauge.h"

void FuelGauge_Init()
{
	/* Initialize State configurable timer 0 to interrupt 5 seconds after it has been started */
	Chip_SCT_Init(LPC_SCT0);

	Chip_SCT_Config(LPC_SCT0, SCT_CONFIG_32BIT_COUNTER | SCT_CONFIG_AUTOLIMIT_L);

	Chip_SCT_SetCount(LPC_SCT0, 0);

	Chip_SCT_SetMatchCount(LPC_SCT0, SCT_MATCH_0, MATCHVALUE);
	Chip_SCT_SetMatchReload(LPC_SCT0, SCT_MATCH_0, MATCHVALUE);

	//set regmode
	LPC_SCT0->REGMODE = 0;

	Chip_SCT_ClearEventFlag(LPC_SCT0, SCT_EVT_0);

	LPC_SCT0->EVENT[0].CTRL = EVENT_CONTROL;
	LPC_SCT0->EVENT[0].STATE = 0xFFFFFFFF;
	Chip_SCT_EnableEventInt(LPC_SCT0, SCT_EVT_0);

	NVIC_EnableIRQ(SCT0_IRQn);

	/* Initialize GPIO pin P0_12, P0_16, P0_24, P0_0, P0_28 as output for LEDs*/
	/* Chip_GPIO_Init is called in board.h */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LED_PORT, 12);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LED_PORT, 16);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LED_PORT, 24);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LED_PORT, 0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, LED_PORT, 28);

}

void SCT_StartCounter()
{
	Chip_SCT_SetControl(LPC_SCT0, SCT_CTRL_BIDIR_L(0));
	Chip_SCT_ClearControl(LPC_SCT0, SCT_CTRL_HALT_L);
}

void SCT0_IRQHandler(void)
{
	Chip_SCT_SetControl(LPC_SCT0, SCT_CTRL_HALT_L);
	Chip_SCT_ClearEventFlag(LPC_SCT0, SCT_EVT_0);
	Chip_SCT_SetCount(LPC_SCT0, 0);
	// Turn off LED's
	Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 16, true);
	Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 12, true);
	Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 28, true);
	Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 0, true);
	Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 24, true);

	NVIC_EnableIRQ(PIN_INT0_IRQn);
}

void fuelGauge(uint8_t capacity)
{
	if(capacity > 80)
	{
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 16, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 12, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 28, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 24, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 0, false);
		//Chip_GPIO_SetPortMask(LPC_GPIO, LED_PORT, FUEL_STATE1);
	}
	else if(capacity > 60)
	{
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 16, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 12, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 28, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 24, false);
		//Chip_GPIO_SetPortMask(LPC_GPIO, LED_PORT, FUEL_STATE2);
	}
	else if(capacity > 40)
	{
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 16, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 12, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 28, false);
		//Chip_GPIO_SetPortMask(LPC_GPIO, LED_PORT, FUEL_STATE3);
	}
	else if(capacity > 20)
	{
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 16, false);
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 12, false);
		//Chip_GPIO_SetPortMask(LPC_GPIO, LED_PORT, FUEL_STATE4);
	}
	else
		Chip_GPIO_SetPinState(LPC_GPIO, LED_PORT, 16, false);
		//Chip_GPIO_SetPortMask(LPC_GPIO, LED_PORT, FUEL_STATE5);

	SCT_StartCounter();
}
