################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Drivers/CMSIS/DSP/Source/TransformFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/SupportFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/StatisticsFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/MatrixFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/FilteringFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/FastMathFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/ControllerFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/ComplexMathFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/CommonTables/subdir.mk
-include Drivers/CMSIS/DSP/Source/BasicMathFunctions/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

BUILD_ARTIFACT_NAME := ProjektPrzejsciowy
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ProjektPrzejsciowy.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ProjektPrzejsciowy.list \

OBJCOPY_BIN += \
ProjektPrzejsciowy.bin \


# All Target
all: main-build

# Main-build Target
main-build: ProjektPrzejsciowy.elf secondary-outputs

# Tool invocations
ProjektPrzejsciowy.elf: $(OBJS) $(USER_OBJS) D:\AM\STM32CubeIDE_1.3.0\Projekty\ProjektPrzejsciowy\STM32F746ZGTX_FLASH.ld
	arm-none-eabi-gcc -o "ProjektPrzejsciowy.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"D:\AM\STM32CubeIDE_1.3.0\Projekty\ProjektPrzejsciowy\STM32F746ZGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ProjektPrzejsciowy.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ProjektPrzejsciowy.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ProjektPrzejsciowy.list"
	@echo 'Finished building: $@'
	@echo ' '

ProjektPrzejsciowy.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ProjektPrzejsciowy.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
