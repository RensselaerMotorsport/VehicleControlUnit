################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Sensors/CANSensors/Accumulator.c \
../Core/Src/Sensors/CANSensors/BMS.c \
../Core/Src/Sensors/CANSensors/BmsData.c \
../Core/Src/Sensors/CANSensors/Esc.c \
../Core/Src/Sensors/CANSensors/Imd.c \
../Core/Src/Sensors/CANSensors/MotorController.c 

OBJS += \
./Core/Src/Sensors/CANSensors/Accumulator.o \
./Core/Src/Sensors/CANSensors/BMS.o \
./Core/Src/Sensors/CANSensors/BmsData.o \
./Core/Src/Sensors/CANSensors/Esc.o \
./Core/Src/Sensors/CANSensors/Imd.o \
./Core/Src/Sensors/CANSensors/MotorController.o 

C_DEPS += \
./Core/Src/Sensors/CANSensors/Accumulator.d \
./Core/Src/Sensors/CANSensors/BMS.d \
./Core/Src/Sensors/CANSensors/BmsData.d \
./Core/Src/Sensors/CANSensors/Esc.d \
./Core/Src/Sensors/CANSensors/Imd.d \
./Core/Src/Sensors/CANSensors/MotorController.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Sensors/CANSensors/%.o Core/Src/Sensors/CANSensors/%.su Core/Src/Sensors/CANSensors/%.cyclo: ../Core/Src/Sensors/CANSensors/%.c Core/Src/Sensors/CANSensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Sensors-2f-CANSensors

clean-Core-2f-Src-2f-Sensors-2f-CANSensors:
	-$(RM) ./Core/Src/Sensors/CANSensors/Accumulator.cyclo ./Core/Src/Sensors/CANSensors/Accumulator.d ./Core/Src/Sensors/CANSensors/Accumulator.o ./Core/Src/Sensors/CANSensors/Accumulator.su ./Core/Src/Sensors/CANSensors/BMS.cyclo ./Core/Src/Sensors/CANSensors/BMS.d ./Core/Src/Sensors/CANSensors/BMS.o ./Core/Src/Sensors/CANSensors/BMS.su ./Core/Src/Sensors/CANSensors/BmsData.cyclo ./Core/Src/Sensors/CANSensors/BmsData.d ./Core/Src/Sensors/CANSensors/BmsData.o ./Core/Src/Sensors/CANSensors/BmsData.su ./Core/Src/Sensors/CANSensors/Esc.cyclo ./Core/Src/Sensors/CANSensors/Esc.d ./Core/Src/Sensors/CANSensors/Esc.o ./Core/Src/Sensors/CANSensors/Esc.su ./Core/Src/Sensors/CANSensors/Imd.cyclo ./Core/Src/Sensors/CANSensors/Imd.d ./Core/Src/Sensors/CANSensors/Imd.o ./Core/Src/Sensors/CANSensors/Imd.su ./Core/Src/Sensors/CANSensors/MotorController.cyclo ./Core/Src/Sensors/CANSensors/MotorController.d ./Core/Src/Sensors/CANSensors/MotorController.o ./Core/Src/Sensors/CANSensors/MotorController.su

.PHONY: clean-Core-2f-Src-2f-Sensors-2f-CANSensors

