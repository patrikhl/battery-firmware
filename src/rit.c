/*
 * rit.c
 *
 *  Created on: 09/11/2015
 *      Author: Nils
 */

#include "rit.h"

extern bool ritint;

void RIT_Init()
{
	/* Initialize RITimer */
	Chip_RIT_Init(LPC_RITIMER);

	/* Configure RIT for a periodic interrupt tick rate */
	Chip_RIT_SetTimerIntervalHz(LPC_RITIMER, TIMER_INTERVAL);
//	Chip_RIT_EnableDebug(LPC_RITIMER);
	Chip_RIT_Enable(LPC_RITIMER);

	NVIC_EnableIRQ(RITIMER_IRQn);
}
void RIT_IRQHandler(void)
{
	/* Clear interrupt */
	ritint = true;
	Chip_RIT_ClearIntStatus(LPC_RITIMER);
	//Board_LED_Toggle(0);
}

inline void RIT_Disable()
{
	Chip_RIT_Disable(LPC_RITIMER);
}

inline void RIT_Enable()
{
	Chip_RIT_Enable(LPC_RITIMER);
}
