/*
 * sysTick.c
 *
 *  Created on: 09/11/2015
 *      Author: P&N
 */
#include "sysTick.h"


void sysTick_Init()
{
	uint32_t sysTickRate;

	Chip_Clock_SetSysTickClockDiv(1);

	sysTickRate = Chip_Clock_GetSysTickClockRate();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(sysTickRate / TICKRATE_HZ);
}

void SysTick_Handler(void)
{
	Board_LED_Toggle(0);
}
