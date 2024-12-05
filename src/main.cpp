#include <stdio.h>
#include <pthread.h>

#include "../include/main.hpp"
#include "../include/system.hpp"

void* createCubeSystem(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    initCubeSystem(cubeSystem);
    printf("[INFO - Successfully created cube system\n]");
    return nullptr;
}   

void* globalReset(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    setExpanderVal(&cubeSystem->Expander1, 0x0000);
    setExpanderVal(&cubeSystem->Expander2, 0x0000);
    setExpanderVal(&cubeSystem->Expander3, 0x0000);
    
    setShifterVal(&cubeSystem->Shifter1, 0x00);
    printf("[INFO - Successfully reset the system globally\n]");

    return nullptr;
}


// Trying to turn D1 ON
void* customPosition(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    setExpanderVal(&cubeSystem->Expander1, 0b0000'0001'0000'0000); // U1_GPB0 HIGH
    setExpanderVal(&cubeSystem->Expander2, 0b0000'0000'0000'0000); // U2
    setExpanderVal(&cubeSystem->Expander3, 0b0000'0000'0000'0000); // U3

    setShifterVal(&cubeSystem->Shifter1, 0x01); // U4_QA HIGH // Handling U4 and U5
    printf("[INFO - Successfully set the custom position\n]");
    return nullptr;
}


void* debugPrints(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;
    time_t startTime = time(NULL),
           currentTime,
           totalTime = 15;

    while(1){
        currentTime = time(NULL);
        printf("\nReadings from second: %ld\n", currentTime - startTime);
        printf("Expander1 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander1.valueGPA, cubeSystem->Expander1.valueGPB);
        printf("Expander2 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander2.valueGPA, cubeSystem->Expander2.valueGPB);
        printf("Expander3 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander3.valueGPA, cubeSystem->Expander3.valueGPB);
        printf("Shifter ~ DataPin: %d\n", cubeSystem->Shifter1.dataPin);
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
    int cycleNum = 0;

    while(1){
        currentTime = time(NULL);
        clockPulse(&cubeSystem->Shifter1, delayBetweenCycles);
        printf("\n"); // TODO: Test printing with \r
        printf("\r[INFO][Cycle %d][time %ld]\n", cycleNum, currentTime - startTime);

        if(difftime(currentTime, startTime) >= totalTime){
            printf("[INFO] Stopping cycleShifter\n");
            break;
        }
        cycleNum++;
        usleep(5000);
    }

    printf("[INFO] returning from cycleShifter\n");
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
