/*
 * sysTick.h
 *
 *  Created on: 09/11/2015
 *      Author: P&N
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "board.h"
#include "chip.h"

#define TICKRATE_HZ (1) /* 1 tick per second */

void sysTick_Init();

void SysTick_Handler(void);

#endif /* SYSTICK_H_ */
