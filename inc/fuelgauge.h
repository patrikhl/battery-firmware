/*
 * fuelgauge.h
 *
 *  Created on: 12/11/2015
 *      Author: P&N
 */

#ifndef FUELGAUGE_H_
#define FUELGAUGE_H_

#include "board.h"
#include "deepSleep.h"

#define LED_PORT 0
#define PORT_MASK 	((1 << 16) | (1 << 12) | (1 << 28) | (1 << 0) | (1 << 24))
#define FUEL_STATE1	((1 << 16) | (1 << 12) | (1 << 28) | (1 << 0) | (1 << 24))
#define FUEL_STATE2	((1 << 16) | (1 << 12) | (1 << 28) | (1 << 0))
#define FUEL_STATE3	((1 << 16) | (1 << 12) | (1 << 28))
#define FUEL_STATE4	((1 << 16) | (1 << 12))
#define FUEL_STATE5	((1 << 16))
#define EVENT_CONTROL ((1 << 21) | (1 << 20) | (1 << 12))
#define MATCHVALUE 360000000 // 5 * main clk

void FuelGauge_Init();

void fuelGauge(uint8_t capacity);

void SCT_StartCounter();

void SCT0_IRQHandler(void);

#endif /* FUELGAUGE_H_ */

