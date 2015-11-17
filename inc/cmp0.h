/*
 * cmp0.h
 *
 *  Created on: 30/10/2015
 *      Author: P&N
 */

#ifndef CMP0_H_
#define CMP0_H_

#include "board.h"
#include "rit.h"

#define CMP_NUMBER 0 // CMP 0 - 3
#define VLADDERMUL 15 // Voltage ladder value = VLADDERMUL * VREF / 31

void CMP0_Init();

void ACMP0_IRQHandler(void);

#endif /* CMP0_H_ */
