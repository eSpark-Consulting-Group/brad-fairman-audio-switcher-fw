################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Libraries/Button/button.cpp 

OBJS += \
./Libraries/Button/button.o 

CPP_DEPS += \
./Libraries/Button/button.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/Button/%.o Libraries/Button/%.su Libraries/Button/%.cyclo: ../Libraries/Button/%.cpp Libraries/Button/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0plus -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G050xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/.Embedded/.GITHUB/bradfairman-audioswitcher-fw/Firmware/STM32/AudioSwitcher/Libraries/MAX14661" -I"C:/.Embedded/.GITHUB/bradfairman-audioswitcher-fw/Firmware/STM32/AudioSwitcher/Libraries/LM1972" -I"C:/.Embedded/.GITHUB/bradfairman-audioswitcher-fw/Firmware/STM32/AudioSwitcher/Libraries/Button" -I"C:/.Embedded/.GITHUB/bradfairman-audioswitcher-fw/Firmware/STM32/AudioSwitcher/Libraries/OutputControl" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libraries-2f-Button

clean-Libraries-2f-Button:
	-$(RM) ./Libraries/Button/button.cyclo ./Libraries/Button/button.d ./Libraries/Button/button.o ./Libraries/Button/button.su

.PHONY: clean-Libraries-2f-Button

