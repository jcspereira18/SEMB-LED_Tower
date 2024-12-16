#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "init_comp.hpp"

void setExpanderVal(Expander *expander, uint16_t data);
void printBinary(uint16_t value);
void setShifterVal(Shifter *shifter, uint16_t data);
void clockPulse(Shifter *shifter, useconds_t utime);
uint16_t readRegisters(Expander *expander);
void readButtons(Expander *expander);

#endif // TOOLS_HPP
