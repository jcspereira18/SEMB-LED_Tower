#include <stdio.h>
#include <string.h>

#include "../../include/components/init_comp.hpp"
#include "../../include/components/tools.hpp"
#include "../../include/modes/snake.hpp"

int getDirectionFromInput(CubeSystem *c, int lastDirection) {
  if (c->Expander1.Button1.state)
    return 5; // -z
  else if (c->Expander1.Button2.state)
    return 2; // +y
  else if (c->Expander1.Button3.state)
    return 1; // -x
  else if (c->Expander1.Button4.state)
    return 3; // -y
  else if (c->Expander2.Button1.state)
    return 4; // +z
  else if (c->Expander2.Button3.state)
    return 0; // +x
  else
    return lastDirection;
}

typedef struct {
    int x, y, z;
} Position;

typedef struct {
    Position positions[216]; // Maximum 6x6x6 snake size
    int front, rear;
} SnakeQueue;

void initSnakeQueue(SnakeQueue *queue) {
    queue->front = 0;
    queue->rear = 0;
}

int isSnakeQueueEmpty(SnakeQueue *queue) {
    return queue->front == queue->rear;
}

void enqueue(SnakeQueue *queue, Position pos) {
    queue->positions[queue->rear] = pos;
    queue->rear++;
    if (queue->rear >= 216) queue->rear = 0; // Wrap around
}

Position dequeue(SnakeQueue *queue) {
    Position pos = queue->positions[queue->front];
    queue->front++;
    if (queue->front >= 216) queue->front = 0; // Wrap around
    return pos;
}

void blinkSnake(LedValues *l, SnakeQueue *snakeQueue, int blinks, int duration_ms) {
    for (int i = 0; i < blinks; i++) {
        // Turn off all snake LEDs
        SnakeQueue tempQueue = *snakeQueue;
        while (!isSnakeQueueEmpty(&tempQueue)) {
            Position pos = dequeue(&tempQueue);
            l->ledValue[pos.x][pos.y][pos.z] = false;
        }
        usleep(duration_ms * 1000); // Blink off duration

        // Turn on all snake LEDs
        tempQueue = *snakeQueue;
        while (!isSnakeQueueEmpty(&tempQueue)) {
            Position pos = dequeue(&tempQueue);
            l->ledValue[pos.x][pos.y][pos.z] = true;
        }
        usleep(duration_ms * 1000); // Blink on duration
    }
}

void resetGame(LedValues *l, SnakeQueue *snakeQueue, int snake[6][6][6], int *head_x, int *head_y, int *head_z, int *length, int *food_x, int *food_y, int *food_z) {
    // Clear the LED array
    clearLedValuesArray(l);

    // Clear the snake array
    memset(snake, 0, sizeof(int) * 6 * 6 * 6);

    // Clear the snake queue
    initSnakeQueue(snakeQueue);

    // Reset snake position and length
    *length = 3; // Snake starts with three segments
    *head_x = 2;
    *head_y = 2;
    *head_z = 2;

    // Initialize the snake body
    Position segments[3] = {
        {2, 2, 0}, // Tail segment
        {2, 2, 1}, // Middle segment
        {2, 2, 2}  // Head segment
    };

    for (int i = 0; i < *length; i++) {
        Position pos = segments[i];
        snake[pos.x][pos.y][pos.z] = i + 1; // Set length values in the array
        l->ledValue[pos.x][pos.y][pos.z] = true; // Turn on the corresponding LED
        enqueue(snakeQueue, pos); // Add to the queue
    }

    // Generate new food position
    do {
        *food_x = rand() % 6;
        *food_y = rand() % 6;
        *food_z = rand() % 6;
    } while (snake[*food_x][*food_y][*food_z] != 0);
    l->ledValue[*food_x][*food_y][*food_z] = true;
}

void blinkFood(LedValues *l, int food_x, int food_y, int food_z, int state) {
    // Turn the food LED on or off
    l->ledValue[food_x][food_y][food_z] = state;
}

void snakeGame(LedValues *l, SystemStates *state, CubeSystem *c) {
    SnakeQueue snakeQueue;
    initSnakeQueue(&snakeQueue);
    int snake[6][6][6] = {0}; // 3D array to track snake positions
    int head_x, head_y, head_z, length;
    int food_x, food_y, food_z;

    // Initialize game state
    resetGame(l, &snakeQueue, snake, &head_x, &head_y, &head_z, &length, &food_x, &food_y, &food_z);

    int direction = 0; // Initial direction: 0 (+x)
    int foodState = 1; // Tracks the blinking state of the food
    while (*state == SNAKE) {
        // Blink the food
        blinkFood(l, food_x, food_y, food_z, foodState);
        foodState = !foodState;

        // Get direction input using your provided function
        direction = getDirectionFromInput(c, direction);

        // Calculate the new head position based on direction
        int new_x = head_x, new_y = head_y, new_z = head_z;
        switch (direction) {
            case 0: new_x++; break; // +x
            case 1: new_x--; break; // -x
            case 2: new_y--; break; // -y
            case 3: new_y++; break; // +y
            case 4: new_z++; break; // +z
            case 5: new_z--; break; // -z
        }

        // Check for collision with walls or snake body
        if (new_x < 0 || new_x >= 6 || new_y < 0 || new_y >= 6 || new_z < 0 || new_z >= 6 || snake[new_x][new_y][new_z] != 0) {
            printf("Game Over! Blinking snake...\n");

            // Blink the snake 3 times
            blinkSnake(l, &snakeQueue, 3, 500);

            printf("Press any key to restart.\n");
            while (1) {
                if (TRUE) { // Placeholder for input detection
                    break;
                }
                usleep(100000); // Polling delay
            }

            // Reset the game state
            resetGame(l, &snakeQueue, snake, &head_x, &head_y, &head_z, &length, &food_x, &food_y, &food_z);
            foodState = 1; // Reset the food blinking state
            continue;
        }

        // Check for food
        if (new_x == food_x && new_y == food_y && new_z == food_z) {
            length++; // Grow the snake

            // Generate new food position
            do {
                food_x = rand() % 6;
                food_y = rand() % 6;
                food_z = rand() % 6;
            } while (snake[food_x][food_y][food_z] != 0);
            l->ledValue[food_x][food_y][food_z] = true;

        } else {
            // Remove the tail if no food was eaten
            Position tail = dequeue(&snakeQueue);
            snake[tail.x][tail.y][tail.z] = 0; // Clear the tail in the array
            l->ledValue[tail.x][tail.y][tail.z] = false; // Turn off the tail LED
        }

        // Update the head
        head_x = new_x;
        head_y = new_y;
        head_z = new_z;
        snake[head_x][head_y][head_z] = length; // Place the new head in the array
        l->ledValue[head_x][head_y][head_z] = true; // Turn on the head LED
        Position head = {head_x, head_y, head_z};
        enqueue(&snakeQueue, head);

        // Delay to control game speed
        sleep(1);
    }
}
