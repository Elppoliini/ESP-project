/*
 * EventHandler.cpp
 *
 *  Created on: 8.3.2020
 *      Author: johan
 */

#include "EventHandler.h"

EventHandler::EventHandler() {
	// TODO Auto-generated constructor stub

}

EventHandler::~EventHandler() {
	// TODO Auto-generated destructor stub
}

bool EventHandler::SWcheckNthBitIs0(uint bit, uint16_t swBytes) {
	uint16_t mask = 0x01 << bit;
	return !(swBytes & mask);
}
bool EventHandler:: SWcheckNthBitIs1(uint bit,uint16_t swBytes) {
	return !SWcheckNthBitIs0(bit,swBytes);
}

//Control Word bit changing

void EventHandler::CWsetNthBitTo0(uint bit, uint16_t &cwBytes) {
	uint16_t mask = ~(0x01 << bit);
	cwBytes &= mask;
}

void EventHandler::CWsetNthBitTo1(uint bit, uint16_t &cwBytes) {
	uint16_t mask = 0x01 << bit;
	cwBytes |= mask;
}

void EventHandler::toggleNthBit(uint bit, uint16_t &original) {
	uint16_t mask = 0x01 << bit;
	original ^= mask;
}
