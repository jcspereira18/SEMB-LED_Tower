#include <stdio.h>
#include <time.h>

#include "../../include/snake/init_snake.hpp"

#define DIMENSION 6

typedef struct {
  int dx, dy, dz;
} Direction;

typedef struct Node {
  int x, y, z;
  struct Node *next;
} Node;

typedef struct {
  Node *head;
  Node *tail;
  int length;
} Snake;

typedef struct {
  int x, y, z;
} Food;

Direction directions[] = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                          {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};
Snake snake;
Food food;

void initializeGame(Snake *s) {
  if (s == NULL) {
    printf("[ERROR] - Invalid parameter in initializeGame\n");
    exit(EXIT_FAILURE);
  }

  // TODO: Clear cube

  // Initialize snake in the center of the cube
  s->head->x = DIMENSION / 2;
  s->head->y = DIMENSION / 2;
  s->head->z = DIMENSION / 2;

  snake.head->next = NULL;
  snake.tail = snake.head;
  snake.length = 1;

  // WARN: Not marking the snake on the cube
  // cube[snake.head->x][snake.head->y][snake.head->z] = 1;

  // Place initial food
  srand(time(NULL));
  food.x = rand() % DIMENSION;
  food.y = rand() % DIMENSION;
  food.z = rand() % DIMENSION;

  // WARN: Not makring the food on the cube
  // cube[food.x][food.y][food.z] = 10; // 10 is food
}

bool moveSnake(int direction) {
  int new_x = snake.head->x + directions[direction].dx;
  int new_y = snake.head->x + directions[direction].dx;
  int new_z = snake.head->x + directions[direction].dx;

  // check for collision with itself
  if (new_x < 0 || new_x >= DIMENSION || new_y < 0 || new_y >= DIMENSION ||
      new_z < 0 || new_z >= DIMENSION || cube)
}

// Para medir o tempo -> Utilizar o temporizador interno
// Não esquecer -> correr apenas essa tarefa ao medir o tempo de execução, e
// certificar que nenhuma outra tarefa está a ser executada
