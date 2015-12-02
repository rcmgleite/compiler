SUBDIRS := \
src/source \

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/source/lex.c \
../src/source/main.c \
../src/source/semantic.c \
../src/source/symbol_table.c \
../src/source/syntatic.c \
../src/source/tables.c \
../src/source/token.c 

OBJS += \
./src/source/lex.o \
./src/source/main.o \
./src/source/semantic.o \
./src/source/symbol_table.o \
./src/source/syntatic.o \
./src/source/tables.o \
./src/source/token.o 

C_DEPS += \
./src/source/lex.d \
./src/source/main.d \
./src/source/semantic.d \
./src/source/symbol_table.d \
./src/source/syntatic.d \
./src/source/tables.d \
./src/source/token.d 

# Each subdirectory must supply rules for building sources it contributes
src/source/%.o: src/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"/home/rafael/workspace/compiler/src/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: compiler

# Tool invocations
compiler: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc  -o "compiler" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) compiler
	-@echo ' '

.PHONY: all clean dependents
