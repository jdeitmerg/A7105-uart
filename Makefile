CC = avr-gcc
CC_ARGS = -Wall -O1 -lm
OBJCOPY = avr-objcopy
SRC_DIRS = src/{avr-softuart,A7105-uart}
BUILD_DIR = bin

# For In-System-Programming:
PG_TYPE = stk500
PG_PORT = /dev/serial/by-id/*mySmartUSBlight*
PG_TARGET = t85

# For serial programming using fastboot:
SER_DEV = /dev/serial/by-id/*USB-Serial_Controller*
SER_BAUD = 9600

MMCU = attiny85
FCPU = 8000000

CC_ARGS += -DF_CPU=$(FCPU)UL

all: $(BUILD_DIR)/main.elf

# Default programming method
burn: serial_burn

isp_check:
	avrdude -p $(PG_TARGET) -c $(PG_TYPE) -P $(PG_PORT)

isp_fuses:
	# Internal 8MHz Oscillator, don't divide clock by 8, self
	# programming enabled
	# Extended may read back 0x00!
	avrdude -p $(PG_TARGET) -c $(PG_TYPE) -P $(PG_PORT) -U\
		lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xfe:m

isp_burn: $(BUILD_DIR)/main.elf
	avrdude -p $(PG_TARGET) -c $(PG_TYPE) -P $(PG_PORT) -U\
		flash:w:$<

serial_burn: $(BUILD_DIR)/main.hex
	avr-FBoot -d $(SER_DEV) -b $(SER_BAUD) -p $<


$(BUILD_DIR)/main.hex: $(BUILD_DIR)/main.elf
	$(OBJCOPY) -O ihex $< $@

# The "shell ls" evaluation is necessary to find the files in all the
# different source directories.
$(BUILD_DIR)/main.elf: $(shell ls $(SRC_DIRS)/*.c $(SRC_DIRS)/*.h)
	@test -d $(BUILD_DIR) || (mkdir $(BUILD_DIR) && echo -e "Created\
		$(BUILD_DIR)/ directory")
	@# $(^:%.h=) leaves out all .h files in the list of prequisites
	$(CC) $(CC_ARGS) -mmcu=$(MMCU) -o $@ $(^:%.h=)

clean:
	rm -f $(BUILD_DIR)/*
