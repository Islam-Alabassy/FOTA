################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 

# Each subdirectory must supply rules for building sources it contributes
system/src/newlib/_startup.null: ../system/src/newlib/_startup.c system/src/newlib/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	"../include" "../system/include" "../system/include/cmsis" "../system/include/stm32f4-hal" DEBUG USE_FULL_ASSERT TRACE OS_USE_TRACE_SEMIHOSTING_DEBUG STM32F401xC USE_HAL_DRIVER HSE_VALUE=18000000 DEBUG USE_FULL_ASSERT TRACE OS_USE_TRACE_SEMIHOSTING_DEBUG STM32F401xC USE_HAL_DRIVER HSE_VALUE=18000000 OS_INCLUDE_STARTUP_INIT_MULTIPLE_RAM_SECTIONS  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-system-2f-src-2f-newlib

clean-system-2f-src-2f-newlib:
	-$(RM) ./system/src/newlib/_startup.null

.PHONY: clean-system-2f-src-2f-newlib

