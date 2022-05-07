#include "config.h"
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>

#define MESSAGES_TO_PRINT 5

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

int main(int argc, char* argv[]){
    int totalProcess = MIN_PROCESS;
    int slaveProcessLiveTime = SLAVE_PROCESS_LIVE_TIME;
    union semun arg;
    key_t testFileSemKey;
    int semid;
    pid_t slaveProcessID;
    time_t * seconds = (time_t*) malloc(sizeof(time_t));

    printf("Master Program Started...\n");

    //Validate Input
    if(argc < 3){
        printf("WARNING: Invalid Number of Args : Slave Process Time: %i.  %i Processes Created.\n", SLAVE_PROCESS_LIVE_TIME, MIN_PROCESS);
    }
    
    else{
        if(0 > atoi(argv[SLAVE_PROCESS_LIVE_TIME])){
            printf("WARNING: Slave Process Time Invalid: default  %i time used.\n", SLAVE_PROCESS_LIVE_TIME);
        }

        else{
            slaveProcessLiveTime = atoi(argv[SLAVE_PROCESS_LIVE_TIME]);
        }

        // Validate user entry and set totalProcess
        if(atoi(argv[PROGRAM_TOTAL_PROCESS]) > MAX_PROCESS){
            printf("WARNING: Invalid Number of Processes: %i.  %i Processes Created.\n", atoi(argv[PROGRAM_TOTAL_PROCESS]), MAX_PROCESS);
            totalProcess = MAX_PROCESS; 
        }

        else if(atoi(argv[PROGRAM_TOTAL_PROCESS]) < MIN_PROCESS){
            printf("WARNING: Invalid Number of Processes: %i.  %i Processes Created.\n", atoi(argv[PROGRAM_TOTAL_PROCESS]), MIN_PROCESS);
            totalProcess = MIN_PROCESS;
        }

        else{
            totalProcess = atoi(argv[PROGRAM_TOTAL_PROCESS]);
        }
    }


    //Open Master Log File
    FILE *testFilePtr;
    testFilePtr = fopen("cstest", "w");
    printf("Test File Successly Created!\n");

    //Get file key
    if(( testFileSemKey = ftok("cstest", 'E')) == -1){
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    //creat semaphore
    if((semid = semget(testFileSemKey, 10, SEM_PERMISSION | IPC_CREAT)) == -1){
        perror("semget");
        exit(EXIT_FAILURE);
    }

    arg.val = 1;
    //initialize semaphore
    if(semctl(semid, 0, SETVAL, arg) == -1){
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    else{
        fprintf(testFilePtr, "Semaphore Initialized SUCCESS!!!\n");
    }
    
    // DO IT!
    int pid = 0; //delete this ling
    int currentProcess = 0;
    pid_t masterPid = getpid();
    for(currentProcess = 0; currentProcess < totalProcess; currentProcess++){
        slaveProcessID = fork();
        printf("Process %i started with Process ID %i\n", currentProcess, slaveProcessID);
        for(int i = 0; i < MESSAGES_TO_PRINT; i++){
            sleep(rand()%2 + 1);
            *seconds = time(NULL);
            fprintf(testFilePtr, "%lld Queue %d File Modified by Process Number %d\n", (long long) *seconds, masterPid, slaveProcessID);
            printf("%lld Queue %d File Modified by Process Number %d\n", (long long) *seconds, masterPid, slaveProcessID);
            sleep(rand()%2 + 1);
        }
        printf("Process %i Killed with Process ID %i", currentProcess, slaveProcessID);
        kill(slaveProcessID, SIGTERM);
    }

    //destroy semaphore
    if (semctl(semid, 0, IPC_RMID, arg) == -1 && totalProcess == currentProcess) 
    {
        perror("semctl");
        exit(EXIT_FAILURE); 
    }

    else{
        fprintf(testFilePtr, "Semaphore Destroy Success!!!\n");
    }

    printf("Master Program Terminated!\n");
    return SUCCESS;
}