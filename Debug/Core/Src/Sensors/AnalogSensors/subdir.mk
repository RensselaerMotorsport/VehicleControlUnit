################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Sensors/AnalogSensors/Accelerometer.c \
../Core/Src/Sensors/AnalogSensors/AccelerometerWrapper.c \
../Core/Src/Sensors/AnalogSensors/App.c \
../Core/Src/Sensors/AnalogSensors/BrakePressure.c \
../Core/Src/Sensors/AnalogSensors/ShockPot.c \
../Core/Src/Sensors/AnalogSensors/SteeringColumn.c \
../Core/Src/Sensors/AnalogSensors/Temperature.c \
../Core/Src/Sensors/AnalogSensors/WheelFlux.c \
../Core/Src/Sensors/AnalogSensors/WheelSpeed.c 

OBJS += \
./Core/Src/Sensors/AnalogSensors/Accelerometer.o \
./Core/Src/Sensors/AnalogSensors/AccelerometerWrapper.o \
./Core/Src/Sensors/AnalogSensors/App.o \
./Core/Src/Sensors/AnalogSensors/BrakePressure.o \
./Core/Src/Sensors/AnalogSensors/ShockPot.o \
./Core/Src/Sensors/AnalogSensors/SteeringColumn.o \
./Core/Src/Sensors/AnalogSensors/Temperature.o \
./Core/Src/Sensors/AnalogSensors/WheelFlux.o \
./Core/Src/Sensors/AnalogSensors/WheelSpeed.o 

C_DEPS += \
./Core/Src/Sensors/AnalogSensors/Accelerometer.d \
./Core/Src/Sensors/AnalogSensors/AccelerometerWrapper.d \
./Core/Src/Sensors/AnalogSensors/App.d \
./Core/Src/Sensors/AnalogSensors/BrakePressure.d \
./Core/Src/Sensors/AnalogSensors/ShockPot.d \
./Core/Src/Sensors/AnalogSensors/SteeringColumn.d \
./Core/Src/Sensors/AnalogSensors/Temperature.d \
./Core/Src/Sensors/AnalogSensors/WheelFlux.d \
./Core/Src/Sensors/AnalogSensors/WheelSpeed.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Sensors/AnalogSensors/%.o Core/Src/Sensors/AnalogSensors/%.su Core/Src/Sensors/AnalogSensors/%.cyclo: ../Core/Src/Sensors/AnalogSensors/%.c Core/Src/Sensors/AnalogSensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Sensors-2f-AnalogSensors

clean-Core-2f-Src-2f-Sensors-2f-AnalogSensors:
	-$(RM) ./Core/Src/Sensors/AnalogSensors/Accelerometer.cyclo ./Core/Src/Sensors/AnalogSensors/Accelerometer.d ./Core/Src/Sensors/AnalogSensors/Accelerometer.o ./Core/Src/Sensors/AnalogSensors/Accelerometer.su ./Core/Src/Sensors/AnalogSensors/AccelerometerWrapper.cyclo ./Core/Src/Sensors/AnalogSensors/AccelerometerWrapper.d ./Core/Src/Sensors/AnalogSensors/AccelerometerWrapper.o ./Core/Src/Sensors/AnalogSensors/AccelerometerWrapper.su ./Core/Src/Sensors/AnalogSensors/App.cyclo ./Core/Src/Sensors/AnalogSensors/App.d ./Core/Src/Sensors/AnalogSensors/App.o ./Core/Src/Sensors/AnalogSensors/App.su ./Core/Src/Sensors/AnalogSensors/BrakePressure.cyclo ./Core/Src/Sensors/AnalogSensors/BrakePressure.d ./Core/Src/Sensors/AnalogSensors/BrakePressure.o ./Core/Src/Sensors/AnalogSensors/BrakePressure.su ./Core/Src/Sensors/AnalogSensors/ShockPot.cyclo ./Core/Src/Sensors/AnalogSensors/ShockPot.d ./Core/Src/Sensors/AnalogSensors/ShockPot.o ./Core/Src/Sensors/AnalogSensors/ShockPot.su ./Core/Src/Sensors/AnalogSensors/SteeringColumn.cyclo ./Core/Src/Sensors/AnalogSensors/SteeringColumn.d ./Core/Src/Sensors/AnalogSensors/SteeringColumn.o ./Core/Src/Sensors/AnalogSensors/SteeringColumn.su ./Core/Src/Sensors/AnalogSensors/Temperature.cyclo ./Core/Src/Sensors/AnalogSensors/Temperature.d ./Core/Src/Sensors/AnalogSensors/Temperature.o ./Core/Src/Sensors/AnalogSensors/Temperature.su ./Core/Src/Sensors/AnalogSensors/WheelFlux.cyclo ./Core/Src/Sensors/AnalogSensors/WheelFlux.d ./Core/Src/Sensors/AnalogSensors/WheelFlux.o ./Core/Src/Sensors/AnalogSensors/WheelFlux.su ./Core/Src/Sensors/AnalogSensors/WheelSpeed.cyclo ./Core/Src/Sensors/AnalogSensors/WheelSpeed.d ./Core/Src/Sensors/AnalogSensors/WheelSpeed.o ./Core/Src/Sensors/AnalogSensors/WheelSpeed.su

.PHONY: clean-Core-2f-Src-2f-Sensors-2f-AnalogSensors

