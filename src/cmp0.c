/*
 * cmp0.c
 *
 *  Created on: 30/10/2015
 *      Author: Nils
 */
#include "cmp0.h"

void CMP0_Init()
{
	Chip_ACMP_Init(LPC_CMP);

	//Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 27);
	Chip_SWM_EnableFixedPin(SWM_FIXED_ACMP_I1);

	/* Disables pullups/pulldowns and disable digital mode */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 27, (IOCON_MODE_INACT | IOCON_ADMODE_EN));

	/* Positive and negative references(ACMP_I1 = PIN0_27), no hysteresis */
	Chip_ACMP_SetupACMPRefs(LPC_CMP, CMP_NUMBER, ACMP_POSIN_ACMP_I1 , ACMP_NEGIN_VREF_DIV, ACMP_HYS_NONE);

	/* Edge triggered interrupt on rising edge. First false - edge, second false - not inverted*/
	Chip_ACMP_SetupACMPInt(LPC_CMP, CMP_NUMBER, false, false, ACMP_EDGESEL_RISING);

	Chip_ACMP_EnableCompInt(LPC_CMP, CMP_NUMBER);

	/* Setup voltage ladder. VLADDERMUL to select ladder value. true for Vref = VDD or false for Vref = VDDCMP pin*/
	Chip_ACMP_SetupVoltLadder(LPC_CMP, CMP_NUMBER, VLADDERMUL, true);

	Chip_ACMP_EnableVoltLadder(LPC_CMP, CMP_NUMBER);

	/* Setup Comparator Filter register to bypass filter and use SysClk without any division */
	Chip_ACMP_SetCompFiltReg(LPC_CMP, CMP_NUMBER, ACMP_SMODE_0, ACMP_CLKDIV_1);
	/* Enable Comparator */
	Chip_ACMP_EnableComp(LPC_CMP, CMP_NUMBER);
	/* Clear interrupt */
	Chip_ACMP_ClearIntFlag(LPC_CMP, CMP_NUMBER);
	NVIC_ClearPendingIRQ(CMP0_IRQ);

	/* Enable the Interrupt for the compare output */
	NVIC_EnableIRQ(CMP0_IRQ);
}

void ACMP0_IRQHandler(void)
{
	NVIC_DisableIRQ(RTC_ALARM_IRQn);
	Chip_ACMP_ClearIntFlag(LPC_CMP, CMP_NUMBER);
	RIT_Enable();
}

