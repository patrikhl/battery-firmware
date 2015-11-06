/*
 * i2c.h
 *
 *  Created on: 13/10/2015
 *      Author: P&N
 */
#include "i2c.h"

void I2C_Init()
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22, IOCON_DIGMODE_EN | I2C_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, IOCON_DIGMODE_EN | I2C_MODE);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SCL);
	Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SDA);

	/* Enable I2C clock and reset I2C peripheral - the boot ROM does not do this */
	Chip_I2C_Init(LPC_I2C0);

	/* Setup clock rate for I2C */
	Chip_I2C_SetClockDiv(LPC_I2C0, I2C_CLK_DIVIDER);

	/* Setup I2CM transfer rate */
	Chip_I2CM_SetBusSpeed(LPC_I2C0, I2C_BITRATE);

	/* Enable Master Mode */
	Chip_I2CM_Enable(LPC_I2C0);

    /* Disable the interrupt for the I2C */
    NVIC_DisableIRQ(I2C0_IRQn);
}

void SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize)
{
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;

	Chip_I2CM_XferBlocking(LPC_I2C0, &i2cmXferRec);
}

void ReadI2C(uint16_t page, uint8_t *data)
{
	union{
		uint16_t p;
		uint8_t a[2];
	} address;

	address.p = page << 7;
	address.p = (address.p << 8) | (address.p >> 8);

	uint8_t blok = (page > 511 ? 1 : 0);

	/* Read EEPROM  */
	SetupXferRecAndExecute(

		/* The EEPROM I2C bus address */
		I2C_EEPROM_ADRESS | blok,

		/* Transmit the two adress bytes */
		address.a, 2,

		/* Receive data page (128 bytes) */
		data, 128);
}

void WriteI2C(uint16_t page, uint8_t *data)
{
	uint8_t data_r = 0;

	union{
		uint16_t address;
		uint8_t data[130];
	} tx;

	tx.address = page << 7;
	tx.data[2] = tx.data[0];
	tx.data[0] = tx.data[1];
	tx.data[1] = tx.data[2];
	memcpy(&tx.data[2], data, 128);

	uint8_t blok = (page > 511 ? 1 : 0);
	/* Read EEPROM  */
	SetupXferRecAndExecute(

		/* The EEPROM I2C bus address */
		I2C_EEPROM_ADRESS | blok,

		/* Transmit two address bytes and 128 data bytes*/
		tx.data, 130,

		/* Receive 0 bytes*/
		&data_r, 0);
}

