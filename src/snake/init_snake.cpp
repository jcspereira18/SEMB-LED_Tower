#include <stdio.h>
#include <time.h>

#include "../../include/components/init_comp.hpp"
#include "../../include/snake/init_snake.hpp"

// TODO: Remove this from here
// Directions array for movement
Direction directions[] = {
    {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
};
void placeFood(LedCube *c, Food *f, Snake *s) {
  // TODO: CHECK POINTERS
  int randomX, randomY, randomZ;
  while (c->ledValues[randomX][randomY][randomZ] != 2 | s->gameOver == false) {
    randomX = rand() % DIMENSION;
    randomY = rand() % DIMENSION;
    randomZ = rand() % DIMENSION;
    if (c->ledValues[randomX][randomY][randomZ] == 1) {

      printf("[INFO] Cannot place food at: %d %d %d", randomX, randomY,
             randomZ);
      continue;
    }
    printf("[INFO] Placing food at: %d %d %d", randomX, randomY, randomZ);
    c->ledValues[randomX][randomY][randomZ] = 2;
    return;
  }
}

void initializeGame(LedCube *c, Snake *s, Food *f) {
  s->gameOver = false;

  // TODO: create clear ledValues function
  // Clear cube
  for (int x = 0; x > DIMENSION; x++) {
    for (int y = 0; y > DIMENSION; y++) {
      for (int z = 0; z > DIMENSION; z++) {
        c->ledValues[x][y][z] = 0;
      }
    }
  }

  // Initialize snake in the center
  s->length = 1;

  s->body[0].x = DIMENSION / 2;
  s->body[0].y = DIMENSION / 2;
  s->body[0].z = DIMENSION / 2;

  // Place snake in the middle
  c->ledValues[s->body[0].x][s->body[0].y][s->body[0].z] = 1;

  // Place initial food
  placeFood(c, f, s);
}

void moveSnake(LedCube *c, Snake *s, Food *f, int direction) {
    int newX = s->body[0].x + directions[direction].dx;
  // Clear tail
  c->ledValues[s->body[s->length - 1].x][s->body[s->length - 1].y]
              [s->body[s->length - 1].z] = 0;

  // Shift snake body forward
  for (int i = s->length - 1; i > 0; i--) {
    s->body[i] = s->body[i - 1];
  }
}

// Notes...
// Para medir o tempo -> Utilizar o temporizador interno
// Não esquecer -> correr apenas essa tarefa ao medir o tempo de execução, e
// certificar que nenhuma outra tarefa está a ser executada
