#include "../include/shiftregs.hpp"

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

// -------------------------------- Threads --------------------------------

void* initShiftRegs(void* arg) {
    wiringPiSetupGpio();

    return nullptr;
}
