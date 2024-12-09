# Variables for source and destination directories
SRC_DIR = src
INC_DIR = include
DST_DIR = /home/pi/led_tower/src
BUILD_DIR = /home/pi/led_tower/build
INCLUDE_DIR = /home/pi/led_tower/include
FINAL_DIR = /led_tower/build

# Files to copy and compile
CPPFILES = $(wildcard $(SRC_DIR)/*.cpp)
HPPFILES = $(wildcard $(INC_DIR)/*.hpp)
TARGET = $(BUILD_DIR)/main
IP2 = 10.227.113.138
IP = 192.168.20.94

# Send the source files to the Raspberry Pi project directory
# These functions are meant to be executed through ssh

.DEFAULT_GOAL := default

sends:
	@echo "Ensuring target directories exist on Raspberry Pi..."
	ssh pi@$(IP) "mkdir -p $(DST_DIR) $(BUILD_DIR) $(INCLUDE_DIR)"
	@echo "Copying files from PC to RASP"
	scp $(CPPFILES) pi@$(IP):$(DST_DIR)
	scp $(HPPFILES) pi@$(IP):$(INCLUDE_DIR)
	scp makefile pi@$(IP):/home/pi/led_tower
	@echo "Files copied successfully."

compiles:
	@echo "--------------------Compiling source files...------------------------" 
	ssh pi@$(IP) "g++ -Iinclude  $(DST_DIR)/*.cpp -o $(BUILD_DIR)/main -lwiringPi"
	@echo "Compilation successful."

runs:
	@ssh -t pi@$(IP) '.$(FINAL_DIR)/main'
	@echo "Execution finished."

cleans:
	@ssh -t pi@$(IP) 'rm -rf led_tower'

alls: sends compiles runs

# These functions are meant to be executed inside the PI
compile:
	g++ src/*.cpp -Iinclude -lwiringPi -o build/main

run:
	./build/main

all: compile run

default: all
