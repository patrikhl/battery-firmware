/*
 * adc.h
 *
 *  Created on: 23/10/2015
 *      Author: P&N
 */

#ifndef ADC_H_
#define ADC_H_

#include "board.h"
#include "mux.h"

#define ADCVOLTAGECH 0
#define	ADCCURRENTCH 1

bool sequence0Complete;

void ADC_Init();

void ADC0A_IRQHandler(void);

uint16_t getCellVoltage(uint8_t cellNumber);

#endif /* ADC_H_ */
