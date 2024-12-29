#include "../../include/modes/animations.hpp"
#include "../../include/components/init_comp.hpp"
#include "../../include/components/tools.hpp"

void rainAnimation(LedValues *l, SystemStates *state) {

  clearLedValuesArray(l);

  // Loop through all layers
  for (int t = 0; *state == RAIN; t++) {
    // Randomly generate raindrops on the top layer (z = 5)
    for (int i = 0; i < 4;
         i++) { // `dropRate` controls how many drops are added per frame
      int x = rand() % 6;
      int y = rand() % 6;
      l->ledValue[x][y][5] = true; // Add a drop at the top layer
    }

    // Move all drops down one layer
    for (int z = 0; z < 5; z++) { // Iterate from bottom to second-to-last layer
      for (int x = 0; x < 6; x++) {
        for (int y = 0; y < 6; y++) {
          if (l->ledValue[x][y][z + 1]) {
            l->ledValue[x][y][z] = true;      // Move the drop down
            l->ledValue[x][y][z + 1] = false; // Turn off the old position
          }
        }
      }
    }

    for (int x = 0; x < 6; x++) {
      for (int y = 0; y < 6; y++) {
        if (l->ledValue[x][y][0]) {
          // Randomly decide whether to remove the drop in the bottom layer
          if ((rand() % 100) < (50)) {
            l->ledValue[x][y][0] = false; // Remove the drop
          }
        }
      }
    }
    usleep(100000);
  }
  clearLedValuesArray(l);

  return;
}

void fireworksAnimation(LedValues *l, SystemStates *state) {
  clearLedValuesArray(l);

  while (*state == IDLE) {
    // Generate a random starting point for the firework burst
    int start_x = rand() % 6;
    int start_y = rand() % 6;
    int start_z = rand() % 6;

    usleep(500000); // 500 ms

    // Pre-burst animation: LED travels vertically
    for (int z = 0; z <= start_z; z++) {
      clearLedValuesArray(l); // Clear the cube for the current frame
      l->ledValue[start_x][start_y][z] =
          true;       // Light up the LED at the current z position
      usleep(100000); // Delay for the traveling animation
    }

    // Firework burst animation
    for (int radius = 0; radius < 6 & *state == IDLE; radius++) {
      clearLedValuesArray(l); // Clear the cube for the current frame

      // Iterate through all points in the cube
      for (int x = 0; x < 6; x++) {
        for (int y = 0; y < 6; y++) {
          for (int z = 0; z < 6; z++) {
            // Calculate the distance from the starting point
            int distance =
                abs(x - start_x) + abs(y - start_y) + abs(z - start_z);

            // If the distance matches the current radius, light up the LED
            if (distance == radius) {
              l->ledValue[x][y][z] = true;
            }
          }
        }
      }

      // Delay between frames to simulate the burst
      usleep(150000); // 150 ms
    }

    // Sparkle effect after the burst
    for (int i = 0; i < 5 && *state == IDLE; i++) { // Number of sparkle frames
      clearLedValuesArray(l); // Clear the cube for the sparkle frame

      // Randomly light up a few LEDs around the starting point
      for (int j = 0; j < 10; j++) { // Number of sparkles per frame
        int sparkle_x = start_x + (rand() % 3 - 1); // Random offset -1, 0, 1
        int sparkle_y = start_y + (rand() % 3 - 1);
        int sparkle_z = start_z + (rand() % 3 - 1);

        // Ensure the sparkles are within the cube bounds
        if (sparkle_x >= 0 && sparkle_x < 6 && sparkle_y >= 0 &&
            sparkle_y < 6 && sparkle_z >= 0 && sparkle_z < 6) {
          l->ledValue[sparkle_x][sparkle_y][sparkle_z] = true;
        }
      }

      // Delay for the sparkle frame
      usleep(100000); // 100 ms
    }

    // Add a brief delay before the next firework
    clearLedValuesArray(l);
  }

  clearLedValuesArray(l); // Clear the cube when exiting the animation
}
