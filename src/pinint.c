/*
 * pinint.c
 *
 *  Created on: 11/11/2015
 *      Author: Nils
 */

#include "pinint.h"



void PININT_Init()
{
	Chip_PININT_Init(LPC_GPIO_PIN_INT);

	/* Set pin back to GPIO (on some boards may have been changed to something
	else by Board_Init()) */
	Chip_IOCON_PinMuxSet(LPC_IOCON, PININT_INPUT_PORT, PININT_INPUT_PIN,
							 (IOCON_DIGMODE_EN | IOCON_MODE_INACT) );

	/* Configure GPIO pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, PININT_INPUT_PORT, PININT_INPUT_PIN);
	/* Enable PININT Clk */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PININT);
	/* Reset the PININT block */
	Chip_SYSCTL_PeriphReset(RESET_PININT);

	/* Configure interrupt channel for the GPIO pin in INMUX block */
	Chip_INMUX_PinIntSel(PININT_INDEX, PININT_INPUT_PORT, PININT_INPUT_PIN);

	/* Configure channel interrupt as edge sensitive and falling edge interrupt */
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
	Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));

	NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
	NVIC_EnableIRQ(PIN_INT0_IRQn);
}

void PIN_INT0_IRQHandler(void)
{
	NVIC_DisableIRQ(PIN_INT0_IRQn);
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
	fuelGauge(BatteryCapacity);
	//uint16_t adcval = getCellVoltage(1);
	BatteryCapacity +=20;
	if(BatteryCapacity > 100)
		BatteryCapacity = 1;
	NVIC_EnableIRQ(PIN_INT0_IRQn);
}


