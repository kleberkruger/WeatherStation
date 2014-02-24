################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include nRF24L01P/subdir.mk
-include mbed/TARGET_LPC1768/TOOLCHAIN_GCC_CR/subdir.mk
-include SHTx/subdir.mk
-include PowerControl/subdir.mk
-include GPS/subdir.mk
-include FaultInjector/subdir.mk
-include DumpRegistersAtHardFault/subdir.mk
-include ConfigFile/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: WeatherStation.axf

# Tool invocations
WeatherStation.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\DumpRegistersAtHardFault" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\FaultRecovery" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\SHTx" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\Logger" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\mbed" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\mbed\TARGET_LPC1768" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\nRF24L01P" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\FaultInjector" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\PowerControl" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\ConfigFile" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\DataStructs" -L"D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\GPS" -Xlinker -Map="WeatherStation.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "D:\Documentos\LPCXpresso\Git\WeatherStation\WeatherStation\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR\LPC1768.ld" -o "WeatherStation.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) WeatherStation.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "WeatherStation.axf"; arm-none-eabi-objcopy -O binary "WeatherStation.axf" "WeatherStation.bin" ; # checksum -p LPC1768 -d "WeatherStation.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
