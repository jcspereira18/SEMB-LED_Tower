#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "../include/main.hpp"
#include "../include/expander.hpp"
#include "../include/shiftregs.hpp"

// Test thread
void* loopFunction(void* arg){
    time_t startTime = time(NULL), currentTime;
    
    while(1){
        currentTime = time(NULL);

        printf("Starting new cycle\n");
        if(difftime(currentTime, startTime) >= 15){ // 15 seconds of testing
            printf("LOOPTIME seconds have been passed. Exiting loop\n");
            break;
        }
    }

    return nullptr;
}

int main() {
    pthread_t initExpanderThread, initShiftRegsThread;
    pthread_create(&initExpanderThread, NULL, initExpander, NULL); 
    pthread_create(&initShiftRegsThread, NULL, initShiftRegs, NULL);

    pthread_join(initExpanderThread, NULL); // expander
    pthread_join(initShiftRegsThread, NULL); // shiftRegs

    return 0;
}
