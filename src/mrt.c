/*
 * mrt.c
 *
 *  Created on: 10/11/2015
 *      Author: P&N
 */

#include "mrt.h"

void MRT_Init()
{
	LPC_MRT_CH_T *pMRT;

	/* Get pointer to timer 0 */
	pMRT = Chip_MRT_GetRegPtr(0);

	Chip_MRT_Init();

	//Chip_MRT_IntClear(pRMT);

	/* Setup timer with rate based on MRT clock. MRT_INTVAL_LOAD will load the interval immediately */
	Chip_MRT_SetInterval(pMRT, (16777215) |
						 MRT_INTVAL_LOAD);

	/* Timer mode */
	Chip_MRT_SetMode(pMRT, MRT_MODE_ONESHOT);

	NVIC_EnableIRQ(MRT_IRQn);
	/* Clear pending interrupt and enable timer */
	Chip_MRT_IntClear(pMRT);
	Chip_MRT_SetEnabled(pMRT);

	uint32_t int_pend;

	/* Get and clear interrupt pending status for all timers */
	int_pend = Chip_MRT_GetIntPending();
	Chip_MRT_ClearIntPending(int_pend);
}

void MRT_IRQHandler(void)
{
	uint32_t int_pend;

	Board_LED_Toggle(1);
	/* Get and clear interrupt pending status for all timers */
	int_pend = Chip_MRT_GetIntPending();
	Chip_MRT_ClearIntPending(int_pend);
	Chip_MRT_SetInterval(Chip_MRT_GetRegPtr(0), (/*Chip_Clock_GetSystemClockRate()*29*/ 16777215) |
						 MRT_INTVAL_LOAD);

	Chip_MRT_SetEnabled(Chip_MRT_GetRegPtr(0));
}
