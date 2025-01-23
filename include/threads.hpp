#ifndef THREADS_HPP
#define THREADS_HPP

#include "components/init_comp.hpp"
#include <unistd.h>

/**
 * @brief Initialize following components:
 * 1 * Led array,
 * 3 * MCP23017,
 * 2 * 74HC164(Cascaded shift register composed by 2 shifters),
 * 12 * Buttons.
 */
void *createCubeSystem(void *args);

/**
 * @brief Set all components to its default value.
 */
void *globalReset(void *args);

/**
 * @brief Loop to turn on or off the leds.
 */
void *displayCube(void *args);

/**
 * @brief Update led array to create rain effect.
 */

void readButtons(CubeSystem *c);

void systemStateTransitions(CubeSystem *c);
void systemStateActions(CubeSystem *c);

void *updateSnakeDirection(void *arg);
void *newRainAnimation(void *arg);

void *micro1(void *args);
void *micro2(void *args);
void *micro3(void *args);
void *micro4(void *args);
#endif // THREADS_HPP
