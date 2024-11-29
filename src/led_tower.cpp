#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <unistd.h>

// I2C Configuration
#define I2C_BUS 1  // Normalmente 1 no Raspberry Pi
#define MCP23017_ADDRESS_1 0x24  // Endereço do MCP23017
#define MCP23017_ADDRESS_2 0x26
#define MCP23017_ADDRESS_3 0x27 


// MCP23017 Registers
#define IODIRA 0x00  // Registro de direção do Port A
#define IODIRB 0x01  // Registro de direção do Port B
#define GPIOA 0x12   // Registro GPIO Port A
#define GPIOB 0x13   // Registro GPIO Port B
#define OLATA 0x14   // Registro de latch de saída Port A
#define OLATB 0x15   // Registro de latch de saída Port B

// GPIO Pins para 74HC164
#define DATA_PIN 17
#define CLOCK_PIN 4

// Timing
#define CLOCK_DELAY 100000  // 100ms em microssegundos

// Função para configurar o MCP23017
void setupExpander(int fd) {
    // Configurar todos os pinos dos Port A e Port B como saída
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

// Configuração inicial do GPIO do 74HC164
void setupGPIO_shifter() {
    wiringPiSetupGpio();  // Usa o esquema BCM
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);

    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
}

// Gerar um único pulso de clock
void clockPulse() {
    digitalWrite(CLOCK_PIN, HIGH);
    usleep(CLOCK_DELAY / 2);
    digitalWrite(CLOCK_PIN, LOW);
    usleep(CLOCK_DELAY / 2);
}

// Ciclo de shift register
void shiftRegisterCycle() {
    for (int clockCount = 0; clockCount < 16; ++clockCount) {
        int init = clockCount % 2;  // 0 para par, 1 para ímpar
        digitalWrite(DATA_PIN, init);
        clockPulse();
    }
}

int main() {
    // Inicializar o barramento I2C
    int fd_1 = wiringPiI2CSetup(MCP23017_ADDRESS_1);
    if (fd_1 == -1) {
        std::cerr << "Erro ao inicializar I2C.\n";
        return -1;
    }

    int fd_2 = wiringPiI2CSetup(MCP23017_ADDRESS_2);
    if (fd_2 == -1) {
        std::cerr << "Erro ao inicializar I2C.\n";
        return -1;
    }

    int fd_3 = wiringPiI2CSetup(MCP23017_ADDRESS_3);
    if (fd_3 == -1) {
        std::cerr << "Erro ao inicializar I2C.\n";
        return -1;
    }

    // Configurar o MCP23017
    setupExpander(fd_1);
    setupExpander(fd_2);
    setupExpander(fd_3);

    setGPIO_expander(fd_1, 'A', 0xFF);  // Configurar todos os pinos do Port A como HIGH
    setGPIO_expander(fd_1, 'B', 0x00);  // Configurar todos os pinos do Port A como HIGH
    
    setGPIO_expander(fd_2, 'A', 0x00);  // Configurar todos os pinos do Port A como HIGH
    setGPIO_expander(fd_2, 'B', 0x00);  // Configurar todos os pinos do Port A como HIGH
    
    setGPIO_expander(fd_3, 'A', 0x00);  // Configurar todos os pinos do Port A como HIGH
    setGPIO_expander(fd_3, 'B', 0x00);  // Configurar todos os pinos do Port B como LOW




    try {
        setupGPIO_shifter();
        //while (true) {
        std::cout << "Iniciando um novo ciclo...\n";
        //shiftRegisterCycle();
        sleep(1);  // Espera 1 segundo antes do próximo ciclo
        //}

        
        uint8_t value = 1 << 2;  // Bit shift para criar um LED aceso em movimento
        setGPIO_expander(fd_1, 'A', value); //2, 3, 4, 5, 6, 7
        setGPIO_expander(fd_1, 'B', value); // 0, 1, 2, 3, 4, 5

        setGPIO_expander(fd_2, 'A', value);
        setGPIO_expander(fd_2, 'B', value);

        setGPIO_expander(fd_3, 'A', value);
        setGPIO_expander(fd_3, 'B', value);

        
        printf("test\n");
        usleep(1000000);  // 200ms

    } catch (...) {
        std::cout << "Saindo do programa.\n";
        digitalWrite(DATA_PIN, LOW);
        digitalWrite(CLOCK_PIN, LOW);
    }

    // Exemplo: Criar efeito "chasing" no Port A
    //for (int i = 0; i < 8; ++i) {
    //uint8_t value = 1 << 0;  // Bit shift para criar um LED aceso em movimento
    //setGPIO_expander(fd, 'A', value);
    //printf("test\n");
    //usleep(1000000);  // 200ms
    //}

    return 0;
}