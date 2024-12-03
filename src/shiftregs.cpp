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
//void shiftRegisterCycle() {
//    for (int clockCount = 0; clockCount < 16; ++clockCount) {
//        int init = clockCount % 2;  // 0 para par, 1 para ímpar
//        digitalWrite(DATA_PIN, init);
//        clockPulse();
//    }
//}

// -------------------------------- Threads --------------------------------

void* initShiftRegs(void* arg) {
    wiringPiSetupGpio();
    return nullptr;
}

void* cycleShiftRegs(void* arg){
    char i = 0; 
    while(1){
        digitalWrite(DATA_PIN, i);
        if(i == 16){
            i = 0;
            usleep(1'000'000 / 1'000); // TODO: Calibrate this value
            continue;
        }
        i++;
    }
}

void* shiftToNextCycle(void* arg, char& index){
    if(index == 16){
        index = 0;
        return nullptr;
    }
    index++;
    return nullptr;
}
