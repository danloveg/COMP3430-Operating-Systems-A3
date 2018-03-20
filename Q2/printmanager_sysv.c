// *****************************************************************************
// printmanager.c
//
// Purpose: Create shared memory segment and queue, then start up print server
//          and client. This process will be replaced by the server process so
//          that it will exit when the server exits.
// Author: Daniel Lovegrove
// Version: Mar 18/2018
// *****************************************************************************

#include "manager.h"
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

void initQueue(char * shmAddress);
void createSemaphores();
void createNamedSemaphore(char * name, int value);
void startClientAndServerProcs();


int main(int argc, char *argv[]) {
    int shmid, status = 0;
    char * shmseg;

    // Create the shared segment for client and server
    if ((shmid = shmget(KEY, SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget failed");
        exit(1);
    }

    // Attach segment
    if ((shmseg = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat failed");
        exit(1);
    }

    initQueue(shmseg);
    createSemaphores();

    // Detach segment
    if ((status = shmdt(shmseg)) != 0) {
        perror("shmdt failed");
        exit(1);
    }

    printf("SysV segment setup complete, starting Client and Server.\n");

    // Start client and server then exit
    startClientAndServerProcs();
}


/**
 * Initialize the print queue and place it in shared memory. The print queue is
 * placed at the starting address of the shared memory segment.
 *
 * @param char * shmAddress: The address of the initialized shared memory
 * segment
 */
void initQueue(char * shmAddress) {
    // Create queue
    PrintQueue * queue = (PrintQueue *) calloc(1, sizeof(PrintQueue));
    queue -> maxLen = QUEUE_LEN;
    queue -> currLen = 0;
    queue -> currIndex = 0;

    // Place the queue in shared memory
    memcpy(shmAddress, queue, sizeof(PrintQueue));
}


/**
 * Create the three named semaphores needed for the client and server to insert
 * and remove from the bounded queue. The following webpage was very helpful:
 * https://stackoverflow.com/questions/32205396/share-posix-semaphore-among-multiple-processes
 */
void createSemaphores() {
    createNamedSemaphore(MUTEX_SEM_NAME, 1); // Init val 1 is a mutex
    createNamedSemaphore(EMPTY_SEM_NAME, QUEUE_LEN);
    createNamedSemaphore(FULL_SEM_NAME, 0);
}


/**
 * Create a single named semaphore with the specified name and starting value.
 *
 * @param char * name: The name of the semaphore
 * @param int value: The starting value of the semaphore
 */
void createNamedSemaphore(char * name, int value) {
    assert(name != NULL && "Named semaphore cannot have a null name");
    sem_t * semaphore;

    // Create named semaphore. If fails, unlink and try again
    if ((semaphore = sem_open(name, O_CREAT | O_EXCL, 0777, value)) == SEM_FAILED) {
        sem_unlink(name);
        if ((semaphore = sem_open(name, O_CREAT | O_EXCL, 0777, value)) == SEM_FAILED) {
            char errBuf[128];
            char *errorMessage = &errBuf[0];
            sprintf(errorMessage, "Error creating semaphore with name %s", name);
            perror(errorMessage);
            exit(1);
        }
    }

    // We don't need this semaphore so close it
    if (sem_close(semaphore) < 0) {
        perror("sem_close failed");
        sem_unlink(name);
        exit(1);
    }
}


/**
 * Starts the client and print server. The client is run in a child process and
 * the server is run in this process. As such, this process is replaced by the
 * server.
 */
void startClientAndServerProcs() {
    int pid, ret;
    char *const clientArgs[] = {"client", 0};
    char *const serverArgs[] = {"server", 0};

    pid = fork();

    if (pid == 0) {
        ret = execv("./client", clientArgs);
        assert(ret != 0 && "Client failed");
    } else {
        ret = execv("./server", serverArgs);
        assert(ret != 0 && "Client failed");
    }
}
