################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/source/lex.c \
../src/source/main.c \
../src/source/token.c 

OBJS += \
./src/source/lex.o \
./src/source/main.o \
./src/source/token.o 

C_DEPS += \
./src/source/lex.d \
./src/source/main.d \
./src/source/token.d 


# Each subdirectory must supply rules for building sources it contributes
src/source/%.o: ../src/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/rafael/workspace/lexical_analyzer/src/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


