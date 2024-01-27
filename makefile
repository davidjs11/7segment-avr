# makefile

SOURCE = display_7seg.c
TARGET = display

CC = avr-gcc
MCU = atmega328p
F_CPU = 16000000UL  # frequency (MHz)
BAUD = 9600
CFLAGS = -Wall -g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD)

# rules
all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

$(TARGET).elf: $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $<

upload: $(TARGET).hex
	sudo avrdude -c arduino -p $(MCU) -P /dev/ttyUSB0 -b 115200 -U flash:w:$<

clean:
	rm -f $(TARGET).elf $(TARGET).hex

.PHONY: all upload clean
