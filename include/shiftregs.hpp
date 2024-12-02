#ifndef SHIFTREGS_HPP
#define SHIFTREGS_HPP

#include <wiringPi.h>
#include <unistd.h>

// GPIO Pins para 74HC164
#define DATA_PIN 17
#define CLOCK_PIN 4

// Timing
#define CLOCK_DELAY 100000  // 100ms em microssegundos

void* initShiftRegs(void* arg);
#endif // SHIFTREGS_HPP
