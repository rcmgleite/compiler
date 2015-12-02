INCLUDE_PATH="./src/include"

C_FILES := $(wildcard ./src/source/*.c)
O_FILES := $(C_FILES:.c=.o)
C_DEPS := $(C_FILES:.c=.d)

ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif

# Compile all files from src/source
src/source/%.o: src/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I$(INCLUDE_PATH) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# All Target
all: compiler

# Compiler Target
compiler: $(O_FILES)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc  -o "compiler" $(O_FILES)
	@echo 'Finished building target: $@'
	@echo ' '

# Clean Target
clean:
	-$(RM) $(O_FILES) $(C_DEPS) compiler
	-@echo ' '

.PHONY: all clean
