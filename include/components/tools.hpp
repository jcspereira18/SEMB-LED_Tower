#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "init_comp.hpp"
// TODO: create documentation here

void setExpanderVal(Expander *expander, uint16_t data);
void printBinary(uint16_t value);
void setShifterVal(Shifter *shifter, uint16_t data);
void clockPulse(Shifter *shifter, useconds_t utime);
uint16_t readExpander(Expander *e);
void readButtons(Expander *expander);
void goToNextcycle(Shifter *s);
bool debounceButton(Button *b, uint16_t reading);

#endif // TOOLS_HPP
