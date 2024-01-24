################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gpio.c \
../Src/main.c \
../Src/rcc.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/gpio.o \
./Src/main.o \
./Src/rcc.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/gpio.d \
./Src/main.d \
./Src/rcc.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"C:/Users/Armand/Documents/STM32 Programs/STM32F1_BareMetal_Template/STM32F1_BareMetal/Drivers/CMSIS/Core/Include" -I"C:/Users/Armand/Documents/STM32 Programs/STM32F1_BareMetal_Template/STM32F1_BareMetal/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/rcc.cyclo ./Src/rcc.d ./Src/rcc.o ./Src/rcc.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

