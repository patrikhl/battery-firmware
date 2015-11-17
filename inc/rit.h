/*
 * rit.h
 *
 *  Created on: 09/11/2015
 *      Author: Nils
 */

#ifndef RIT_H_
#define RIT_H_

#include "board.h"
#include "chip.h"

#define TIMER_INTERVAL (10)

void RIT_Init();
void RIT_IRQHandler(void);
void RIT_Disable();
void RIT_Enable();

#endif /* RIT_H_ */
