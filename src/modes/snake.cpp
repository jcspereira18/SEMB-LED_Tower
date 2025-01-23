#include <stdio.h>
#include <string.h>

#include "../../include/components/init_comp.hpp"
#include "../../include/components/tools.hpp"
#include "../../include/modes/snake.hpp"

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

int isSnakeQueueEmpty(SnakeQueue *queue) { return queue->front == queue->rear; }

void enqueue(SnakeQueue *queue, Position pos) {
  queue->positions[queue->rear] = pos;
  queue->rear++;
  if (queue->rear >= 216)
    queue->rear = 0; // Wrap around
}

Position dequeue(SnakeQueue *queue) {
  Position pos = queue->positions[queue->front];
  queue->front++;
  if (queue->front >= 216)
    queue->front = 0; // Wrap around
  return pos;
}

void blinkSnake(LedValues *l, SnakeQueue *snakeQueue, int blinks,
                int duration_ms) {
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

void resetGame(LedValues *l, SnakeQueue *snakeQueue, int snake[6][6][6],
               int *head_x, int *head_y, int *head_z, int *length, int *food_x,
               int *food_y, int *food_z) {
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
    snake[pos.x][pos.y][pos.z] = i + 1;      // Set length values in the array
    l->ledValue[pos.x][pos.y][pos.z] = true; // Turn on the corresponding LED
    enqueue(snakeQueue, pos);                // Add to the queue
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
  static SnakeQueue snakeQueue;
  static int snake[6][6][6] = {0}; // 3D array to track snake positions
  static int head_x, head_y, head_z, length;
  static int food_x, food_y, food_z;
  static int direction = 0; // Initial direction: 0 (+x)
  static int foodState = 1; // Tracks the blinking state of the food
  static bool initialized = false;

  // Initialize game state once
  if (!initialized) {
    initSnakeQueue(&snakeQueue);
    resetGame(l, &snakeQueue, snake, &head_x, &head_y, &head_z, &length,
              &food_x, &food_y, &food_z);
    initialized = true;
  }

  // Blink the food for the current frame
  blinkFood(l, food_x, food_y, food_z, foodState);
  foodState = !foodState;

  // Get direction input
  direction = c->SnakeDirection;

  // Calculate the new head position based on direction
  int new_x = head_x, new_y = head_y, new_z = head_z;
  switch (direction) {
  case 0:
    new_x++;
    break; // +x
  case 1:
    new_x--;
    break; // -x
  case 2:
    new_y--;
    break; // -y
  case 3:
    new_y++;
    break; // +y
  case 4:
    new_z++;
    break; // +z
  case 5:
    new_z--;
    break; // -z
  }

  // Check for collision with walls or snake body
  if (new_x < 0 || new_x >= 6 || new_y < 0 || new_y >= 6 || new_z < 0 ||
      new_z >= 6 || snake[new_x][new_y][new_z] != 0) {

    // Blink the snake 3 times (non-blocking)
    static int blinkCounter = 0;
    if (blinkCounter < 6) {               // Blink 3 times (on + off)
      blinkSnake(l, &snakeQueue, 1, 100); // One blink per frame
      blinkCounter++;
      return; // Exit for this frame
    }

    // Reset the game state
    blinkCounter = 0;
    resetGame(l, &snakeQueue, snake, &head_x, &head_y, &head_z, &length,
              &food_x, &food_y, &food_z);
    foodState = 1; // Reset the food blinking state
    return;        // Exit for this frame
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
    snake[tail.x][tail.y][tail.z] = 0;           // Clear the tail in the array
    l->ledValue[tail.x][tail.y][tail.z] = false; // Turn off the tail LED
  }

  // Update the head
  head_x = new_x;
  head_y = new_y;
  head_z = new_z;
  snake[head_x][head_y][head_z] = length;     // Place the new head in the array
  l->ledValue[head_x][head_y][head_z] = true; // Turn on the head LED
  Position head = {head_x, head_y, head_z};
  enqueue(&snakeQueue, head);

  // Exit without delay for frame-by-frame operation
  return;
}
