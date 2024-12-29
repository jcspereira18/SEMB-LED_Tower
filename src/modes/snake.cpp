#include <stdio.h>
#include <string.h>

#include "../../include/components/init_comp.hpp"
#include "../../include/components/tools.hpp"
#include "../../include/modes/snake.hpp"

int getDirectionFromInput(CubeSystem *c, int lastDirection) {
  if (c->Expander1.Button1.state)
    return 5; // -z
  else if (c->Expander1.Button2.state)
    return 3; // +y
  else if (c->Expander1.Button3.state)
    return 1; // -x
  else if (c->Expander1.Button4.state)
    return 2; // -y
  else if (c->Expander2.Button1.state)
    return 4; // +z
  else if (c->Expander2.Button3.state)
    return 0; // +x
  else
    return lastDirection;
}

void snakeGame(LedValues *l, SystemStates *state, CubeSystem *c) {
    // Initialize the snake's initial position and size
    int snake[6][6][6] = {0}; // 3D array to track snake positions
    int head_x = 2, head_y = 2, head_z = 2;
    int tail_x = 2, tail_y = 2, tail_z = 2;
    int length = 1; // Initial length of the snake
    int food_x, food_y, food_z;

    // Place the initial snake head
    l->ledValue[head_x][head_y][head_z] = true;
    snake[head_x][head_y][head_z] = length;

    // Generate initial food position
    do {
        food_x = rand() % 6;
        food_y = rand() % 6;
        food_z = rand() % 6;
    } while (snake[food_x][food_y][food_z] != 0); // Ensure food isn't placed on the snake
    l->ledValue[food_x][food_y][food_z] = true;

    // Game loop
    int direction = 0; // Initial direction: 0 (+x), 1 (-x), 2 (+y), 3 (-y), 4 (+z), 5 (-z)
    while (*state == SNAKE) {
        // Get direction input (stubbed here)
        direction = getDirectionFromInput(c, direction);

        // Calculate the new head position based on direction
        int new_x = head_x, new_y = head_y, new_z = head_z;
        switch (direction) {
            case 0: new_x++; break; // +x
            case 1: new_x--; break; // -x
            case 2: new_y++; break; // +y
            case 3: new_y--; break; // -y
            case 4: new_z++; break; // +z
            case 5: new_z--; break; // -z
        }

        // Check for collision with walls or snake body
        if (new_x < 0 || new_x >= 6 || new_y < 0 || new_y >= 6 || new_z < 0 || new_z >= 6 || snake[new_x][new_y][new_z] != 0) {
            // Game over - Blink only the snake
            for (int i = 0; i < 3; i++) { // Blink 3 times
                // Turn off snake
                for (int x = 0; x < 6; x++) {
                    for (int y = 0; y < 6; y++) {
                        for (int z = 0; z < 6; z++) {
                            if (snake[x][y][z] > 0) {
                                l->ledValue[x][y][z] = false;
                            }
                        }
                    }
                }
                usleep(600000); // 500ms delay

                // Turn on snake
                for (int x = 0; x < 6; x++) {
                    for (int y = 0; y < 6; y++) {
                        for (int z = 0; z < 6; z++) {
                            if (snake[x][y][z] > 0) {
                                l->ledValue[x][y][z] = true;
                            }
                        }
                    }
                }
                usleep(500000); // 500ms delay
            }

            // Wait for keypress to restart
            printf("Game Over! Press any key to restart.\n");
            while (1) {
                if (TRUE) { // Placeholder for input detection
                    break;
                }
                usleep(100000); // Polling delay
            }

            // Restart the game by reinitializing state
            clearLedValuesArray(l);
            memset(snake, 0, sizeof(snake));
            head_x = 2, head_y = 2, head_z = 2;
            tail_x = 2, tail_y = 2, tail_z = 2;
            length = 1;
            l->ledValue[head_x][head_y][head_z] = true;
            snake[head_x][head_y][head_z] = length;

            // Generate new food position
            do {
                food_x = rand() % 6;
                food_y = rand() % 6;
                food_z = rand() % 6;
            } while (snake[food_x][food_y][food_z] != 0);
            l->ledValue[food_x][food_y][food_z] = true;

            direction = 0; // Reset direction
            continue;
        }

        // Check for food
        if (new_x == food_x && new_y == food_y && new_z == food_z) {
            length++; // Grow the snake

            // Generate new food position after eating
            do {
                food_x = rand() % 6;
                food_y = rand() % 6;
                food_z = rand() % 6;
            } while (snake[food_x][food_y][food_z] != 0); // Ensure food isn't placed on the snake
            l->ledValue[food_x][food_y][food_z] = true;

        } else {
            // Remove the tail if no food was eaten
            snake[tail_x][tail_y][tail_z] = 0;
            l->ledValue[tail_x][tail_y][tail_z] = false;

            // Find the new tail position: The tail will be the part that had the smallest number
            int new_tail_val = length - 1;
            for (int x = 0; x < 6; x++) {
                for (int y = 0; y < 6; y++) {
                    for (int z = 0; z < 6; z++) {
                        if (snake[x][y][z] == new_tail_val) {
                            tail_x = x;
                            tail_y = y;
                            tail_z = z;
                            break;
                        }
                    }
                }
            }
        }

        // Move the body: Move each part of the snake's body
        for (int length_index = length; length_index > 1; length_index--) {
            for (int x = 0; x < 6; x++) {
                for (int y = 0; y < 6; y++) {
                    for (int z = 0; z < 6; z++) {
                        if (snake[x][y][z] == length_index - 1) {
                            // Move the body part to the next position
                            snake[x][y][z] = length_index;
                            break;
                        }
                    }
                }
            }
        }

        // Update the head: Place the new head
        head_x = new_x;
        head_y = new_y;
        head_z = new_z;
        snake[head_x][head_y][head_z] = length; // Set the new head
        l->ledValue[head_x][head_y][head_z] = true; // Turn on the new head

        // Delay to control game speed
        usleep(1000000); // 300 ms
    }

    clearLedValuesArray(l);
}
