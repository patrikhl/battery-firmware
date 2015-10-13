/*
 * i2c.h
 *
 *  Created on: 13/10/2015
 *      Author: P&N
 */

/* I2CM transfer record */
static I2CM_XFER_T  i2cmXferRec;
/* I2C clock is set to 1.8MHz */
#define I2C_CLK_DIVIDER         (40)
/* 100KHz I2C bit-rate */
#define I2C_BITRATE         (100000)
/* Standard I2C mode */
#define I2C_MODE    (0)
/* 7-bit I2C address of EEPROM */
#define I2C_EEPROM_ADRESS  (0b1010000)

static void Init_I2C_PinMux(void);



static void setupI2CMaster();

static void SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize);

static void ReadI2C(uint16_t page, uint8_t *data);

static void WriteI2C(uint16_t page, uint8_t *data);


