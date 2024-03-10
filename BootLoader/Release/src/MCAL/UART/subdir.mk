################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MCAL/UART/UART_CNF.c \
../src/MCAL/UART/UART_program.c 

OBJS += \
./src/MCAL/UART/UART_CNF.o \
./src/MCAL/UART/UART_program.o 

C_DEPS += \
./src/MCAL/UART/UART_CNF.d \
./src/MCAL/UART/UART_program.d 


# Each subdirectory must supply rules for building sources it contributes
src/MCAL/UART/%.o: ../src/MCAL/UART/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DNDEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=18000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

