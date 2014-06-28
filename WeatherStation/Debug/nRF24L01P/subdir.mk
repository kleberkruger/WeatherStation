################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../nRF24L01P/nRF24L01P.cpp 

OBJS += \
./nRF24L01P/nRF24L01P.o 

CPP_DEPS += \
./nRF24L01P/nRF24L01P.d 


# Each subdirectory must supply rules for building sources it contributes
nRF24L01P/%.o: ../nRF24L01P/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -D__CODE_RED -DCPP_USE_HEAP -DTARGET_LPC1768 -DTARGET_M3 -DTARGET_NXP -DTARGET_LPC176X -DTARGET_MBED_LPC1768 -DTOOLCHAIN_GCC_CR -DTOOLCHAIN_GCC -D__CORTEX_M3 -DARM_MATH_CM3 -DMBED_BUILD_TIMESTAMP=1399758505.68 -D__MBED__=1 -I"C:\Users\Kleber\git\WeatherStation\WeatherStation" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\DumpRegistersAtHardFault" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\FaultRecovery" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\SHTx" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\Logger" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\mbed" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\mbed\TARGET_LPC1768" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\mbed\TARGET_LPC1768\TARGET_NXP" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\mbed\TARGET_LPC1768\TARGET_NXP\TARGET_LPC176X" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\mbed\TARGET_LPC1768\TARGET_NXP\TARGET_LPC176X\TARGET_MBED_LPC1768" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\nRF24L01P" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\FaultInjector" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\PowerControl" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\ConfigFile" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\DataStructs" -I"C:\Users\Kleber\git\WeatherStation\WeatherStation\GPS" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


