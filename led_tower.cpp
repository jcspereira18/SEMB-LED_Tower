#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <unistd.h>

// I2C Configuration
#define I2C_BUS 1  // Normalmente 1 no Raspberry Pi
#define MCP23017_ADDRESS 0x24  // Endereço do MCP23017

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
void setGPIO(int fd, char port, uint8_t value) {
    if (port == 'A') {
        wiringPiI2CWriteReg8(fd, OLATA, value);
    } else if (port == 'B') {
        wiringPiI2CWriteReg8(fd, OLATB, value);
    } else {
        std::cerr << "Porta inválida. Use 'A' ou 'B'.\n";
    }
}

// Configuração inicial do GPIO do 74HC164
void setupGPIOPins() {
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
    int fd = wiringPiI2CSetup(MCP23017_ADDRESS);
    if (fd == -1) {
        std::cerr << "Erro ao inicializar I2C.\n";
        return -1;
    }

    // Configurar o MCP23017
    setupExpander(fd);

    // Exemplo: Ativar todos os LEDs conectados ao Port A
    setGPIO(fd, 'A', 0xFF);  // Configurar todos os pinos do Port A como HIGH
    // Exemplo: Desativar todos os LEDs conectados ao Port B
    setGPIO(fd, 'B', 0x00);  // Configurar todos os pinos do Port B como LOW

    try {
        setupGPIOPins();
        while (true) {
            std::cout << "Iniciando um novo ciclo...\n";
            shiftRegisterCycle();
            sleep(1);  // Espera 1 segundo antes do próximo ciclo
        }
    } catch (...) {
        std::cout << "Saindo do programa.\n";
        digitalWrite(DATA_PIN, LOW);
        digitalWrite(CLOCK_PIN, LOW);
    }

    // Exemplo: Criar efeito "chasing" no Port A
    for (int i = 0; i < 8; ++i) {
        uint8_t value = 1 << i;  // Bit shift para criar um LED aceso em movimento
        setGPIO(fd, 'A', value);
        usleep(200000);  // 200ms
    }

    return 0;
}