################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Common/Src/buttons.c \
../Common/Src/imu.c \
../Common/Src/move.c \
../Common/Src/wheel.c 

OBJS += \
./Common/Src/buttons.o \
./Common/Src/imu.o \
./Common/Src/move.o \
./Common/Src/wheel.o 

C_DEPS += \
./Common/Src/buttons.d \
./Common/Src/imu.d \
./Common/Src/move.d \
./Common/Src/wheel.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Src/%.o: ../Common/Src/%.c Common/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32WB35xx -c -I../Core/Inc -I../Common/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc -I../Drivers/STM32WBxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../Drivers/CMSIS/Include -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Common-2f-Src

clean-Common-2f-Src:
	-$(RM) ./Common/Src/buttons.d ./Common/Src/buttons.o ./Common/Src/imu.d ./Common/Src/imu.o ./Common/Src/move.d ./Common/Src/move.o ./Common/Src/wheel.d ./Common/Src/wheel.o

.PHONY: clean-Common-2f-Src

