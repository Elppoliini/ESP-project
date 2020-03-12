/*
 * StateMachine.cpp
 *
 *  Created on: 8.3.2020
 *      Author: johan
 */

#include "StateMachine.h"


StateMachine::~StateMachine() {
	// TODO Auto-generated destructor stub
}
void StateMachine::HandelControlWorld(const Event &e,ModbusRegister& ControlWord){
	uint16_t CW = ControlWord;
	switch (e.type) {
	case EventType::begin:
		ControlWord = 0x0406;
		printf("not ready to switch on\n");
		break;
	case EventType::switch_not_ready:
		//add the event from the Eventhandler here
		// set CW  to xxxx xxxx xxxx x110
		events_.CWsetNthBitTo0(0, CW);
		events_.CWsetNthBitTo1(1, CW);
		events_.CWsetNthBitTo1(2, CW);
		events_.CWsetNthBitTo1(4, CW);
		events_.CWsetNthBitTo1(10, CW);
		ControlWord = 0x0406;
		//ControlWord =0000010000010110;
		printf("not ready to switch on\n");
		break;
	case EventType::switch_ready:
		//add the event from the Eventhandler here
		//set CW to xxxx xxxx xxxx x111
		events_.CWsetNthBitTo1(0, CW);
		events_.CWsetNthBitTo1(1, CW);
		events_.CWsetNthBitTo1(2, CW);
		events_.CWsetNthBitTo1(4, CW);
		//ControlWord =0000010000010111;
		printf("ready to switch on\n");
		break;
	case EventType::ready_operate:
		//add the event from the Eventhandler here
		// bit3 =1
		events_.CWsetNthBitTo1(3, CW);
		//ControlWord =0000010000010111;
		printf("ready to operate\n");
		break;
	case EventType::operation_enalbled:
		//add the event from the Eventhandler here
		//bit 5 =1
		events_.CWsetNthBitTo1(5, CW);
		events_.CWsetNthBitTo1(6, CW);
		//ControlWord =0000010000111111;
		printf("Operation enabled\n");
		break;
	case EventType::RFG:
		//add the event from the Eventhandler here
		//bit 6=1
		events_.CWsetNthBitTo1(6, CW);
		//ControlWord =0000010001111111;
		printf("Acceleration enabled\n");
		break;
	case EventType::operating:
		//add the event from the Eventhandler here
		printf("operating\n");
		break;
	}
	ControlWord=CW;
}

void StateMachine::HandelState(ModbusRegister& StatusWord,ModbusRegister& ControlWord){
	// check each state untill it mach the condition and after that change the CW
	//laita jokaisin while looppiin ehdoksi SW bitti compareit and take 1 awey
	//printf("Status=%04X\n", (int)StatusWorld);
	HandelControlWorld(Event(EventType::begin), ControlWord);
	printf("Status MONSTER BAAAAAAARH 0=%04X\n", (int)StatusWord);
	while(events_.SWcheckNthBitIs0(0, StatusWord)){
		printf("Status 1=%04X\n", (int)StatusWord);
	}
	HandelControlWorld(Event(EventType::switch_not_ready), ControlWord);
	printf("Status 2=%04X\n", (int)StatusWord);
	while(!events_.SWcheckNthBitIs1(0,StatusWord)){}
	HandelControlWorld(Event(EventType::switch_ready), ControlWord);
	printf("Status 3=%04X\n", (int)StatusWord);
	while(!events_.SWcheckNthBitIs1(1,StatusWord)){}
	printf("Status 4=%04X\n", (int)StatusWord);
	while(!events_.SWcheckNthBitIs1(12,StatusWord)){}
	HandelControlWorld(Event(EventType::ready_operate), ControlWord);
	// check firt that sw bit 12=1 and then bit 2 =1
	printf("Status 5=%04X\n", (int)StatusWord);
	while(!events_.SWcheckNthBitIs1(2,StatusWord)&& events_.SWcheckNthBitIs1(12,StatusWord)){}
	HandelControlWorld(Event(EventType::operation_enalbled), ControlWord);
	HandelControlWorld(Event(EventType::RFG), ControlWord);
	printf("Status 6=%04X\n", (int)StatusWord);
	while(!events_.SWcheckNthBitIs1(8,StatusWord)){}
	HandelControlWorld(Event(EventType::operating), ControlWord);
	printf("Status last=%04X\n", (int)StatusWord);

}


