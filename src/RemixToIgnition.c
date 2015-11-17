/*
===============================================================================
 Name        : RemixToIgnition.c
 Author      : P&N
 Version     : 0.1
 Description : main definition
===============================================================================
*/

#include "board.h"
#include "stdbool.h"
#include "cmp0.h"
#include "i2c.h"
#include "adc.h"
#include "rtc.h"
#include "deepSleep.h"
#include "rit.h"
#include "mrt.h"
#include "fuelgauge.h"

bool ritint = false;
bool newDataHeader = true;
uint8_t BatteryCapacity = 1;


#include <cr_section_macros.h>

int main(void)
{
//Init peripherals
    SystemCoreClockUpdate();
    Board_Init();
    I2C_Init();
    RTC_Init();
    ADC_Init();
    DeepSleep_Init();
    CMP0_Init();
    RIT_Init();
//    MRT_Init();
    PININT_Init();
    FuelGauge_Init();

	// declare variables
	uint16_t voltageC1;
	uint16_t voltageC2;
	uint16_t voltageC3;
	uint16_t voltageC4;

	uint16_t voltageBufC1;
	uint16_t voltageBufC2;
	uint16_t voltageBufC3;
	uint16_t voltageBufC4;

	uint32_t time;
	uint8_t dataSize = 0;
	uint8_t bytesOnPage = 2;
	uint8_t data[128];
	uint8_t sizeIndex=0;

	uint16_t EEPROM_pageNr = 0;

	// set RTC 1 Hz Alarm to 60 sec
    Chip_RTC_SetAlarm(LPC_RTC, Chip_RTC_GetCount(LPC_RTC) + 10);
	SCT_StartCounter();


	while(1)
	{

		//uint32_t clock = Chip_Clock_GetSystemClockRate();

		// TO DO TEST CODE FOR FUELGAUGE

		/* Create data header */
		if(newDataHeader)
		{
			Board_LED_Set(1, false);
			Board_LED_Set(2, true);
			if(bytesOnPage > 0)
				ReadI2C(EEPROM_pageNr, data);

			time = getTime();

			voltageC1 = getCellVoltage(1);
			voltageC2 =	getCellVoltage(2);
			voltageC3 = getCellVoltage(3);
			voltageC4 =	getCellVoltage(4);

			sizeIndex = bytesOnPage;
			bytesOnPage++;

			data[bytesOnPage++] = voltageC1;
			data[bytesOnPage++] = ((voltageC1 >> 8) | (voltageC2 << 2));
			data[bytesOnPage++] = ((voltageC2 >> 6) | (voltageC3 << 4));
			data[bytesOnPage++] = ((voltageC3 >> 4) | (voltageC4 << 6));
			data[bytesOnPage++] = (voltageC4 >> 2);

			data[bytesOnPage++] = time;
			data[bytesOnPage++] = (time >> 8);
			data[bytesOnPage++] = (time >> 16);
			data[bytesOnPage++] = (time >> 24);

			dataSize = 5;
			newDataHeader = false;
		}

		if(ritint && Chip_ACMP_GetCompStatus(LPC_CMP, CMP_NUMBER))
		{
			Board_LED_Set(2, false);
			Board_LED_Toggle(1);

			voltageBufC1 = getCellVoltage(1);
			voltageBufC2 = getCellVoltage(2);
			voltageBufC3 = getCellVoltage(3);
			voltageBufC4 = getCellVoltage(4);

			data[bytesOnPage++] = (((voltageC1 - voltageBufC1) & 0xF) | ((voltageC2 - voltageBufC2) << 4));
			data[bytesOnPage++] = (((voltageC3 - voltageBufC3) & 0xF) | ((voltageC4 - voltageBufC4) << 4));

			dataSize +=2;

			voltageC1 = voltageBufC1;
			voltageC2 = voltageBufC2;
			voltageC3 = voltageBufC3;
			voltageC4 = voltageBufC4;

			ritint = false;
		}

		if(bytesOnPage >= 128)
		{
			data[sizeIndex] = dataSize;
			//WriteI2C(EEPROM_pageNr, data);

			EEPROM_pageNr = (EEPROM_pageNr > 1023 ? 0 : EEPROM_pageNr+1);

			if(bytesOnPage > 128)
				memcpy(data, &data[129], 16);	//kan muligvis optimeres

			bytesOnPage = bytesOnPage - 128;
			dataSize = 0;
			newDataHeader = true;
		}


		if(!Chip_ACMP_GetCompStatus(LPC_CMP, CMP_NUMBER))
		{
			if(bytesOnPage > 0)
			{
				data[sizeIndex] = dataSize;
				//WriteI2C(EEPROM_pageNr, data);
			}

			NVIC_EnableIRQ(RTC_ALARM_IRQn);

			newDataHeader = true;

			/* Set new alarm to 60 seconds from now */
			Chip_RTC_SetAlarm(LPC_RTC, Chip_RTC_GetCount(LPC_RTC) + 10);
			//RIT_Disable();
			NVIC_EnableIRQ(PIN_INT0_IRQn);
			deepSleep();
		}
	}
	return 0;
}
