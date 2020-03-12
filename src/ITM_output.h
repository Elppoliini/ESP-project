/*
 * ITM_output.h
 *
 *  Created on: 24.1.2020
 *      Author: Elppo
 */

#ifndef ITM_OUTPUT_H_
#define ITM_OUTPUT_H_

#include <string>
#include "ITM_write.h"

class ITM_output {
public:
	ITM_output() {
	};
	~ITM_output() = default;
	void print(std::string output);
};


#endif /* ITM_OUTPUT_H_ */
