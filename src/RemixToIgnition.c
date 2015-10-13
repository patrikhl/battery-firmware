/*
===============================================================================
 Name        : RemixToIgnition.c
 Author      : P&N
 Version     : 0.1
 Description : main definition
===============================================================================
*/

#include "board.h"
#include "i2c.h"
#include <cr_section_macros.h>
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {
    // read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();

    /*****************************************************************************
     * I2C init
     ****************************************************************************/

    /* Setup I2C pin muxing */
    Init_I2C_PinMux();

    /* Allocate I2C handle, setup I2C rate, and initialize I2C clocking */
    setupI2CMaster();

    /* Disable the interrupt for the I2C */
    NVIC_DisableIRQ(I2C0_IRQn);

    /*****************************************************************************
     * I2C init
     *****************************************************************************/

  	/* Loop forever */
  	while (1);


    return 0;
}
