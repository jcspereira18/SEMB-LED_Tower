#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "../include/main.hpp"
#include "../include/expander.hpp"
#include "../include/shiftregs.hpp"



void* updateLedValues(void* arg, unsigned char led[ANDARES][COLUNAS][LINHAS], unsigned char newLedState[ANDARES][COLUNAS][LINHAS]){
    for(int andar = 0; andar <= ANDARES; andar++){
        for(int coluna = 0; coluna <= COLUNAS; coluna++){
            for(int linha = 0; linha <= LINHAS; linha++){
                if(led[andar][coluna][linha] != newLedState[andar][coluna][linha]){
                    printf("Updating value = cube[%d][%d][%d] [Andar][Coluna][Linha]",andar, coluna, linha);
                    led[andar][coluna][linha] = newLedState[andar][coluna][linha];
                }
            }
        }
    }
    
    return nullptr;
}

// TODO: função para remover andares imaginários

// Test thread
void* loopFunction(void* arg){
    time_t startTime = time(NULL), currentTime;
    
    while(1){
        currentTime = time(NULL);
        printf("Starting new cycle\n");


        if(difftime(currentTime, startTime) >= 15){ // 15 seconds of testing
            printf("LOOPTIME seconds have been passed. Exiting loop\n");
            startTime = time(NULL);
            break;
        }
    }
    return nullptr;
}

int main() {
    unsigned char led[ANDARES][COLUNAS][LINHAS] = {0};
    unsigned char newLedState[ANDARES][COLUNAS][LINHAS] = {0};
    
    newLedState[0][0][0] = 1;
    newLedState[0][0][0] = 1;
    newLedState[0][0][0] = 1;
    newLedState[0][0][0] = 1;
    newLedState[0][0][0] = 1;
    newLedState[0][0][0] = 1;

    pthread_t initExpanderThread, initShiftRegsThread;
    pthread_create(&initExpanderThread, NULL, initExpander, NULL); 
    pthread_create(&initShiftRegsThread, NULL, initShiftRegs, NULL);
    
    // Initializations
    pthread_join(initExpanderThread, NULL); // expander
    pthread_join(initShiftRegsThread, NULL); // shiftRegs

    // Routines
    // pthread_t 
    
    return 0;
}
