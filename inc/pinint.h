/*
 * pinint.h
 *
 *  Created on: 11/11/2015
 *      Author: P&N
 */

#ifndef PININT_H_
#define PININT_H_

#include "board.h"
#include "fuelgauge.h"
#include "adc.h""

#define PININT_INPUT_PIN 0
#define PININT_INPUT_PORT 1
#define PININT_INDEX   0

extern uint8_t BatteryCapacity;

void PININT_Init();

void PIN_INT0_IRQHandler(void);

#endif /* PININT_H_ */
