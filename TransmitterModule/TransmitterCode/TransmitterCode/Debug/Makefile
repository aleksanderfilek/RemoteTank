################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../audio.c \
../main.c \
../motor.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
audio.o \
main.o \
motor.o

OBJS_AS_ARGS +=  \
audio.o \
main.o \
motor.o

C_DEPS +=  \
audio.d \
main.d \
motor.d

C_DEPS_AS_ARGS +=  \
audio.d \
main.d \
motor.d

OUTPUT_FILE_PATH +=TransmitterCode.elf

OUTPUT_FILE_PATH_AS_ARGS +=TransmitterCode.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
B:/Projekty/RemoteTank/RVC/RVC/Debug/libRVC.a

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./audio.o: .././audio.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"B:\Programy\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.7.374\include"  -Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega328p -B "B:\Programy\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.7.374\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"B:\Programy\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.7.374\include"  -Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega328p -B "B:\Programy\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.7.374\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./motor.o: .././motor.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"B:\Programy\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.7.374\include"  -Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega328p -B "B:\Programy\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.7.374\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 5.4.0
	$(QUOTE)B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="TransmitterCode.map" -Wl,--start-group -Wl,-lm -Wl,-lRVC  -Wl,--end-group -Wl,-L"B:\Projekty\RemoteTank\RVC\RVC\Debug" -Wl,-L"C:\Users\filek\Documents\GitHub\RemoteTank\RVC\RVC\Debug"  -Wl,--gc-sections -mmcu=atmega328p -B "B:\Programy\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.7.374\gcc\dev\atmega328p"  
	@echo Finished building target: $@
	"B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "TransmitterCode.elf" "TransmitterCode.hex"
	"B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "TransmitterCode.elf" "TransmitterCode.eep" || exit 0
	"B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "TransmitterCode.elf" > "TransmitterCode.lss"
	"B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "TransmitterCode.elf" "TransmitterCode.srec"
	"B:\Programy\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "TransmitterCode.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "TransmitterCode.elf" "TransmitterCode.a" "TransmitterCode.hex" "TransmitterCode.lss" "TransmitterCode.eep" "TransmitterCode.map" "TransmitterCode.srec" "TransmitterCode.usersignatures"
	