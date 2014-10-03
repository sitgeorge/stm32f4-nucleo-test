CC=arm-none-eabi-gcc
AS=arm-none-eabi-as

PROJNAME=main

IDIR1=../../libs/stm32-cube-f4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
IDIR2=../../libs/stm32-cube-f4/Drivers/CMSIS/Include 
DEBUGDIR=./debug/
LDIR=../../libs/stm32-cube-f4/Projects/STM32F401RE-Nucleo/Templates/TrueSTUDIO/STM32F4xx-Nucleo/
ASMDIR=./system/
CFLAGS=-Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -mthumb-interwork -DSTM32F401xE -I$(IDIR1) -I$(IDIR2)
LDFLAGS=-Wl,--gc-sections -Wl,-Map=$(DEBUGDIR)$(PROJNAME).map

CFLAGS+=-g

LIB=STM32F401CE_FLASH.ld

#SOURCES=system.c main.c delay.c $(ASMDIR)startup_stm32f401xe.s
#OBJECTS=system.o main.o delay.o $(ASMDIR)startup_stm32f401xe.o

SOURCES=system_stm32f4xx.c main.c delay.c $(ASMDIR)startup_stm32f401xe.s
OBJECTS=system_stm32f4xx.o main.o delay.o $(ASMDIR)startup_stm32f401xe.o

EXECUTABLE=$(PROJNAME).elf

OBJCPY=arm-none-eabi-objcopy
OCFLAGS=-Oihex
HEX=$(PROJNAME).hex

OPENOCD_PROC_FILE=flash.cfg

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)	
	$(CC) $(CFLAGS) -T$(LDIR)$(LIB) $(LDFLAGS) $(OBJECTS) -o $(DEBUGDIR)$(EXECUTABLE)

startup_stm32f401xe.o: $(ASMDIR)startup_stm32f401xe.s
	$(AS) $(CFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

hex:
	$(OBJCPY) $(OCFLAGS) $(DEBUGDIR)$(EXECUTABLE) $(DEBUGDIR)$(HEX)

#deploy: 
#	openocd -f /usr/local/share/openocd/scripts/board/st_nucleo_f401re.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash $(DEBUGDIR)$(HEX)" -c shutdown

clean: 
	rm -rf *o $(DEBUGDIR)$(EXECUTABLE) $(DEBUGDIR)$(HEX)