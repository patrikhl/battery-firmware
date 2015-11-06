/*
 * adc.h
 *
 *  Created on: 23/10/2015
 *      Author: P&N
 */

#include "board.h"
#include "mux.h"

#define adcChannel 0

bool sequence0Complete;

void ADC_Init();

void ADC0A_IRQHandler(void);

uint16_t getCellVoltage(uint8_t cellNumber);



