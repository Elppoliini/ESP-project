################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DigitalIoPin.cpp \
../src/ESPProject.cpp \
../src/I2C.cpp \
../src/I2Cpressuresensor.cpp \
../src/ITM_output.cpp \
../src/LiquidCrystal.cpp \
../src/LpcUart.cpp \
../src/ModbusMaster.cpp \
../src/ModbusRegister.cpp \
../src/SerialPort.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc15xx.cpp 

C_SRCS += \
../src/ITM_write.c \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/DigitalIoPin.o \
./src/ESPProject.o \
./src/I2C.o \
./src/I2Cpressuresensor.o \
./src/ITM_output.o \
./src/ITM_write.o \
./src/LiquidCrystal.o \
./src/LpcUart.o \
./src/ModbusMaster.o \
./src/ModbusRegister.o \
./src/SerialPort.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc15xx.o \
./src/crp.o \
./src/sysinit.o 

CPP_DEPS += \
./src/DigitalIoPin.d \
./src/ESPProject.d \
./src/I2C.d \
./src/I2Cpressuresensor.d \
./src/ITM_output.d \
./src/LiquidCrystal.d \
./src/LpcUart.d \
./src/ModbusMaster.d \
./src/ModbusRegister.d \
./src/SerialPort.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc15xx.d 

C_DEPS += \
./src/ITM_write.d \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Elppo\Documents\Koulu\2.vuosi_Smart_Systems\3 jakso\ES-project\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Elppo\Documents\Koulu\2.vuosi_Smart_Systems\3 jakso\ES-project\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ESPProject.o: ../src/ESPProject.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=c++11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Elppo\Documents\Koulu\2.vuosi_Smart_Systems\3 jakso\ES-project\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Elppo\Documents\Koulu\2.vuosi_Smart_Systems\3 jakso\ES-project\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/ESPProject.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"C:\Users\Elppo\Documents\Koulu\2.vuosi_Smart_Systems\3 jakso\ES-project\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Elppo\Documents\Koulu\2.vuosi_Smart_Systems\3 jakso\ES-project\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


