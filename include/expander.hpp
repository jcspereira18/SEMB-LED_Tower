#ifndef EXPANDER_HPP
#define EXPANDER_HPP

#include <wiringPi.h>
#include <wiringPiI2C.h>

// I2C Configuration
#define MCP23017_ADDRESS_1 0x24
#define MCP23017_ADDRESS_2 0x26
#define MCP23017_ADDRESS_3 0x27 

// MCP23017 Registers
#define IODIRA 0x00  // Registro de direção do Port A
#define IODIRB 0x01  // Registro de direção do Port B
#define GPIOA 0x12   // Registro GPIO Port A
#define GPIOB 0x13   // Registro GPIO Port B
#define OLATA 0x14   // Registro de latch de saída Port A
#define OLATB 0x15   // Registro de latch de saída Port B


void* initExpander(void* arg);

#endif // EXPANDER_HPP
