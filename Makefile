CC = avr-gcc
OBJCOPY = avr-objcopy

TARGET = main
CFLAGS = -std=gnu99 -mmcu=attiny20
SOURCES=$(wildcard *.c)
OBJ = $(SOURCES:.c=.o)

all: $(SOURCES)
	$(CC) -c $(CFLAGS) $(SOURCES) -o $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET).elf build/$(TARGET).hex
	rm *.elf
clean:
	rm *.hex
