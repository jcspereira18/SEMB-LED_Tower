#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 

//#include "../include/main.hpp"
#include "../include/system.hpp"
#include "../include/threads.hpp"

// Trying to turn D1 ON
void* customPosition(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    setExpanderVal(&cubeSystem->Expander1, 0b0000'0000'0000'0000); // U1
    setExpanderVal(&cubeSystem->Expander2, 0b1111'1111'1111'1111); // U2
    setExpanderVal(&cubeSystem->Expander3, 0b1111'1111'1111'1111); // U3

    setShifterVal(&cubeSystem->Shifter1, 0b0000'0000'0000'0001); // U4_QA HIGH // Handling U4 and U5
    printf("[INFO] - Successfully set the custom position\n");
    return nullptr;
}

int main() {
    CubeSystem cubeSystem;

    pthread_t initCubeSystemThread; // Initializing Cube System
    if(pthread_create(&initCubeSystemThread, NULL, createCubeSystem, (void*)&cubeSystem) != 0) {
        printf("[ERROR] - Cannot create cube system\n");
        exit(EXIT_FAILURE);
    }
    pthread_join(initCubeSystemThread, NULL); // wait thread to finish
    printf("[INFO] - Cube has been initialized\n");

    pthread_t globalResetThread; // Reseting global State of the Cube
    if(pthread_create(&globalResetThread, NULL, globalReset,  (void*)&cubeSystem) != 0){
        printf("[ERROR] - Cannot create globalResetThread\n");
        exit(EXIT_FAILURE);
    }
    pthread_join(globalResetThread, NULL);

    // Following 2 threads are runnning cuncurently

    pthread_t debugPrintsThread; // Print Cube status
    if(pthread_create(&debugPrintsThread, NULL, debugPrints, (void*)&cubeSystem) != 0) {
        printf("[ERROR] - Cannot create debugPrintsThread\n");
        exit(EXIT_FAILURE);
    } // Dont want to wait function to end ...


    pthread_t customPositionThread; // Set custom position
    if(pthread_create(&customPositionThread, NULL, customPosition, (void*)&cubeSystem) != 0) {
        printf("[ERROR] - Cannot create customPositionThread\n");
        exit(EXIT_FAILURE);
    }

    pthread_join(customPositionThread, NULL);
    printf("[INFO] - Completed custom position\n");
    pthread_join(debugPrintsThread, NULL);

    printf("Program runned all tasks. Exiting now.\n");
    return 0;
}
