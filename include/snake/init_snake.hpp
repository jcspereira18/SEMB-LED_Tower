#ifndef INIT_SNAKE_HPP
#define INIT_SNAKE_HPP

#include <stdlib.h>
#include <unistd.h>
#include "../components/init_comp.hpp"

// TODO: Remove this
#define DIMENSION 6

typedef struct {
  int dx, dy, dz;
} Direction;

typedef struct Node {
  int x, y, z;
} Node;

typedef struct {
  Node body[DIMENSION * DIMENSION * DIMENSION];
  int length;
  bool gameOver;
} Snake;

typedef struct {
  int x, y, z;
} Food;

void initializeGame(LedValues *c, Snake *s, Food *f);
void moveSnake(LedValues *c, Snake *s, Food *f, int direction);

#endif // INIT_SNAKE_HPP
