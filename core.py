import Rpi.GPIO as GPIO
import time
import threading

#code designed for a 6x6x6 LED cube

#SETUP
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

#setup GPIO output pins
pins = [
    2,  #pin3 SDA -> pin13 MCP23017
    3,  #pin5 SCL -> pin12 MCP23017
    4,  #pin7 CLK -> pin2 74HC164
    17, #pin11 INIT -> pin8 74HC164
]

transistors = []

for pin in pins:
    GPIO.setup(pin, GPIO.OUT)

#three-dimensional list containing LED point values
points = [[[0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]],
          [[0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]],
          [[0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]],
          [[0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]],
          [[0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]],
          [[0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, 0, 0]]]

class ShiftRegister():
    def __init__(self, datapin, clockpin, latchpin):
        self.datapin = datapin
        self.clockpin = clockpin
        self.latchpin = latchpin

    def clock(self, n):
        #input data
        GPIO.output(self.datapin, n)
        
        #clock clock (whee)
        GPIO.output(self.clockpin, 1)
        GPIO.output(self.clockpin, 0)
        
        #reset data pin
        GPIO.output(self.datapin, 0)

    def latch(self):
        #latch and reset register
        GPIO.output(self.latchpin, 1)
        GPIO.output(self.latchpin, 0)

    def clear(self):
        #clear and reset register by filling with low values
        for i in range(16):
            self.clock(0)
        self.latch()
        
#this class runs in a separate thread, continuously reading the 'points' list
#   and writing its values to the LED cube
class Multiplexer():
    def __init__(self):
        self.running = True
        self.register1 = ShiftRegister(10, 12, 13)
        self.register2 = ShiftRegister(22, 16, 18)

    #the second parameter has to exist for the thread to run, but does nothing
    def multiplex(self, p):
        while self.running:
            for y in range(len(points)):
                #turn previous layer off before updating LEDs
                #GPIO.output(transistors[y-1], 0)
                
                for x in range(len(points[y])):
                    for z in range(len(points[y][x])):
                        #pick which register to write to - each controls half
                        #   of the cube
                        if x < 2:
                            self.register2.clock(points[y][x][z])
                        else:
                            self.register1.clock(points[y][x][z])

                #write register values to cube and enable layer
                self.register1.latch()
                self.register2.latch()
                GPIO.output(transistors[y], 1)
                
                sleep(0.001)