/*
 * EventHandler.h
 *
 *  Created on: 8.3.2020
 *      Author: johan
 */

#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_
#include "chip.h"
#include <cstring>
#include <cstdio>
class EventHandler {
public:
	EventHandler();
	virtual ~EventHandler();
	bool SWcheckNthBitIs0(uint bit, uint16_t swBytes);
	bool SWcheckNthBitIs1(uint bit,uint16_t swBytes);
	//Control Word bit changing
	void CWsetNthBitTo0(uint bit, uint16_t &cwBytes);
	void CWsetNthBitTo1(uint bit, uint16_t &cwBytes);
	void toggleNthBit(uint bit, uint16_t &original);
};

#endif /* EVENTHANDLER_H_ */
