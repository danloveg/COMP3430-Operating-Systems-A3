// *****************************************************************************
// printclient_sysv.c
//
// Purpose: Add jobs to be "printed" to the print queue
// Author: Daniel Lovegrove
// Version: Mar 18/2018
// *****************************************************************************

#include "manager.h"
#include "printsemaphores.h"
#include <assert.h>
#include <semaphore.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void insertIntoBoundedBuffer(PrintRequest * req);
bool enter(PrintRequest * req);
void openSemaphores();
void closeSemaphores();

PrintQueue * queue = NULL;
sem_t * mutex;
sem_t * full;
sem_t * empty;


int main(int argc, char *argv[]) {
    int shmid, i, filesize;
    char *shmseg;
    long pid = getpid();
    unsigned int randseed = (unsigned int) pid;
    char filename[FILENAME_SIZE];
    PrintRequest request;

    // Locate segment
    if ((shmid = shmget(KEY, SIZE, 0666)) < 0) {
        perror("shmget failed");
        exit(1);
    }

    // Attach segment
    if ((shmseg = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat failed");
        exit(1);
    }

    // The manager placed the queue at the first address
    queue = (PrintQueue *) shmseg;

    openSemaphores();


    // Add 6 print jobs to the queue
    for (i = 0; i < NUM_ITERATIONS; i++) {
        // Filesize between MIN and MAX
        filesize = (rand_r(&randseed) % (FILE_SIZE_MAX + 1 - FILE_SIZE_MIN)) + FILE_SIZE_MIN;

        // Put the filename in the buffer, then duplicate it to a new string
        sprintf(filename, "File-%ld-%d", pid, i);

        // Create the request job
        request.clientID = pid;
        strcpy(request.filename, filename);
        request.fileSize = filesize;

        insertIntoBoundedBuffer(&request);

        // Sleep for up to MAX seconds
        sleep(rand_r(&randseed) % (SLEEP_TIME_MAX + 1));
    }

    closeSemaphores();

    // Exit when done
    printf("Client exiting.\n");

    exit(0);
}


/**
 * Insert a print request into the bounded queue. Uses the three named
 * semaphores created by the manager to facilitate safe access to the queue.
 */
void insertIntoBoundedBuffer(PrintRequest * req) {
    assert(queue != NULL && "Queue must be initialized");

    sem_wait(empty); // Wait if zero empty slots
    sem_wait(mutex); // Lock mutex
    enter(req);
    sem_post(mutex); // Unlock mutex
    sem_post(full);  // Increment full spaces
}


/**
 * Add an item to the queue.
 */
bool enter(PrintRequest * req) {
    assert(queue != NULL && "Queue must be initialized");
    bool entered = false;

    // If not full...
    if ((queue -> currLen) != (queue -> maxLen)) {
        queue -> queueArray[(queue -> currIndex + queue -> currLen) % queue -> maxLen] = *req;
        //memcpy(&(queue -> queueArray[(queue -> currIndex + queue -> currLen) % queue -> maxLen]), req, sizeof(PrintRequest));
        queue -> currLen++;
        entered = true;
    }

    return entered;
}


/**
 * Open the three named semaphores.
 */
void openSemaphores() {
    if ((mutex = sem_open(MUTEX_SEM_NAME, O_RDWR)) == SEM_FAILED) {
        printf("Client could not get mutex semaphore\n");
        exit(1);
    }
    if ((full = sem_open(FULL_SEM_NAME, O_RDWR)) == SEM_FAILED) {
        printf("Client could not get full semaphore\n");
        exit(1);
    }
    if ((empty = sem_open(EMPTY_SEM_NAME, O_RDWR)) == SEM_FAILED) {
        printf("Client could not get empty semaphore\n");
        exit(1);
    }
}


/**
 * Close the three named semaphores.
 */
void closeSemaphores() {
    if (mutex != NULL) {
        sem_close(mutex);
    }
    if (full != NULL) {
        sem_close(full);
    }
    if (empty != NULL) {
        sem_close(empty);
    }
}
