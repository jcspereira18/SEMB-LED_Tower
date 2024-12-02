#include <iostream>
#include "../include/expander.hpp"

// Configurar todos os pinos dos Port A e Port B como saída
void setupExpander(int fd) {
    wiringPiI2CWriteReg8(fd, IODIRA, 0x00);  // Todos os pinos do Port A como saída
    wiringPiI2CWriteReg8(fd, IODIRB, 0x00);  // Todos os pinos do Port B como saída
}

// Função para configurar GPIO do MCP23017
void setGPIO_expander(int fd, char port, uint8_t value) {
    if (port == 'A') {
        wiringPiI2CWriteReg8(fd, OLATA, value);
    } else if (port == 'B') {
        wiringPiI2CWriteReg8(fd, OLATB, value);
    } else {
        std::cerr << "Porta inválida. Use 'A' ou 'B'.\n";
    }
}

// -------------------------------- Threads --------------------------------

void* initExpander(void* arg) {

    // Inicializar barramento I2C
    int fd_1 = wiringPiI2CSetup(MCP23017_ADDRESS_1);
    int fd_2 = wiringPiI2CSetup(MCP23017_ADDRESS_2);
    int fd_3 = wiringPiI2CSetup(MCP23017_ADDRESS_3);
    
    if (fd_1 == -1 || fd_2 == -1 || fd_3 == -1) {
        std::cerr << "Erro ao inicializar I2C.\n";
        return nullptr;
    }
    
    // Pinos como saída
    setupExpander(fd_1);
    setupExpander(fd_2);
    setupExpander(fd_3);

    setGPIO_expander(fd_1, 'A', 0x00);  
    setGPIO_expander(fd_1, 'B', 0x00);   
    setGPIO_expander(fd_2, 'A', 0x00);   
    setGPIO_expander(fd_2, 'B', 0x00);   
    setGPIO_expander(fd_3, 'A', 0x00);  
    setGPIO_expander(fd_3, 'B', 0x00);

    return nullptr;
}

