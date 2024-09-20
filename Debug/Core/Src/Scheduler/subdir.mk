################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Scheduler/PriorityQueue.c \
../Core/Src/Scheduler/Scheduler.c \
../Core/Src/Scheduler/Task.c 

OBJS += \
./Core/Src/Scheduler/PriorityQueue.o \
./Core/Src/Scheduler/Scheduler.o \
./Core/Src/Scheduler/Task.o 

C_DEPS += \
./Core/Src/Scheduler/PriorityQueue.d \
./Core/Src/Scheduler/Scheduler.d \
./Core/Src/Scheduler/Task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Scheduler/%.o Core/Src/Scheduler/%.su Core/Src/Scheduler/%.cyclo: ../Core/Src/Scheduler/%.c Core/Src/Scheduler/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Scheduler

clean-Core-2f-Src-2f-Scheduler:
	-$(RM) ./Core/Src/Scheduler/PriorityQueue.cyclo ./Core/Src/Scheduler/PriorityQueue.d ./Core/Src/Scheduler/PriorityQueue.o ./Core/Src/Scheduler/PriorityQueue.su ./Core/Src/Scheduler/Scheduler.cyclo ./Core/Src/Scheduler/Scheduler.d ./Core/Src/Scheduler/Scheduler.o ./Core/Src/Scheduler/Scheduler.su ./Core/Src/Scheduler/Task.cyclo ./Core/Src/Scheduler/Task.d ./Core/Src/Scheduler/Task.o ./Core/Src/Scheduler/Task.su

.PHONY: clean-Core-2f-Src-2f-Scheduler

