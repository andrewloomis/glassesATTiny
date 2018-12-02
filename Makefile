CC = avr-gcc
OBJCOPY = avr-objcopy

TARGET = main
CFLAGS = -std=gnu99 -Os -mmcu=attiny20
SOURCES=$(wildcard *.c)
OBJ = $(SOURCES:.c=.o)

all: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET).elf
	$(OBJCOPY) -O ihex $(TARGET).elf build/$(TARGET).hex
	rm *.elf
clean:
	rm build/*.hex

program:
	sudo avrdude -p t20 -C avrdude_gpio.conf -c linuxgpio -v