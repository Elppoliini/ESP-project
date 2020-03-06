/*
 * ITM_output.cpp
 *
 *  Created on: 24.1.2020
 *      Author: Elppo
 */

#include "ITM_output.h"
#include <string>

void ITM_output::print(std::string output) {
	if(!(output.find('\n') != std::string::npos)) {
		output += "\n";
	}
	const char* print = output.c_str();
	ITM_write(print);
}


