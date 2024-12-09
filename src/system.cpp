#include <stdio.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#include "../include/system.hpp"

void initExpander(
        Expander* expander,
        uint8_t I2CAddress,
        uint8_t IOdirA,
        uint8_t IOdirB,
        uint8_t portGPIOA,
        uint8_t portGPIOB,
        uint8_t portGPPUA,
        uint8_t portGPPUB,
        uint8_t inputsDirA,
        uint8_t inputsDirB,
        uint8_t initialValueGPA,
        uint8_t initialValueGPB){

    if(expander == NULL){
        printf("[ERROR] - Invalid parameter while initializing expander\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize MCP23017 on I2C address
    expander->I2CAddress = I2CAddress;
    expander->fd = wiringPiI2CSetup(expander->I2CAddress);
    if(expander->fd == -1){
        printf("[ERROR] - Cannot initialize expander with %d Address\n", expander->I2CAddress);
        exit(EXIT_FAILURE);
    }
    printf("[TEST] ------- %d\t%d\n", inputsDirA, inputsDirB);

    // Port A and B Setup
    // Set Port A direction Inputs and Outputs 
    expander->IOdirA = IOdirA;
    expander->IOdirB = IOdirB;
    expander->inputsDirA = inputsDirA;
    expander->inputsDirB = inputsDirB;
    if(wiringPiI2CWriteReg8(expander->fd, expander->IOdirA, expander->inputsDirA) == -1) {
        printf("[ERROR] - Cannot set IO for Port A direction\n");
        exit(EXIT_FAILURE);
    }
    if(wiringPiI2CWriteReg8(expander->fd, expander->IOdirB, expander->inputsDirB) == -1) {
        printf("[ERROR] - Cannot set IO for Port B direction\n");
        exit(EXIT_FAILURE);
    }
    // WARN: TEST PULL-UPS !!!!
    // Enable pull-ups
    expander->portGPPUA = portGPPUA;
    expander->portGPPUB = portGPPUB;
    expander->inputsDirA = inputsDirA;
    expander->inputsDirB = inputsDirB;
    if(wiringPiI2CWriteReg8(expander->fd, expander->portGPPUA, expander->inputsDirA) == -1){
        printf("[ERROR] - Cannot set IO for Port A direction\n");
        exit(EXIT_FAILURE);
    }
    if(wiringPiI2CWriteReg8(expander->fd, expander->portGPPUB, expander->inputsDirB) == -1){
        printf("[ERROR] - Cannot set IO for Port B direction\n");
        exit(EXIT_FAILURE);
    }

    expander->portGPIOA = portGPIOA;
    expander->portGPIOB = portGPIOB;

    // Set initial values
    setExpanderVal(expander, (uint16_t(initialValueGPA) << 8) || initialValueGPB);
    printf("[INFO] - Expander has been started with value: %d\n",(uint16_t(initialValueGPA) << 8) || initialValueGPB);
    printf("[INFO] - Sucessfully initialized expander\n");
    

    return;
}

void initShifter(
        Shifter* shifter,
        uint16_t dataPin,
        uint16_t clockPin,
        uint16_t initialData){

    if(shifter == NULL){
        printf("[ERROR] - Invalid parameter while initializing shifter\n");
        exit(EXIT_FAILURE);
    }

    shifter->dataPin = dataPin;
    shifter->clockPin = clockPin;

    if(wiringPiSetupGpio() == -1) {
        printf("[ERROR] - Cannot initialize shifter\n");
        exit(EXIT_FAILURE);
    }
    printf("dataPin: %d\n clockPin: %d\nOutput: %d\n LOW: %d\n", shifter->dataPin, shifter->clockPin, OUTPUT, LOW);
    pinMode(shifter->dataPin, OUTPUT);
    pinMode(shifter->clockPin, OUTPUT);
    digitalWrite(shifter->dataPin, LOW);
    digitalWrite(shifter->clockPin, LOW);

    return;
}

void initButton(Button* button,  uint16_t pin){
    if(button == NULL){
        printf("[ERROR] - Invalid parameter while initializing button \n");
        exit(EXIT_FAILURE);
    }

    if (pin > 0b1111'1111'1111'1111 || pin < 0b0000'0000'0000'0000){
        printf("[ERROR] - Trying give invalid pin to button\n");
        exit(EXIT_FAILURE);
    }

    // Initial value
    button->state = LOW;
    button->pin = pin;

    return;
}

void initCubeSystem(CubeSystem* system){
    if(system == NULL){
        printf("[ERROR] - Invalid parameter while initializing cube system\n");
        exit(EXIT_FAILURE);
    }
    // initialize Cube with 0's
    memset(system->Cube.ledValues, 0, sizeof(system->Cube.ledValues));
    memset(system->Cube.toUpdateVal, 0, sizeof(system->Cube.toUpdateVal));
    
    // initialize buttons
    initButton(&system->Button11, 0b0000'0001); // GPA0
    initButton(&system->Button12, 0b0000'0010); // GPA1
    initButton(&system->Button21, 0b0100'0000); // GPB6
    initButton(&system->Button22, 0b1000'0000); // GPB7
                                               
    initButton(&system->Button13, 0b0000'0001); // GPA0
    initButton(&system->Button14, 0b0000'0010); // GPA1
    initButton(&system->Button23, 0b0100'0000); // GPB6
    initButton(&system->Button24, 0b1000'0000); // GPB7

    initButton(&system->Button15, 0b0000'0001); // GPA0
    initButton(&system->Button16, 0b0000'0010); // GPA1
    initButton(&system->Button25, 0b0100'0000); // GPB6
    initButton(&system->Button26, 0b1000'0000); // GPB7
                                                
    // link expander to button
    system->Expander1.Button1 = &system->Button11;
    system->Expander1.Button2 = &system->Button12;
    system->Expander1.Button3 = &system->Button21;
    system->Expander1.Button4 = &system->Button22;

    system->Expander2.Button1 = &system->Button13;
    system->Expander2.Button2 = &system->Button14;
    system->Expander2.Button3 = &system->Button23;
    system->Expander2.Button4 = &system->Button24;

    system->Expander3.Button1 = &system->Button15;
    system->Expander3.Button2 = &system->Button16;
    system->Expander3.Button3 = &system->Button25;
    system->Expander3.Button4 = &system->Button26;

    // initialize expanders
    initExpander(
            &system->Expander1,
            MCP23017_I2C_ADDRESS_1,
            IODIRA,
            IODIRB,
            GPIOA,
            GPIOB,
            GPPUA,
            GPPUB,
            system->Button11.pin | system->Button12.pin,
            system->Button21.pin | system->Button22.pin,
            0x00,
            0x00
            );

    initExpander(
            &system->Expander2,
            MCP23017_I2C_ADDRESS_2,
            IODIRA,
            IODIRB,
            GPIOA,
            GPIOB,
            GPPUA,
            GPPUB,
            system->Button13.pin | system->Button14.pin,
            system->Button23.pin | system->Button24.pin,
            0x00,
            0x00
            );

    initExpander(
            &system->Expander3,
            MCP23017_I2C_ADDRESS_3,
            IODIRA,
            IODIRB,
            GPIOA,
            GPIOB,
            GPPUA,
            GPPUB,
            system->Button15.pin | system->Button16.pin,
            system->Button25.pin | system->Button26.pin,
            0x00,
            0x00
            );

    // initialize shifters
    initShifter(&system->Shifter1, DATA_PIN, CLOCK_PIN, 0x0000);
    printf("[INFO] - Successfully initialized system\n");

    return;
}

void setExpanderVal(Expander* expander, uint16_t data){
    if(expander == NULL){
        printf("[ERROR] - Invalid parameter while setting values to expander\n");
        exit(EXIT_FAILURE);
    }
    
    expander->valueGPA = data & 0xFF;
    expander->valueGPB = (data >> 8) & 0xFF;

    if(wiringPiI2CWriteReg16(expander->fd, expander->portGPIOA, data) == -1){
        printf("Error \n");
        exit(EXIT_FAILURE);
    }
    return;
}

void printBinary(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        printf("%d", (value >> i) & 1);

        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

void setShifterVal(Shifter* shifter, uint16_t data){
    if(shifter == NULL){
        printf("[ERROR] - Invalid parameter in setShifterVal\n");
        exit(EXIT_FAILURE);
    }
    if (data > 0b1111'1111'1111'1111 || data < 0b0000'0000'0000'0000){
        printf("[ERROR] - Trying to write invalid data on Shifter\n");
        printf("[ERROR] - Trying to write: ");
        printBinary(data);
        exit(EXIT_FAILURE);
    }
    int index = 0;
    bool resultBool = 0;
    for(int i = 0; i < 16; i++){
        resultBool = (data & (1 << (15 - i))) ? HIGH : LOW;
        printf("[INFO] - resultBool value: %d\n",resultBool);
        digitalWrite(shifter->dataPin, resultBool);
        clockPulse(shifter, 100);
    }
    shifter->data = data;
    printf("[INFO] - Value in shifter->data: %d\n", shifter->data);
    printf("[INFO] - Successfully wrote to shifter. Data: ");
    printBinary(shifter->data);

    printf("dataPin value: %d", shifter->dataPin);

    return;
}

void clockPulse(Shifter* shifter, useconds_t utime){
    if(shifter == NULL){
        printf("[ERROR] - Invalid parameter in clockPulse\n");
        exit(EXIT_FAILURE);
    }
    digitalWrite(shifter->clockPin, HIGH);
    usleep(utime);
    digitalWrite(shifter->clockPin, LOW);
    return;
}

uint16_t readRegisters(Expander* expander){
    if(expander == NULL){
        printf("[ERROR] - Invalid parameter in readRegisters\n");
        exit(EXIT_FAILURE);
    }

    printf("[AAAA] - %d\t%d\n", expander->fd, expander->portGPIOA);
    printf("[BBBB] - %d\n", wiringPiI2CReadReg8(expander->fd, expander->portGPIOA));
    
    return ((uint16_t)wiringPiI2CReadReg8(expander->fd, expander->portGPIOB) << 8)|
        wiringPiI2CReadReg8(expander->fd, expander->portGPIOA);
}
