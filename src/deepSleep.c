/*
 * deepSleep.c
 *
 *  Created on: 03/11/2015
 *      Author: P&N
 */

#include "deepSleep.h"

void DeepSleep_Init()
{
	Chip_PMU_ClearSleepFlags(LPC_PMU, PMU_PCON_SLEEPFLAG | PMU_PCON_DPDFLAG);

	Chip_SYSCTL_SetWakeup(~(SYSCTL_SLPWAKE_IRCOUT_PD	|
	    					SYSCTL_SLPWAKE_IRC_PD 		|
							SYSCTL_SLPWAKE_ACMP0_PD		|
	    					SYSCTL_SLPWAKE_ADC0_PD 		|
	    					SYSCTL_SLPWAKE_ADC1_PD 		|
	    					SYSCTL_SLPWAKE_FLASH_PD 	|
	    					SYSCTL_SLPWAKE_SYSOSC_PD 	|
	    					SYSCTL_SLPWAKE_SYSPLL_PD));
	//Enable ACMP0 and RTC alarm to wake up system from deep sleep mode
	Chip_SYSCTL_EnableERP1PeriphWakeup(SYSCTL_ERP1_WAKEUP_ACMP0INT);
}

inline void deepSleep()
{
    LPC_PWRD_API->power_mode_configure(PMU_DEEP_SLEEP, ((PMU_PD_WDOSC | PMU_PD_BOD | PMU_PD_ACMP0 | PMU_PD_ACMP1 | PMU_PD_ACMP2 | PMU_PD_ACMP3 | PMU_PD_IREF | PMU_PD_TS) & ~PMU_PD_ACMP0));
    __WFI();
}
