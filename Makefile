CC=arm-none-eabi-gcc
IDIR1=../../libs/stm32-cube-f4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
IDIR2=../../libs/stm32-cube-f4/Drivers/CMSIS/Include 
DEBUGDIR=./debug/
LDIR=../../libs/stm32-cube-f4/Projects/STM32F401RE-Nucleo/Templates/TrueSTUDIO/STM32F4xx-Nucleo/
ASMDIR=./system/
CFLAGS=-Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -mthumb-interwork -DSTM32F401xE -I$(IDIR1) -I$(IDIR2)
LDFLAGS=-Wl,--gc-sections

SOURCES=system.c main.c $(ASMDIR)startup_stm32f401xe.s
LIB=STM32F401CE_FLASH.ld
OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=main.elf

OBJCPY=arm-none-eabi-objcopy
OCFLAGS=-Oihex
HEX=main.hex

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)	
	$(CC) -T$(LDIR)$(LIB) $(LDFLAGS) $(OBJECTS) -o $(DEBUGDIR)$(EXECUTABLE)

.s.o:
	$(CC) $(CFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

hex:
	$(OBJCPY) $(OCFLAGS) $(DEBUGDIR)$(EXECUTABLE) $(DEBUGDIR)$(HEX)

clean: 
	rm -rf *o $(DEBUGDIR)$(EXECUTABLE) $(DEBUGDIR)$(HEX)