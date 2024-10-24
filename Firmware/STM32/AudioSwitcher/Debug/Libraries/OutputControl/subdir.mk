################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Libraries/OutputControl/outputControl.cpp 

OBJS += \
./Libraries/OutputControl/outputControl.o 

CPP_DEPS += \
./Libraries/OutputControl/outputControl.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/OutputControl/%.o Libraries/OutputControl/%.su Libraries/OutputControl/%.cyclo: ../Libraries/OutputControl/%.cpp Libraries/OutputControl/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0plus -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G050xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/.Embedded/.GITHUB/bradfairman-audioswitcher-fw/Firmware/STM32/AudioSwitcher/Libraries/MAX14661" -I"C:/.Embedded/.GITHUB/bradfairman-audioswitcher-fw/Firmware/STM32/AudioSwitcher/Libraries/LM1972" -I"C:/.Embedded/.GITHUB/bradfairman-audioswitcher-fw/Firmware/STM32/AudioSwitcher/Libraries/Button" -I"C:/.Embedded/.GITHUB/bradfairman-audioswitcher-fw/Firmware/STM32/AudioSwitcher/Libraries/OutputControl" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libraries-2f-OutputControl

clean-Libraries-2f-OutputControl:
	-$(RM) ./Libraries/OutputControl/outputControl.cyclo ./Libraries/OutputControl/outputControl.d ./Libraries/OutputControl/outputControl.o ./Libraries/OutputControl/outputControl.su

.PHONY: clean-Libraries-2f-OutputControl

