#include <stdio.h>
#include <time.h>

#include "../../include/snake/init_snake.hpp"

// TODO: Remove this
#define DIMENSION 6

typedef struct {
  int dx, dy, dz;
} Direction;

typedef struct Node {
  int x, y, z;
} Node;

typedef struct {
    Node body[DIMENSION][DIMENSION][DIMENSION];
    int length;
} Snake;

typedef struct {
  int x, y, z;
} Food;

Direction directions[] = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                          {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};

void initializeGame(Snake *s) {
  if (s == NULL) {
    printf("[ERROR] - Invalid parameter in initializeGame\n");
    exit(EXIT_FAILURE);
  }

// Para medir o tempo -> Utilizar o temporizador interno
// Não esquecer -> correr apenas essa tarefa ao medir o tempo de execução, e
// certificar que nenhuma outra tarefa está a ser executada
