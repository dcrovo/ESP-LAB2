################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Display.c \
../source/Time.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/Display.d \
./source/Time.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d 

OBJS += \
./source/Display.o \
./source/Time.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\board" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\source" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\drivers" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\CMSIS" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\utilities" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Display.d ./source/Display.o ./source/Time.d ./source/Time.o ./source/main.d ./source/main.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

