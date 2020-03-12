/*
 * DigitalIoPin1.h
 *
 *  Created on: 24.1.2020
 *      Author: Elppo
 */

#ifndef DIGITALIOPIN_H_
#define DIGITALIOPIN_H_

#include "Chip.h"

class DigitalIoPin {
public:
	DigitalIoPin(int port, int pin, bool input = true, bool pullup = true, bool invert = false){

		pin_ = pin;
		port_ = port;
		input_ = input;
		pullup_ = pullup;
		invert_ = invert;

		if(input_) {
			if(pullup_) {
				if(invert_) {
					Chip_IOCON_PinMuxSet(LPC_IOCON, port_, pin_, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
				} else {
					Chip_IOCON_PinMuxSet(LPC_IOCON, port_, pin_, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
				}
			}else {
				Chip_IOCON_PinMuxSet(LPC_IOCON, port_, pin_, (IOCON_DIGMODE_EN));
			}
		} else {
			Chip_IOCON_PinMuxSet(LPC_IOCON, port_, pin_, (IOCON_MODE_INACT |IOCON_DIGMODE_EN));
		}
		if(input_) {
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, port_, pin_);
		} else {
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, port_, pin_);
		}
	};
	virtual ~DigitalIoPin() = default;
	bool read();
	void write(bool value);
private:
	// add these as needed
	int port_;
	int pin_;
	bool input_;
	bool pullup_;
	bool invert_;
};

#endif /* DIGITALIOPIN_H_ */
