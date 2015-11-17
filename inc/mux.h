/*
 * mux.h
 *
 *  Created on: 22/10/2015
 *      Author: P&N
 */

#ifndef MUX_H_
#define MUX_H_

#include "board.h"
#include "chip.h"

#define PIN_S1	3
#define PIN_S2	25
#define MUXPORT 0


void Mux_Init();

void selectBatteryCell(uint8_t cellNumber);

#endif /* MUX_H_ */
