import RPi.GPIO as GPIO
import smbus2
import time

# I2C Configuration
I2C_BUS = 1  # Typically 1 on Raspberry Pi
MCP23017_ADDRESS = 0x24  # 24, 26, 27

# MCP23017 Registers
IODIRA = 0x00  # I/O Direction Register for Port A
IODIRB = 0x01  # I/O Direction Register for Port B
GPIOA = 0x12   # GPIO Port A Register
GPIOB = 0x13   # GPIO Port B Register
OLATA = 0x14   # Output Latch Register for Port A
OLATB = 0x15   # Output Latch Register for Port B

# GPIO Pins for 74HC164
DATA_PIN = 17
CLOCK_PIN = 4

#Timing
CLOCK_DELAY = 0.1


def setup_expander(bus):
    """
    Initialize the MCP23017 expander.
    Set all pins on Port A and Port B as outputs.
    """
    # Set all pins of Port A and Port B as outputs (0x00 means all pins are outputs)
    bus.write_byte_data(MCP23017_ADDRESS, IODIRA, 0x00)  # Set Port A as output
    bus.write_byte_data(MCP23017_ADDRESS, IODIRB, 0x00)  # Set Port B as output

def set_gpio(bus, port, value):
    """
    Set GPIO pins on Port A or Port B.
    :param bus: SMBus object.
    :param port: 'A' for Port A or 'B' for Port B.
    :param value: 8-bit value to write to the port (e.g., 0xFF turns all pins on).
    """
    if port == 'A':
        bus.write_byte_data(MCP23017_ADDRESS, OLATA, value)
    elif port == 'B':
        bus.write_byte_data(MCP23017_ADDRESS, OLATB, value)
    else:
        raise ValueError("Port must be 'A' or 'B'.")
    
def setup_gpio_pins():
    """
    Setup the GPIO pins for the 74HC164 shift register.
    """
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(DATA_PIN, GPIO.OUT)
    GPIO.setup(CLOCK_PIN, GPIO.OUT)
    
    GPIO.output(DATA_PIN, GPIO.LOW)
    GPIO.output(CLOCK_PIN, GPIO.LOW)


def clock_pulse():
    """
    Generate a single clock pulse.
    """
    GPIO.output(CLOCK_PIN, GPIO.HIGH)
    time.sleep(CLOCK_DELAY / 2)
    GPIO.output(CLOCK_PIN, GPIO.LOW)
    time.sleep(CLOCK_DELAY / 2)

def shift_register_cycle():
    """
    Perform a cycle of 16 clock pulses, alternating 'init' value:
    - Even clock: Data (init) = 0
    - Odd clock: Data (init) = 1
    """
    for clock_count in range(16):
        init = clock_count % 2  # 0 for even, 1 for odd
        GPIO.output(DATA_PIN, init)  # Set data pin
        clock_pulse()  # Generate clock pulse


def main():
    # Initialize the I2C bus
    bus = smbus2.SMBus(I2C_BUS)
    
    # Setup the MCP23017
    setup_expander(bus)
    
    # Example: Turn on all LEDs connected to Port A
    set_gpio(bus, 'A', 0xFF)  # Set all pins on Port A to HIGH
    # Example: Turn off all LEDs connected to Port B
    set_gpio(bus, 'B', 0x00)  # Set all pins on Port B to LOW
    
    try:
        setup_gpio_pins()
        while True:
            print("Starting a new cycle...")
            shift_register_cycle()
            time.sleep(1)  # Wait before starting the next cycle
    except KeyboardInterrupt:
        print("Exiting program.")
    finally:
        GPIO.cleanup()  # Reset GPIO pins on exit
    
    # Example: Create a chasing effect on Port A LEDs
    
    for i in range(8):
        value = 1 << i  # Shift a bit to create a moving "ON" LED
        set_gpio(bus, 'A', value)
        time.sleep(0.2)
    
    # Cleanup
    bus.close()

if __name__ == "__main__":
    main()
