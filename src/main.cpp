#include <stdio.h>
#include <pthread.h>

#include "../include/main.hpp"
#include "../include/system.hpp"

void* createCubeSystem(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    initCubeSystem(cubeSystem);
    return nullptr;
}   

void* globalReset(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    setExpanderVal(&cubeSystem->Expander1, 0x0000);
    setExpanderVal(&cubeSystem->Expander2, 0x0000);
    setExpanderVal(&cubeSystem->Expander3, 0x0000);
    
    setShifterVal(&cubeSystem->Shifter1, 0x00);

    return nullptr;
}


void* customPosition(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    setExpanderVal(&cubeSystem->Expander1, 0x00FF);
    setExpanderVal(&cubeSystem->Expander2, 0x0000);
    setExpanderVal(&cubeSystem->Expander3, 0x0000);

    setShifterVal(&cubeSystem->Shifter1, 0x01);
    return nullptr;
}


void* debugPrints(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;
    int timeS = 0;
    while(1){
        printf("\nReadings from second: %d\n",timeS);
        printf("Expander1 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander1.valueGPA, cubeSystem->Expander1.valueGPB);
        printf("Expander2 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander2.valueGPA, cubeSystem->Expander2.valueGPB);
        printf("Expander3 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander3.valueGPA, cubeSystem->Expander3.valueGPB);
        printf("Shifter ~ DataPin: %d\n", cubeSystem->Shifter1.dataPin);
        timeS++;
        usleep(1'000'000);
    }
    return nullptr;
}

void* cycleShifter(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    useconds_t delayBetweenCycles = 10'000; // 10 ms
    time_t startTime = time(NULL),
           currentTime = time(NULL),
           totalTime = 10;

    while(1){
        currentTime = time(NULL);
        clockPulse(&cubeSystem->Shifter1, delayBetweenCycles);

        if(difftime(currentTime, startTime) >= totalTime){
            break;
        }
    usleep(5000);

    }
    return nullptr;
}

int main() {
    CubeSystem cubeSystem;
    pthread_t initCubeSystemThread;
    if(pthread_create(&initCubeSystemThread, NULL, createCubeSystem, (void*)&cubeSystem) != 0) {
        printf("[ERROR] - Cannot create cube system\n");
        return 0;
    }
    pthread_join(initCubeSystemThread, NULL); // wait thread to finish

    // TODO:
    pthread_t globalResetThread;

    pthread_t debugPrintsThread;
    if(pthread_create(&debugPrintsThread, NULL, debugPrints, (void*)&cubeSystem) != 0) {
        printf("[ERROR] - 1\n");
        return 0;
    }

    pthread_t customPositionThread, cycleShifterThread;
    if(pthread_create(&customPositionThread, NULL, customPosition, (void*)&cubeSystem) != 0) {
        printf("[ERROR] - 1\n");
        return 0;
    }
    if(pthread_create(&cycleShifterThread, NULL, cycleShifter, (void*)&cubeSystem) != 0) {
        printf("[ERROR] - 2\n");
        return 0;
    }
    pthread_join(customPositionThread, NULL);
    printf("Completed custom position\n");
    pthread_join(cycleShifterThread, NULL);

    printf("Program runned all tasks. Exiting now.\n");
    return 0;
}
