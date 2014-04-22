################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FaultRecovery/FaultRecovery.cpp 

OBJS += \
./FaultRecovery/FaultRecovery.o 

CPP_DEPS += \
./FaultRecovery/FaultRecovery.d 


# Each subdirectory must supply rules for building sources it contributes
FaultRecovery/%.o: ../FaultRecovery/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -D__CODE_RED -DCPP_USE_HEAP -DTARGET_LPC1768 -DTARGET_M3 -DTARGET_NXP -DTARGET_LPC176X -DTARGET_MBED_LPC1768 -DTOOLCHAIN_GCC_CR -DTOOLCHAIN_GCC -D__CORTEX_M3 -DARM_MATH_CM3 -DMBED_BUILD_TIMESTAMP=1396782748.38 -D__MBED__=1 -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\DumpRegistersAtHardFault" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\FaultRecovery" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\SHTx" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\Logger" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\mbed" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\mbed\TARGET_LPC1768" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\nRF24L01P" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\FaultInjector" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\PowerControl" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\ConfigFile" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\DataStructs" -I"D:\Documentos\LPCXpresso\Workspace\WeatherStation\GPS" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


