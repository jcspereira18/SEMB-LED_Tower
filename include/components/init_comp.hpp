#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

// Cube dimensions
// TODO: change these names to english
// Dimensions of the cube: 6x6x6
#define ANDARES 6 // 6 real + removed 5 imaginary
#define COLUNAS 6
#define LINHAS 6

// Expander defines
#define MCP23017_I2C_ADDRESS_1 0x24
#define MCP23017_I2C_ADDRESS_2 0x26
#define MCP23017_I2C_ADDRESS_3 0x27

// TODO: change these names to english
#define IODIRA 0x00 // Registro de direção do Port A
#define IODIRB 0x01 // Registro de direção do Port B
#define GPIOA 0x12  // Registro GPIO Port A
#define GPIOB 0x13  // Registro GPIO Port B
#define OLATA 0x14  // Registro de latch de saída Port A
#define OLATB 0x15  // Registro de latch de saída Port B
#define GPPUA 0x0C  // Pull-up Resistor Configuration for Port A
#define GPPUB 0x0D  // Pull-up Resistor Configuration for Port B

// Shifter defines
#define DATA_PIN 17
#define CLOCK_PIN 4

typedef struct {
  bool ledValues[ANDARES][COLUNAS][LINHAS];
} LedCube;

// TODO: Debouce buttons
typedef struct {
  bool state;
  bool previousState;
  uint16_t pin;
} Button;

// MCP23017
typedef struct {
  int fd;             // File descriptor for I2C
  uint8_t I2CAddress; // I2C address
  uint8_t portGPIOA;  // GPIO register for Port A
  uint8_t portGPIOB;  // GPIO register for Port B
  uint8_t IOdirA;     // I/O direction register for Port A
  uint8_t IOdirB;     // I/O direction register for Port B
  uint8_t inputsDirA; // Input pins for Port A
  uint8_t inputsDirB; // Input pins for Port B
  uint8_t portGPPUA;  // Pull-up resistor configuration for Port A
  uint8_t portGPPUB;  // Pull-up resistor configuration for Port B
  uint8_t valueGPA;   // Pin values for Port A
  uint8_t valueGPB;   // Pin values for Port B
  Button *Button1;
  Button *Button2; // [1] [2]
  Button *Button3; // [3] [4]
  Button *Button4;
} Expander;

// 74HC164
typedef struct {
  int dataPin;
  int clockPin;
  uint16_t data;
} Shifter;

typedef struct {
  LedCube Cube;
  Expander Expander1; // U1
  Expander Expander2; // U2
  Expander Expander3; // U3
  Shifter Shifter1;   // U4 & U5
  Button Button11;
  Button Button12;
  Button Button13;
  Button Button14;
  Button Button15;
  Button Button16;
  Button Button21;
  Button Button22;
  Button Button23;
  Button Button24;
  Button Button25;
  Button Button26;
} CubeSystem;

/**
 * @brief Initializes the MCP23017 expander with the given configuration.
 *
 * This function sets up an MCP23017 I/O expander on the specified I2C address
 * and configures its ports, directions, and initial values. It ensures proper
 * initialization and validates parameters and hardware interaction.
 *
 * @param e Pointer to the Expander structure to initialize. Must not be NULL.
 * @param I2CAddress The I2C address of the expander.
 * @param IOdirA The register address to configure the I/O direction for Port A.
 * @param IOdirB The register address to configure the I/O direction for Port B.
 * @param portGPIOA The register address for Port A GPIO.
 * @param portGPIOB The register address for Port B GPIO.
 * @param portGPPUA The register address for Port A pull-up configuration.
 * @param portGPPUB The register address for Port B pull-up configuration.
 * @param inputsDirA The direction settings for Port A (input or output bits).
 * @param inputsDirB The direction settings for Port B (input or output bits).
 * @param initialValueGPA The initial output value for Port A GPIO.
 * @param initialValueGPB The initial output value for Port B GPIO.
 *
 * @throws std::runtime_error Exits the program if a parameter is invalid
 *         or if communication with the expander fails.
 *
 * @note This function uses the WiringPi library for I2C communication.
 * @warning Exits the program if the `Expander` pointer is NULL or if
 *          hardware setup fails.
 */
void initExpander(Expander *e, uint8_t I2CAddress, uint8_t IOdirA,
                  uint8_t IOdirB, uint8_t portGPIOA, uint8_t portGPIOB,
                  uint8_t portGPPUA, uint8_t portGPPUB, uint8_t inputsDirA,
                  uint8_t inputsDirB, uint8_t initialValueGPA,
                  uint8_t initialValueGPB);
/**
 * @brief Initializes a shift register with the specified pins and initial data.
 *
 * This function sets up a shift register by configuring its data and clock pins
 * as outputs and initializing them to a LOW state. It also validates the input
 * parameters and ensures proper GPIO setup using the WiringPi library.
 *
 * @param s Pointer to the Shifter structure to initialize. Must not be NULL.
 * @param dataPin The GPIO pin number used as the data pin for the shift
 * register.
 * @param clockPin The GPIO pin number used as the clock pin for the shift
 * register.
 * @param initialData The initial value to load into the shift register (not
 * used in this implementation but reserved for future use).
 *
 * @throws std::runtime_error The program exits if:
 * - The `Shifter` pointer is NULL.
 * - GPIO initialization fails using WiringPi.
 *
 * @note This function uses the WiringPi library for GPIO configuration. Ensure
 *       WiringPi is installed and properly configured before calling this
 * function.
 *
 * @warning Exits the program if invalid parameters are provided or if GPIO
 *          setup fails.
 */
void initShifter(Shifter *s, uint16_t dataPin, uint16_t clockPin,
                 uint16_t initialData);

/**
 * @brief Initializes the Cube System, including LEDs, buttons, expanders, and
 * shifters.
 *
 * This function sets up the CubeSystem structure by:
 * - Initializing the Cube LEDs to a zero state.
 * - Configuring buttons and associating them with the appropriate expanders.
 * - Initializing MCP23017 I/O expanders.
 * - Setting up a shift register for additional hardware control.
 *
 * @param s Pointer to the CubeSystem structure to initialize. Must not be NULL.
 *
 * @throws std::runtime_error The program exits if:
 * - The `CubeSystem` pointer is NULL.
 * - Any initialization step (e.g., for buttons, expanders, or shifters) fails.
 *
 * @note
 * - The initialization relies on WiringPi for GPIO and I2C interactions.
 *   TODO:
 * - There is a known bug in the button association logic, where buttons are not
 *   correctly linked to their corresponding expanders. This needs to be
 * addressed.
 *
 * @warning Exits the program if invalid parameters are provided or any hardware
 *          setup fails.
 */
void initCubeSystem(CubeSystem *s);

/**
 * @brief Initializes a Button structure with the specified GPIO pin and default
 * state.
 *
 * This function sets up a Button structure by assigning the given GPIO pin and
 * initializing the button state to LOW. It validates the input parameters to
 * ensure the structure pointer is not NULL.
 *
 * @param b Pointer to the Button structure to initialize. Must not be NULL.
 * @param pin The GPIO pin number assigned to the button.
 *
 * @throws std::runtime_error The program exits if the `Button` pointer is NULL.
 *
 * @note This function does not configure the GPIO hardware itself. Ensure the
 *       associated GPIO pin is properly configured elsewhere in the program.
 *
 * @warning Exits the program if the `Button` pointer is invalid.
 */
void initButton(Button *b, uint16_t pin);

#endif // COMPONENTS_HPP
