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

    expander->fd = wiringPiI2CSetup(I2Caddress);
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

    if(expander->fd == -1){
        printf("[ERROR] - Cannot initialize expander with %d Address\n", I2Caddress);
        return;
    }

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
        uint16_t clockPin){

    shifter->dataPin = dataPin;
    shifter->clockPin = clockPin;
    
    if(wiringPiSetupGpio() == -1) {
        printf("[ERROR] - Cannot initialize shifter\n");
        return;
    }

    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);

    pinMode(dataPin, LOW);
    pinMode(clockPin, LOW);

    return;
}

void initCubeSystem(CubeSystem* system){
    // initialize Cube with 0's
    memset(system->Cube.ledValues, 0, sizeof(system->Cube.ledValues));
    memset(system->Cube.toUpdateVal, 0, sizeof(system->Cube.toUpdateVal));

    // initialize expanders
    initExpander(&system->Expander1, MCP23017_I2C_ADDRESS_1, OUTPUTTYPE, IODIRA, IODIRB, GPIOA, GPIOB, OLATA, OLATB, 0x00, 0x00);
    initExpander(&system->Expander2, MCP23017_I2C_ADDRESS_2, OUTPUTTYPE, IODIRA, IODIRB, GPIOA, GPIOB, OLATA, OLATB, 0x00, 0x00);
    initExpander(&system->Expander3, MCP23017_I2C_ADDRESS_3, OUTPUTTYPE, IODIRA, IODIRB, GPIOA, GPIOB, OLATA, OLATB, 0x00, 0x00);

    // initialize shifters
    initShifter(&system->Shifter1, DATA_PIN, CLOCK_PIN);
    //initShifter(&system->Shifter2, DATA_PIN, CLOCK_PIN);
}

void setExpanderVal(Expander* expander, uint16_t data){
    // update expander values 
    expander->valueGPA = data & 0xFF;
    expander->valueGPB = (data >> 8) & 0xFF;

    if(wiringPiI2CWriteReg16(expander->fd, expander->portGPIOA, data) == -1){
        printf("Error \n");
    }
}

void setShifterVal(Shifter* shifter, uint16_t data){
    printf("data: %d \n", data);
    if (data >=16 || data <= 0){
        printf("[ERROR] - Trying to write invalid data on Shifter\n");
        return;
    }
    digitalWrite(shifter->dataPin, data);
    return;
}

void clockPulse(Shifter* shifter, useconds_t utime){
    digitalWrite(shifter->clockPin, HIGH);
    usleep(utime);
    digitalWrite(shifter->clockPin, LOW);
    usleep(utime);
}
