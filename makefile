# Variables for source and destination directories
SRC_DIR = src
INC_DIR = include
DST_DIR = /home/pi/led_tower/src
BUILD_DIR = /home/pi/led_tower/build
INCLUDE_DIR = /home/pi/led_tower/include
FINAL_DIR = /led_tower/build

# Files to copy and compile
CPPFILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/components/*.cpp) $(wildcard $(SRC_DIR)/modes/*.cpp)
HPPFILES = $(wildcard $(INC_DIR)/*.hpp) $(wildcard $(INC_DIR)/components/*.hpp) $(wildcard $(INC_DIR)/modes/*.hpp)

TARGET = $(BUILD_DIR)/main
IP = 10.227.113.138
IP2 = 192.168.104.94

# Send the source files to the Raspberry Pi project directory
# These functions are meant to be executed through ssh

.DEFAULT_GOAL := default

sends:
	@echo "Ensuring target directories exist on Raspberry Pi..."
	ssh pi@$(IP) "mkdir -p $(DST_DIR) $(DST_DIR)/components $(BUILD_DIR) $(INCLUDE_DIR) $(INCLUDE_DIR)/components"
	@echo "Copying files from PC to RASP"
	scp main.cpp pi@$(IP):/home/pi/led_tower
	scp $(wildcard $(SRC_DIR)/*.cpp) pi@$(IP):$(DST_DIR)
	scp $(wildcard $(SRC_DIR)/components/*.cpp) pi@$(IP):$(DST_DIR)/components
	scp $(wildcard $(SRC_DIR)/modes/*.cpp) pi@$(IP):$(DST_DIR)/modes
	scp $(wildcard $(INC_DIR)/*.hpp) pi@$(IP):$(INCLUDE_DIR)
	scp $(wildcard $(INC_DIR)/components/*.hpp) pi@$(IP):$(INCLUDE_DIR)/components
	scp $(wildcard $(INC_DIR)/modes/*.hpp) pi@$(IP):$(INCLUDE_DIR)/modes
	scp makefile pi@$(IP):/home/pi/led_tower
	@echo "Files copied successfully."

compiles:
	@echo "--------------------Compiling source files...------------------------"
	ssh pi@$(IP) "g++ -I$(INCLUDE_DIR) /home/pi/led_tower/main.cpp $(DST_DIR)/*.cpp $(DST_DIR)/components/*.cpp $(DST_DIR)/modes/*.cpp -o $(BUILD_DIR)/main -lwiringPi"
	@echo "Compilation successful."

runs:
	@ssh -t pi@$(IP) 'sudo .$(FINAL_DIR)/main'
	@echo "Execution finished."

cleans:
	@ssh -t pi@$(IP) 'rm -rf led_tower'

alls: sends compiles runs

# These functions are meant to be executed inside the PI
compile:
	g++ src/*.cpp -Iinclude -lwiringPi -o build/main

run:
	sudo ./build/main

all: compile run

default: all
