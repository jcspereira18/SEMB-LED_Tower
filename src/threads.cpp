#include <stdio.h>
#include <pthread.h>

#include "../include/threads.hpp"
#include "../include/system.hpp"


void* createCubeSystem(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    initCubeSystem(cubeSystem);
    printf("[INFO] - Successfully created cube system\n");
    return nullptr;
}   

void* globalReset(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    setExpanderVal(&cubeSystem->Expander1, 0xFFFF);
    setExpanderVal(&cubeSystem->Expander2, 0xFFFF);
    setExpanderVal(&cubeSystem->Expander3, 0xFFFF);
    
    setShifterVal(&cubeSystem->Shifter1, 0b0000'0000'0000'0000);
    printf("[INFO] - Successfully reset the system globally\n");

    return nullptr;
}

void* debugPrints(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;
    time_t startTime = time(NULL),
           currentTime,
           totalTime = 15;

    usleep(3 * 1'000'000);
    while(1){
        currentTime = time(NULL);
        printf("\n----------- # ----------\n");
        printf("Readings from second %ld\n", currentTime - startTime);
        printf("Expander1 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander1.valueGPA, cubeSystem->Expander1.valueGPB);
        printf("Expander2 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander2.valueGPA, cubeSystem->Expander2.valueGPB);
        printf("Expander3 ~ valueGPA: %d - valueGPB: %d\n", cubeSystem->Expander3.valueGPA, cubeSystem->Expander3.valueGPB);
        printf("Shifter ~ Data: %d\n", cubeSystem->Shifter1.data);
        printf("----------- # ----------\n\n");
        usleep(1'000'000);
        if(difftime(currentTime, startTime) >= totalTime * 500){
            printf("[INFO] debugPrints ended execution of %ld seconds, returning.\n",totalTime);
            break;
        }
    }
    return nullptr;
}

void* customPosition(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    setExpanderVal(&cubeSystem->Expander1, 0b1111'1111'0000'0000); // U1
    setExpanderVal(&cubeSystem->Expander2, 0b0000'0000'0000'0000); // U2
    setExpanderVal(&cubeSystem->Expander3, 0b0000'0000'0000'0000); // U3 LOGICA INVERSAAAAAAAAAAAAAAAAAA

    setShifterVal(&cubeSystem->Shifter1, 0b0000'0000'0000'0000); // U4_QA HIGH // Handling U4 and U5
    printf("[INFO] - Successfully set the custom position\n");
    return nullptr;
}

void* readButtons(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    while(1){
        printf("[INFO] - Values read from expander1: \n");
        printBinary(readRegisters(&cubeSystem->Expander1));
        usleep(1000000);
    }
}
