/*
 * I2Cpressuresensor.cpp
 *
 *  Created on: 28.2.2020
 *      Author: johan
 */

#include "I2Cpressuresensor.h"

I2C_pressure_sensor::I2C_pressure_sensor() {
	// TODO Auto-generated constructor stub

}

I2C_pressure_sensor::~I2C_pressure_sensor() {
	// TODO Auto-generated destructor stub
}

static volatile std::atomic_int counter;


/* I2CM transfer record */
static I2CM_XFER_T  i2cmXferRec;
/* I2C clock is set to 55 KhZ*/
#define I2C_CLK_DIVIDER         (1309)
/* 100KHz I2C bit-rate */
#define I2C_BITRATE         (100000)
/* Standard I2C mode */
#define I2C_MODE    (0)

#if defined(BOARD_NXP_LPCXPRESSO_1549)
/** 7-bit I2C addresses of Temperature Sensor */
#define I2C_TEMP_ADDR_7BIT  (0x4D)
#endif

/* SysTick rate in Hz */
#define TICKRATE_HZ         (10)

/* Current state for LED control via I2C cases */
static volatile int state;

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void) {
if(counter > 0) counter--;

static int ticks = 0;

ticks++;
if (ticks > TICKRATE_HZ) {
ticks = 0;
state = 1 - state;
}
}
#ifdef __cplusplus
}
#endif
void Sleep(int ms) {
counter = ms;
while(counter > 0) {
__WFI();
}
}

/* Initializes pin muxing for I2C interface - note that SystemInit() may
   already setup your pin muxing at system startup */
void Init_I2C_PinMux(void)
{
#if defined(BOARD_KEIL_MCB1500)||defined(BOARD_NXP_LPCXPRESSO_1549)
Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22, IOCON_DIGMODE_EN | I2C_MODE);
Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, IOCON_DIGMODE_EN | I2C_MODE);
Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SCL);
Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SDA);
#else
#error "No I2C Pin Muxing defined for this example"
#endif
}

/* Setup I2C handle and parameters */
void setupI2CMaster()
{
/* Enable I2C clock and reset I2C peripheral - the boot ROM does not
   do this */
Chip_I2C_Init(LPC_I2C0);

/* Setup clock rate for I2C */
Chip_I2C_SetClockDiv(LPC_I2C0, I2C_CLK_DIVIDER);

/* Setup I2CM transfer rate */
Chip_I2CM_SetBusSpeed(LPC_I2C0, I2C_BITRATE);

/* Enable Master Mode */
Chip_I2CM_Enable(LPC_I2C0);
}

static void SetupXferRecAndExecute(uint8_t devAddr,
   uint8_t *txBuffPtr,
   uint16_t txSize,
   uint8_t *rxBuffPtr,
   uint16_t rxSize)
{
/* Setup I2C transfer record */
i2cmXferRec.slaveAddr = devAddr;
i2cmXferRec.status = 0;
i2cmXferRec.txSz = txSize; /*!< Number of bytes in transmit array, if 0 only receive transfer will be carried on */
i2cmXferRec.rxSz = rxSize;
i2cmXferRec.txBuff = txBuffPtr;
i2cmXferRec.rxBuff = rxBuffPtr; /*!< Pointer memory where bytes received from I2C be stored */

Chip_I2CM_XferBlocking(LPC_I2C0, &i2cmXferRec);
}
uint8_t readStatus() {
uint8_t status;
uint8_t read = 0xF1;
SetupXferRecAndExecute(0x40, &read, 1, &status, 1); // read current status
Sleep(10);
return status;
}
void changeStatus(uint8_t const &status) {
ITM_output p;
uint8_t write[2] = {0xF1, status};
SetupXferRecAndExecute(0x40, write, 2, 0, 0); // write the new status
Sleep(10);
}

bool getModeFromStatus(uint8_t status) {
return (status >> 7) & 1; // can be done without & 1 because 7 is the last bit
}

bool getDataReadyFromStatus(uint8_t status) {
return (status >> 6) & 1;
}

void readNewTemperature(uint8_t &temperature) {
SetupXferRecAndExecute(0x48, 0x00, 1, &temperature, 1); //read new temperature to the variable
}

/* Master I2CM receive in polling mode */
#if defined(BOARD_NXP_LPCXPRESSO_1549)
/* Function to read LM75 I2C temperature sensor and output result */
static void ReadTemperatureI2CM(void)
{
uint8_t status;
uint8_t temperature;
ITM_output p;
std::string temperatureText;

if(getDataReadyFromStatus(readStatus())) { /*if status (6th bit) is 1 */
p.print("Status reading successful");
readNewTemperature(temperature);
temperatureText = "Temperature read over I2C is " + std::to_string(temperature) + " degrees C.\r\n";
}  else {
temperatureText = "Error " + std::to_string(i2cmXferRec.status) + " reading temperature.\r\n";
}
p.print(temperatureText);
}
#endif
