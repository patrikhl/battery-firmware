/*
 * i2c.h
 *
 *  Created on: 13/10/2015
 *      Author: P&N
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "board.h"
#include "string.h"

/* I2CM transfer record */
I2CM_XFER_T  i2cmXferRec;
/* I2C clock is set to 1.8MHz */
#define I2C_CLK_DIVIDER         (40)
/* 100KHz I2C bit-rate */
#define I2C_BITRATE         (100000)
/* Standard I2C mode */
#define I2C_MODE    (0)
/* 7-bit I2C address of EEPROM */
#define I2C_EEPROM_ADRESS  (0b1010000)

void I2C_Init();

void SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize);

void ReadI2C(uint16_t page, uint8_t *data);

void WriteI2C(uint16_t page, uint8_t *data);

#endif /* I2C_H_ */

