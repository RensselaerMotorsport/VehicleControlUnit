################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Sensors/AnalogSensor.c \
../Core/Src/Sensors/CANSensor.c \
../Core/Src/Sensors/DigitalSensor.c \
../Core/Src/Sensors/Sensor.c 

OBJS += \
./Core/Src/Sensors/AnalogSensor.o \
./Core/Src/Sensors/CANSensor.o \
./Core/Src/Sensors/DigitalSensor.o \
./Core/Src/Sensors/Sensor.o 

C_DEPS += \
./Core/Src/Sensors/AnalogSensor.d \
./Core/Src/Sensors/CANSensor.d \
./Core/Src/Sensors/DigitalSensor.d \
./Core/Src/Sensors/Sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Sensors/%.o Core/Src/Sensors/%.su Core/Src/Sensors/%.cyclo: ../Core/Src/Sensors/%.c Core/Src/Sensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Sensors

clean-Core-2f-Src-2f-Sensors:
	-$(RM) ./Core/Src/Sensors/AnalogSensor.cyclo ./Core/Src/Sensors/AnalogSensor.d ./Core/Src/Sensors/AnalogSensor.o ./Core/Src/Sensors/AnalogSensor.su ./Core/Src/Sensors/CANSensor.cyclo ./Core/Src/Sensors/CANSensor.d ./Core/Src/Sensors/CANSensor.o ./Core/Src/Sensors/CANSensor.su ./Core/Src/Sensors/DigitalSensor.cyclo ./Core/Src/Sensors/DigitalSensor.d ./Core/Src/Sensors/DigitalSensor.o ./Core/Src/Sensors/DigitalSensor.su ./Core/Src/Sensors/Sensor.cyclo ./Core/Src/Sensors/Sensor.d ./Core/Src/Sensors/Sensor.o ./Core/Src/Sensors/Sensor.su

.PHONY: clean-Core-2f-Src-2f-Sensors

