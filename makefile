# makefile

# files
LIB_SOURCE = display_7seg.c
EXAMPLE_SOURCE = example.c
TARGET = example

# macros
CC = avr-gcc
MCU = atmega328p
F_CPU = 16000000UL  # frequency (MHz)
BAUD = 9600
CFLAGS = -Wall -g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD)

# rules
all: $(TARGET).hex

# link the final program
$(TARGET).hex: $(TARGET).elf
	@echo "linking..."
	@avr-objcopy -O ihex -R .eeprom $< $@
	@echo "[+] done!"

# compile
$(TARGET).elf: $(EXAMPLE_SOURCE) $(LIB_SOURCE)
	@echo "compiling $@..."
	@$(CC) $(CFLAGS) -o $@ $^ -L.

# upload to arduino (change if you use other platform)
upload: $(TARGET).hex
	@echo "uploading..."
	@sudo avrdude -c arduino -p $(MCU) -P /dev/ttyUSB0 -b 115200 -U flash:w:$<
	@echo "[+] done!"

# clean directory
clean:
	@echo "cleaning..."
	@rm -f $(TARGET).elf $(TARGET).hex

.PHONY: all upload clean
