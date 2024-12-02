# Variables for source and destination directories
SRC_DIR = src
DST_DIR = /home/pi/led_tower/src
BUILD_DIR = /home/pi/led_tower/build
FINAL_DIR = /led_tower/build

# Files to copy and compile
FILES = $(wildcard $(SRC_DIR)/*.cpp)
TARGET = $(BUILD_DIR)/main
IP2 = 10.227.113.138
IP = 192.168.175.94

# Send the source files to the Raspberry Pi project directory
send:
	@echo "Ensuring target directories exist on Raspberry Pi..."
	ssh pi@$(IP) "mkdir -p $(DST_DIR) $(BUILD_DIR)"
	@echo "Copying files from $(SRC_DIR) to $(DST_DIR)..."
	scp $(FILES) pi@$(IP):$(DST_DIR)
	@echo "Files copied successfully."

compile:
	@echo "--------------------Compiling source files...------------------------" 
	ssh pi@$(IP) "g++ $(DST_DIR)/*.cpp -lwiringPi -o $(BUILD_DIR)/main"
	@echo "Compilation successful."

#run:
#	@ssh -t pi@$(IP) '.$(FINAL_DIR)/main'
#	@echo "Execution finished."

all: send compile

