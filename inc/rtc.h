/*
 * rtc.h
 *
 *  Created on: 26/10/2015
 *      Author: P&N
 */
#include "board.h"

void RTC_Init();

inline uint32_t getTime();

inline void setTime(uint32_t time);

void RTC_ALARM_IRQHandler(void);
