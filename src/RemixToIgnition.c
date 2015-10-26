/*
===============================================================================
 Name        : RemixToIgnition.c
 Author      : P&N
 Version     : 0.1
 Description : main definition
===============================================================================
*/

#include "board.h"
//#include "i2c.h"
#include "adc.h"
#include "rtc.h"
#include <cr_section_macros.h>

#define adcChannel 0
#define TICKRATE_HZ (100)
static bool sequence0Complete;

// TODO: insert other include files here

// TODO: insert other definitions and declarations here


//void ADC0A_IRQHandler(void)
//{
//	uint32_t pending;
//
//	/* Get pending interrupts */
//	pending = Chip_ADC_GetFlags(LPC_ADC0);
//
//	/* Sequence A completion interrupt */
//	if (pending & ADC_FLAGS_SEQA_INT_MASK) {
//		sequence0Complete = true;
//	}
//
//	/* Clear any pending interrupts */
//	Chip_ADC_ClearFlags(LPC_ADC0, pending);
//}

int main(void) {
    // read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();

    //  I2C_Init();

    //   ADC_Init();

    RTC_Init();


    uint32_t time;

    while(1)
    {
    	time = getTime();

    }



    return 0;
}
