/*
 * adc.c
 *
 *  Created on: 23/10/2015
 *      Author: P&N
 */

#include "adc.h"


void ADC0A_IRQHandler(void)
{
	uint32_t pending;

	/* Get pending interrupts */
	pending = Chip_ADC_GetFlags(LPC_ADC0);

	/* Sequence A completion interrupt */
	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
		sequence0Complete = true;
	}

	/* Clear any pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC0, pending);
}

void ADC_Init(){

	/* Initialize mux to switch between battery cells */
	Mux_Init();
	/* Setup ADC for 12-bit mode and normal power */
	Chip_ADC_Init(LPC_ADC0, ADC_CR_MODE10BIT); // ADC_CR_MODE10BIT and/or ADC_CR_LPWRMODEBIT. 0 for 12 bit mode
	//Chip_ADC_Init(LPC_ADC1, ADC_CR_MODE10BIT);

	/* Setup for maximum ADC clock rate */
	Chip_ADC_SetClockRate(LPC_ADC0, ADC_MAX_SAMPLE_RATE);
	//Chip_ADC_SetClockRate(LPC_ADC1, ADC_MAX_SAMPLE_RATE);

	/* Set sample frequency to 1 MHz */
	Chip_ADC_SetDivider(LPC_ADC0, (Chip_Clock_GetSystemClockRate() / 1000000) - 1);

	Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, (ADC_SEQ_CTRL_CHANSEL(0) /*| ADC_SEQ_CTRL_CHANSEL(1) */ | ADC_SEQ_CTRL_MODE_EOS));

	/* Select pin for ADC0 channel 0 to pin0_8 */
	Chip_SWM_EnableFixedPin(SWM_FIXED_ADC0_0);

	/* Select pin for ADC0 channel 1 to pin0_7 */
	//Chip_SWM_Enable_Fixed_pin(SWM_FIXED_ADC0_1);

	/* Need to do a calibration after initialization and trim */
	Chip_ADC_StartCalibration(LPC_ADC0);

	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC0))) {}

	Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));

	/* Enable interrupt on SEQA*/
	Chip_ADC_EnableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);
	NVIC_EnableIRQ(ADC0_SEQA_IRQn);

	Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);
}

uint16_t getCellVoltage(uint8_t cellNumber)
{
	selectBatteryCell(cellNumber);

	// Insert delay 490 ns

	Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);

	uint32_t rawSample;

	while(!sequence0Complete);

	sequence0Complete = false;

	rawSample = Chip_ADC_GetDataReg(LPC_ADC0, ADCVOLTAGECH);

	return (uint16_t)(ADC_DR_RESULT(rawSample) >> 2);
}

uint16_t getCellCurrent(uint8_t cellNumber)
{
	selectBatteryCell(cellNumber);

	// Insert delay 490 ns

	Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);

	uint32_t rawSample;

	while(!sequence0Complete);

	sequence0Complete = false;

	rawSample = Chip_ADC_GetDataReg(LPC_ADC0, ADCCURRENTCH);

	return (uint16_t)(ADC_DR_RESULT(rawSample) >> 2);
}
