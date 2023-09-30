################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gpio_driver.c \
../Src/i2c_driver.c \
../Src/led.c \
../Src/main.c \
../Src/oskernel.c \
../Src/spi_driver.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/uart.c 

OBJS += \
./Src/gpio_driver.o \
./Src/i2c_driver.o \
./Src/led.o \
./Src/main.o \
./Src/oskernel.o \
./Src/spi_driver.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/uart.o 

C_DEPS += \
./Src/gpio_driver.d \
./Src/i2c_driver.d \
./Src/led.d \
./Src/main.d \
./Src/oskernel.d \
./Src/spi_driver.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F411VETx -DSTM32 -DSTM32F4 -DSTM32F411E_DISCO -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/gpio_driver.cyclo ./Src/gpio_driver.d ./Src/gpio_driver.o ./Src/gpio_driver.su ./Src/i2c_driver.cyclo ./Src/i2c_driver.d ./Src/i2c_driver.o ./Src/i2c_driver.su ./Src/led.cyclo ./Src/led.d ./Src/led.o ./Src/led.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/oskernel.cyclo ./Src/oskernel.d ./Src/oskernel.o ./Src/oskernel.su ./Src/spi_driver.cyclo ./Src/spi_driver.d ./Src/spi_driver.o ./Src/spi_driver.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

