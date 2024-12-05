#include <stdio.h>
#include <pthread.h>

#include "../include/main.hpp"
#include "../include/system.hpp"

void* createCubeSystem(void* args){
    CubeSystem* cubeSystem = (CubeSystem*)args;

    initCubeSystem(cubeSystem);
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

    printf("Program runned all tasks. Exiting now.\n");
    return 0;
}

// TODO: Descobrir qual é a associação entre A e B -> andares ou colunas?
//void* showCubeLedStatus(void* arg){
//    Cube *ledCube = (Cube*)arg;
//    while(1){
//        for(int andar = 0; andar < ANDARES; andar++){
//            incrementShiftVal(&ledCube->shifterVal); // start shifter with 0 and cycle to andares
//            
//            for(int coluna = 0; coluna < COLUNAS; coluna++){
//                incrementExpanderVal(&ledCube->expanderAVal); // assuming A as columns
//
//                for(int linha = 0; linha < LINHAS; linha++){
//                    incrementExpanderVal(&ledCube->expanderBVal); // assuming B as lines
//
//                    if(ledCube->ledValues[andar][coluna][linha] == 1){
//                        printf("Turning on ledCube[%d][%d][%d]\n", andar, coluna, linha);
//                        a
//
//                        
//                    }
//                }
//            }
//        }
//        // maybe we will need to reset the expanders + shifter values to 0 in the end...
//        usleep(50'000); // 50 ms sleep  TODO: Tune this value! 
//    }
//}
//        unsigned char led[ANDARES][COLUNAS][LINHAS] = *()
//        unsigned char newLedState[ANDARES][COLUNAS][LINHAS],
//        unsigned char* expanderAValue,
//        unsigned char* expanderBValue,
//        unsigned char* shifterValue
//
//    for(int andar = 0; andar < ANDARES; andar++){
//        // start with shifter 0 and cycle to andares
//        incrementShiftVal(shifterValue);
//
//        for(int coluna = 0; coluna < COLUNAS; coluna++){
//            incrementExpanderVal(expanderAValue); // Assumindo A como colunas
//
//            for(int linha = 0; linha < LINHAS; linha++){
//                incrementExpanderVal(expanderBValue); // Assumindo B como andares
//
//                if(led[andar][coluna][linha] != newLedState[andar][coluna][linha]){
//                    printf("Updating value = cube[%d][%d][%d][Andar][Coluna][Linha]"
//                            ,andar, coluna, linha);
//                    led[andar][coluna][linha] = newLedState[andar][coluna][linha];
//                }
//            }
//        }
//    }
//    
//    return nullptr;
//}

// TODO: função para remover andares imaginários

// Test thread
//void* loopFunction(void* arg){
//    Cube led_cube = {0}; // Initialize all members with 0
//
//    time_t startTime = time(NULL), currentTime;
//    
//    while(1){
//        currentTime = time(NULL);
//        printf("Starting new cycle\n");
//
//
//        if(difftime(currentTime, startTime) >= 15){ // 15 seconds of testing
//            printf("LOOPTIME seconds have been passed. Exiting loop\n");
//            startTime = time(NULL);
//            break;
//        }
//    }
//    return nullptr;
//}


