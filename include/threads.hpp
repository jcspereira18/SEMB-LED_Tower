#ifndef THREADS_HPP
#define THREADS_HPP

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
 * @brief Read the value from the expanders and update button state.
 */
void *readButtonsFunc(void *args);

/**
 * @brief Loop to turn on or off the leds.
 */
void *updateLedStatus(void *args);


/**
 * @brief Update led array to create rain effect.
 */
void *animationCube1(void *args);
#endif // THREADS_HPP
