/*
 * DigitalIoPin1.cpp
 *
 *  Created on: 24.1.2020
 *      Author: Elppo
 */

#include "DigitalIoPin.h"
#include "Chip.h"

bool DigitalIoPin::read() {
	return Chip_GPIO_GetPinState(LPC_GPIO, port_, pin_);

}

void DigitalIoPin::write(bool value) {
	if(invert_) {
		Chip_GPIO_SetPinState(LPC_GPIO, port_, pin_, !value);
	} else {
		Chip_GPIO_SetPinState(LPC_GPIO, port_, pin_, value);
	}
}
