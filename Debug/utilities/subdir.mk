################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_debug_console.c \
../utilities/fsl_io.c \
../utilities/fsl_log.c \
../utilities/fsl_str.c 

C_DEPS += \
./utilities/fsl_debug_console.d \
./utilities/fsl_io.d \
./utilities/fsl_log.d \
./utilities/fsl_str.d 

OBJS += \
./utilities/fsl_debug_console.o \
./utilities/fsl_io.o \
./utilities/fsl_log.o \
./utilities/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\board" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\source" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\drivers" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\CMSIS" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\utilities" -I"C:\Users\dacp1\Documents\01.Study\01.MSc\02.MScEE\01.Semester3\ESP\workspace\ESP-LAB2\ESP-LAB2\startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o ./utilities/fsl_io.d ./utilities/fsl_io.o ./utilities/fsl_log.d ./utilities/fsl_log.o ./utilities/fsl_str.d ./utilities/fsl_str.o

.PHONY: clean-utilities

