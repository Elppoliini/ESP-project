/*
 * StateMachine.h
 *
 *  Created on: 8.3.2020
 *      Author: johan
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_
#include "EventHandler.h"
#include "ModbusRegister.h"
enum EventType {begin,switch_not_ready,switch_ready,ready_operate,operation_enalbled,RFG,operating};
class Event {
public:
	Event(EventType e):type(e) {};
	~Event(){};
	EventType type;
};

class StateMachine {
public:
	StateMachine():events_(){};
	virtual ~StateMachine();
	void HandelState(ModbusRegister& StatusWorld,ModbusRegister& ControlWord);
	void HandelControlWorld(const Event &e,ModbusRegister& ControlWord);
private:
	EventHandler events_;
};

#endif /* STATEMACHINE_H_ */


