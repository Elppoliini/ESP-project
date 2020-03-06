/*
 * I2Cpressuresensor.h
 *
 *  Created on: 28.2.2020
 *      Author: johan
 */
#include <atomic>
#ifndef I2CPRESSURESENSOR_H_
#define I2CPRESSURESENSOR_H_

class I2C_pressure_sensor {
public:
	I2C_pressure_sensor();
	virtual ~I2C_pressure_sensor();
	void setupI2CMaster();
	void SetupXferRecAndExecute(uint8_t devAddr,
			uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr,
			uint16_t rxSize);
	uint8_t readStatus();
	void changeStatus(uint8_t const &status);
	bool getModeFromStatus(uint8_t status);
	bool getDataReadyFromStatus(uint8_t status);
	void readNewTemperature(uint8_t &temperature);
	void ReadTemperatureI2CM(void);
};

#endif /* I2CPRESSURESENSOR_H_ */
