#ifndef THREADS_HPP
#define THREADS_HPP

#include <unistd.h>

void *createCubeSystem(void *args);
void *globalReset(void *args);
void *customPosition(void *args);
void *readButtonsFunc(void *args);
#endif // THREADS_HPP
