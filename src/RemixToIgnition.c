/*
===============================================================================
 Name        : RemixToIgnition.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "board.h"
#include <cr_section_macros.h>
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {
    // read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();

    // TODO: insert code here

    // loop forever and ever
    while(1);

    return 0;
}
