#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include "StateMachine.h"
#include <cr_section_macros.h>
#include <cstring>
#include <cstdio>
#include <string>
#include "ModbusMaster.h"
#include "ModbusRegister.h"
#include "DigitalIoPin.h"
#include "LpcUart.h"
#include "ITM_output.h"
#include "I2C.h"
#include "LiquidCrystal.h"
static volatile int counter;
static volatile uint32_t systicks;
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Handle interrupt from SysTick timer
 * @return Nothing
 */
void SysTick_Handler(void)
{
	systicks++;
	if(counter > 0) counter--;
}
#ifdef __cplusplus
}
#endif
void Sleep(int ms)
{
	counter = ms;
	while(counter > 0) {
		__WFI();
	}
}

/* this function is required by the modbus library */
uint32_t millis() {
	return systicks;
}

// check that value stays in the wanted limit
void LimitCheck(auto& nr,int max,int min){

	if(nr <= min){
		nr = min;
	}
	if(nr >= max){
		nr = max;
	}
}
bool setFrequency(ModbusMaster& node, uint16_t freq)
{
	int result;
	int ctr;
	bool atSetpoint;
	const int delay = 100;
	ModbusRegister Frequency(&node, 1); // reference 1
	ModbusRegister StatusWord(&node, 3);
	Frequency = freq; // set motor frequency
	printf("Set freq = %d\n", freq/40); // for debugging
	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = StatusWord;
		// check if we are at setpoint
		if (result >= 0 && (result & 0x0100)) atSetpoint = true;
		ctr++;
	} while(ctr < 20 && !atSetpoint);
	//printf("Elapsed: %d\n", ctr * delay); // for debugging
	return atSetpoint;
}
//alttitudecorrection for 0-120 meter hight

int16_t AlltiduteCorrection(int16_t pressure){
	pressure = (pressure/240)*0.95;
	return pressure;
}

int scale(float base,int nr){
	float i = nr/base;
	return (int) i;
}
// get the value of the pressure sensor
int16_t PressureSensor(){
	I2C_config cfg;
	I2C p(cfg);
	uint8_t status[3];
	uint16_t t;
	uint8_t read= 0xF1;
	//if(p.read(0x40, &read, 1));
	p.transaction(0x40, &read, 1,status,3);
	//printf("T; %d",t);
	// left shit and add the secound value to the
	t = ((uint16_t)status[0] << 8) | status[1];
	if((t & 0x8000)!=0) {
		int16_t mask = 0x7FFF;
		t = t & mask;
		t -= 32768;
	}
	return  AlltiduteCorrection((int16_t)t);
}
// Gives the requested frequency according to the pascal value between 0 to 120 = 1.19 ((max value - 1)/100)
//
int PascalToScale(int nr,int base){
	nr+=125;
	int nr2 = scale(base,nr);
	return nr2*200;
}
//encrease the frequency accordingly to the pressure and limit check
bool PressureSetting(int expectedpressure,ModbusMaster& node,int16_t& freq){

	bool ready = false;
	int16_t pressure= PressureSensor();
	if(pressure == expectedpressure ){
		ready = true;
		printf("frequency %d\n", freq);
	}else if(expectedpressure<pressure){
		freq-=100;
		printf("frequency %d\n", freq);
	}else if(expectedpressure>pressure){
		freq+=100;
		printf("frequency %d\n", freq);
	}
	LimitCheck(freq,20000,0);
	setFrequency(node, freq);
	printf("Frequency: %4d expected: %d, actuall pressure %d\n",freq,expectedpressure,pressure);
	return ready;
}
// Edit the frequency to change the fan speed
void ManualButtons(ModbusMaster& node,int16_t& freq, LiquidCrystal *l){
	DigitalIoPin button1(0,9, true, true, true);
	DigitalIoPin button2(0,0, true, true, true);
	DigitalIoPin button3(1,3, true, true, true);
	DigitalIoPin button4(0,10, true, true, true);

	while(1){
		l-> clear();
		int percentage2 = scale(200,freq);
		std::string prec= std::to_string(percentage2);
		l->print("Speed(0-100):" + prec);
		int p = PressureSensor();
		std::string pre = std::to_string(p);
		l->setCursor(0,1);
		l->print("P:" + pre);
		if (button1.read()) {
			while(button1.read()){
				Sleep(100);
				freq +=200;
				LimitCheck(freq,20000,0);
				setFrequency(node, freq);
				int percentage = scale(200,freq);
				printf("freq: %4d precentage: %d\n",freq,percentage);
				l -> clear();
				l->print("Speed(0-100):" + std::to_string(percentage));
				int p = PressureSensor();
				std::string pre = std::to_string(p);
				l->setCursor(0,1);
				l->print("P:" + pre);
			}
		}else if (button2.read()) {
			while(button2.read()){
				Sleep(100);
				freq -=200;
				LimitCheck(freq,20000,0);
				setFrequency(node, freq);
				int percentage = scale(200,freq);
				printf("freq: %4d precentage: %d\n",freq,percentage);
				l-> clear();
				std::string prec= std::to_string(percentage);
				l->print("Speed(0-100):" + prec);
				int p = PressureSensor();
				std::string pre = std::to_string(p);
				l->setCursor(0,1);
				l->print("P:" + pre);
			}
		}else if (button3.read()) {
			printf("button3\n");
			l->clear();
			l->print("Manual 1 Auto 2");
			Sleep(500);
			break;
		}
		setFrequency(node, freq);
		int16_t pressure=PressureSensor();
		int percentage = scale(200,freq);
		printf("Freq: %4d %: %d Pressure: %4d\n",freq,percentage,pressure);
	}
}
// Fan speed set accordingly to the wanted pressure

void AutomaticButtons(ModbusMaster& node,int16_t& freq,int& expectedpresusre, LiquidCrystal *l, ModbusRegister * Current ){

	DigitalIoPin button1(0,9, true, true, true);
	DigitalIoPin button2(0,0, true, true, true);
	DigitalIoPin button3(1,3, true, true, true);
	//DigitalIoPin button4(0,10, true, true, true);
	int count=0;
	int counterlimit = 10000;
	while(1){
		std::string ep = std::to_string(expectedpresusre);
		l->clear();
		l->print("Press(0-120):"+ep);
		std::string fan = std::to_string((int)*Current);
		l->setCursor(0,1);
		int p = PressureSensor();
		std::string cp = std::to_string(p);
		l->print("S:" + fan + " P:" +cp);
		if (button1.read()) {
			while(button1.read()){
				Sleep(100);
				expectedpresusre +=1;
				LimitCheck(expectedpresusre,120,0);
				PressureSetting(expectedpresusre,node,freq);
				std::string ep = std::to_string(expectedpresusre);
				l-> clear();
				l->print("Press(0-120):"+ep);
				std::string fan = std::to_string((int)*Current);
				l->setCursor(0,1);
				int p = PressureSensor();
				std::string cp = std::to_string(p);
				l->print("S:" + fan + " P:" +cp);
			}
			printf("button1, %d\n",expectedpresusre);
			count = 0;
			Sleep(500);
		}else if (button2.read()) {
			while(button2.read()){
				Sleep(100);
				expectedpresusre -=1;
				LimitCheck(expectedpresusre,120,0);
				PressureSetting(expectedpresusre,node,freq);
				std::string ep = std::to_string(expectedpresusre);
				l->clear();
				l->print("Press(0-120):"+ep);
				std::string fan = std::to_string((int)*Current);
				l->setCursor(0,1);
				int p = PressureSensor();
				std::string cp = std::to_string(p);
				l->print("S:" + fan + " P:" +cp);
			}
			printf("button2, %d\n",expectedpresusre);
			count=0;
			Sleep(500);
		}else if (button3.read()) {
			printf("button3\n");
			l->clear();
			l->print("Manual 1 Auto 2");
			printf("loop out\n");
			Sleep(500);
			break;
		}/*else if (button4.read()) {
printf("button4\n");
break;
//back to main menu
}*/
		count++;
		int p2 = PressureSensor();
		if(count >= counterlimit&& expectedpresusre != p2){
			l-> clear();
			l->print("Cant reach pressure");
			printf("message\n");
			count=0;
			Sleep(3000);
		}
		printf("count %d",count);
		if(PressureSetting(expectedpresusre,node,freq)){
			count=0;
		}
	}
}

void abbmodbus(LiquidCrystal *l){
	ModbusMaster node(2);
	node.begin(9600);
	ModbusRegister ControlWord(&node, 0);
	ModbusRegister StatusWord(&node, 3);
	ModbusRegister OutputFrequency(&node, 102);
	ModbusRegister Current(&node, 103);
	DigitalIoPin button1(0,9, true, true, true);
	DigitalIoPin button2(0,0, true, true, true);
	DigitalIoPin button3(1,3, true, true, true);
	//DigitalIoPin button4(0,10, true, true, true);
	StateMachine state;
	int16_t freq= 0;
	/*ControlWord = 0x0406;
	printf("Status=%04X\n", (int)StatusWord); // for debugging
	Sleep(1000);
	ControlWord = 0x047F;
	printf("Status=%04X\n", (int)StatusWord); // for debugging
	Sleep(1000);
	printf("Start!\n");*/
	state.HandelState(StatusWord,ControlWord);
	int expecetedpressure=0;
	while(1){
		l-> clear();
		l-> print("Manual 1 Auto 2");
		while (node.HeartBeat()) {
			if (button1.read()) {
				l-> clear();
				l->print("Manual mode");
				l->setCursor(0,1);
				l->print("speed: b1+ b2-");
				Sleep(1000);
				ManualButtons(node,freq, l);
			}else if (button2.read()) {
				l-> clear();
				l->print("Automatic mode");
				l->setCursor(0,1);
				l->print("pres: b1+ b2-");
				Sleep(1000);
				AutomaticButtons(node,freq,expecetedpressure, l, &Current);
			}
			printf("F=%4d, I=%4d\n", (int) OutputFrequency, (int) Current);
			//printf("Getting inside the function\n");
			//Sleep(delay);
		}
		l-> clear();
		l->print("Inactive!");
		printf("INACTIVE\n");
	}
}

int main(void)
{

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	uint32_t sysTickRate;
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / 1000);
	// TODO: insert code here
	Chip_RIT_Init(LPC_RITIMER);
	LpcPinMap none = {-1, -1}; // unused pin has negative values in it
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
	LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
	LpcUart dbgu(cfg);

	DigitalIoPin rs = DigitalIoPin(0,8, false, false, false);
	DigitalIoPin en = DigitalIoPin(1,6, false, false, false);
	DigitalIoPin d4= DigitalIoPin(1,8, false, false, false);
	DigitalIoPin d5= DigitalIoPin(0,5, false, false, false);
	DigitalIoPin d6= DigitalIoPin(0,6, false, false, false);
	DigitalIoPin d7 = DigitalIoPin (0,7, false, false, false);
	LiquidCrystal lcd(&rs, &en, &d4, &d5, &d6, &d7);
	lcd.begin(16,2);
	lcd.setCursor(0,0);
	/* Set up SWO to PIO1_2 */
	Chip_SWM_MovablePortPinAssign(SWM_SWO_O, 1, 2); // Needed for SWO printf

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(SystemCoreClock / 1000);

	Board_LED_Set(0, false);
	Board_LED_Set(1, true);
	lcd.print("Started");
	printf("Started\n"); // goes to ITM console if retarget_itm.c is included
	dbgu.write("Hello, world\n");
	printf("Hello\n");
	abbmodbus(&lcd);
	return 1;

}

