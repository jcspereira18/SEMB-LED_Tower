#include <stdio.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#include "../include/system.hpp"

void initExpander(
        Expander* expander,
        int I2Caddress,
        bool InOutType,
        uint16_t IOdirA,
        uint16_t IOdirB,
        uint16_t portGPIOA,
        uint16_t portGPIOB,
        uint16_t portOutLatchA,
        uint16_t portOutLatchB,
        uint8_t InitialValueGPA,
        uint8_t InitialValueGPB){
    if(expander == NULL){
        printf("[ERROR] - Invalid parameter while initializing expander\n");
        return;
    }

    expander->fd = wiringPiI2CSetup(I2Caddress);
    if(expander->fd == -1){
        printf("[ERROR] - Cannot initialize expander with %d Address\n", I2Caddress);
        return;
    }
    expander->type = InOutType;
    expander->IOdirA = IOdirA;
    expander->IOdirB = IOdirB;
    expander->I2CAddress = I2Caddress;
    expander->portGPIOA = portGPIOA;
    expander->portGPIOB = portGPIOB;
    expander->portOutLatchA = portOutLatchA;
    expander->portOutLatchB = portOutLatchB;
    expander->valueGPA = InitialValueGPA;
    expander->valueGPB = InitialValueGPB;

    // Settings ports in expander as outputs
    if(wiringPiI2CWriteReg8(expander->fd, IOdirA, InOutType) == -1) {
        printf("[ERROR] - Cannot set default expander type to IOdirA\n");
        return;
    }
    if(wiringPiI2CWriteReg8(expander->fd, IOdirB, InOutType) == -1) {
        printf("[ERROR] - Cannot set default expander type to IOdirB\n");
        return;
    }

    // Set ports outputs to initial values
    setExpanderVal(expander, (uint16_t(InitialValueGPB) << 8) || InitialValueGPA);
}

void initShifter(
        Shifter* shifter,
        uint16_t dataPin,
        uint16_t clockPin,
        uint16_t initialData){

    if(shifter == NULL){
        printf("[ERROR] - Invalid parameter while initializing shifter\n");
        return;
    }

    shifter->dataPin = dataPin;
    shifter->clockPin = clockPin;

//    if(wiringPiSetupGpio() == -1) {
//        printf("[ERROR] - Cannot initialize shifter\n");
//        return;
//    }
    wiringPiSetupGpio();
    printf("dataPin: %d\n clockPin: %d\nOutput: %d\n LOW: %d\n", shifter->dataPin, shifter->clockPin, OUTPUT, LOW);
    pinMode(shifter->dataPin, OUTPUT);
    pinMode(shifter->clockPin, OUTPUT);
    digitalWrite(shifter->dataPin, LOW);
    digitalWrite(shifter->clockPin, LOW);

    // Assumming it starts with 0
    //setShifterVal(shifter, initialData);
    // printf("[INFO] - Shifter has the correct initial value\n");

    return;
}

void initCubeSystem(CubeSystem* system){
    if(system == NULL){
        printf("[ERROR] - Invalid parameter while initializing cube system\n");
        return;
    }
    printf("[INFO] - Starting initialization of cubeSystem\n");
    // initialize Cube with 0's
    memset(system->Cube.ledValues, 0, sizeof(system->Cube.ledValues));
    memset(system->Cube.toUpdateVal, 0, sizeof(system->Cube.toUpdateVal));
    
    printf("[INFO] - Done initializing ledCubeArrays\n");
    // initialize expanders
    initExpander(&system->Expander1, MCP23017_I2C_ADDRESS_1, OUTPUTTYPE, IODIRA, IODIRB, GPIOA, GPIOB, OLATA, OLATB, 0x00, 0x00);
    initExpander(&system->Expander2, MCP23017_I2C_ADDRESS_2, OUTPUTTYPE, IODIRA, IODIRB, GPIOA, GPIOB, OLATA, OLATB, 0x00, 0x00);
    initExpander(&system->Expander3, MCP23017_I2C_ADDRESS_3, OUTPUTTYPE, IODIRA, IODIRB, GPIOA, GPIOB, OLATA, OLATB, 0x00, 0x00);

    printf("[INFO] - Done initializing all expanders\n");
    // initialize shifters
    initShifter(&system->Shifter1, DATA_PIN, CLOCK_PIN, 0x0000);
    printf("[INFO] - Shifter has been initialized correctly\n");
    //initShifter(&system->Shifter2, DATA_PIN, CLOCK_PIN);
    return;
}

void setExpanderVal(Expander* expander, uint16_t data){
    if(expander == NULL){
        printf("[ERROR] - Invalid parameter while setting values to expander\n");
        return;
    }
    
    expander->valueGPA = data & 0xFF;
    expander->valueGPB = (data >> 8) & 0xFF;

    if(wiringPiI2CWriteReg16(expander->fd, expander->portGPIOA, data) == -1){
        printf("Error \n");
    }
    return;
}

void printBinary(uint16_t value){
    for(int i = 15; i >= 0; i--){
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
    return;
}

void setShifterVal(Shifter* shifter, uint16_t data){
    if(shifter == NULL){
        printf("[ERROR] - Invalid parameter in setShifterVal\n");
        return;
    }
    if (data > 0b1111'1111'1111'1111 || data < 0b0000'0000'0000'0000){
        printf("[ERROR] - Trying to write invalid data on Shifter\n");
        printf("[ERROR] - Trying to write: ");
        printBinary(data);
        return;
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
        return;
    }
    digitalWrite(shifter->clockPin, HIGH);
    usleep(utime);
    digitalWrite(shifter->clockPin, LOW);
    return;
}
