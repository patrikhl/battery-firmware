/*
 * rtc.c
 *
 *  Created on: 26/10/2015
 *      Author: P&N
 */
#include "rtc.h"

void RTC_Init()
{
	 Chip_Clock_EnableRTCOsc();

	 Chip_RTC_Init(LPC_RTC);

	 Chip_RTC_DisableOptions(LPC_RTC, RTC_CTRL_SWRESET);

	 Chip_RTC_EnableOptions(LPC_RTC, RTC_CTRL_RTC_EN);

	 NVIC_EnableIRQ(RTC_ALARM_IRQn);

}

inline uint32_t getTime()
{
	return Chip_RTC_GetCount(LPC_RTC);
}

inline void setTime(uint32_t time)
{
	Chip_RTC_Disable(LPC_RTC);

	Chip_RTC_SetCount(LPC_RTC, time);

	Chip_RTC_Enable(LPC_RTC);
}

void RTC_ALARM_IRQHandler(void)
{
	/* Clear RTC 1 Hz alarm status flag */
	Chip_RTC_EnableOptions(LPC_RTC, RTC_CTRL_ALARM1HZ);
}
