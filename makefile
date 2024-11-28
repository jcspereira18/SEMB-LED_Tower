# Set the source directory, output binary name, and build directory
SRC_DIR = src
BUILD_DIR = build
OUTPUT = led_tower
TARGET_DIR = led_cube

# Set the Raspberry Pi target details
PI_USER = pi
PI_HOST = 192.168.175.94
PI_PATH = /home/pi

# Set the flags for compiling
CFLAGS = -Wall
LDFLAGS = -lwiringPi

# Find all .c source files in the src directory
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Find the corresponding object files in the build directory
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target: compile and run the program
.PHONY: all run clean runp

# Create the build directory if it doesn't exist locally
$(shell mkdir -p $(BUILD_DIR))

# Default build target to compile the program locally
all: $(OUTPUT)

# Compile the program from source files locally
$(OUTPUT): $(SOURCES)
	$(CC) $(SOURCES) -o $(OUTPUT) $(LDFLAGS)

# This target will compile everything remotely on the Raspberry Pi
runp: $(OUTPUT)
	@# Create led_cube directory structure on Raspberry Pi (including build and src directories)
	@ssh $(PI_USER)@$(PI_HOST) 'mkdir -p $(PI_PATH)/$(TARGET_DIR)/build $(PI_PATH)/$(TARGET_DIR)/src'

	@# Copy the source files to the Raspberry Pi under led_cube/src (only src, not src/src)
	@rsync -avz $(SRC_DIR)/ $(PI_USER)@$(PI_HOST):$(PI_PATH)/$(TARGET_DIR)/src/

	@# SSH into the Raspberry Pi to compile the files remotely
	@ssh $(PI_USER)@$(PI_HOST) 'cd $(PI_PATH)/$(TARGET_DIR)/src && $(CC) *.c -o $(PI_PATH)/$(TARGET_DIR)/build/$(OUTPUT) $(LDFLAGS)'

	@# Optionally, run the compiled program on Raspberry Pi from led_cube/build directory
	@ssh $(PI_USER)@$(PI_HOST) 'cd $(PI_PATH)/$(TARGET_DIR)/build && ./$(OUTPUT)'
	
	rm -rf $(OUTPUT) $(BUILD_DIR)

# Clean the object files and the output binary (on the Raspberry Pi)
clean:
	@ssh $(PI_USER)@$(PI_HOST) 'rm -f $(PI_PATH)/$(TARGET_DIR)/build/$(OUTPUT) && rm -f $(PI_PATH)/$(TARGET_DIR)/build/*.o'

