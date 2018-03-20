// *****************************************************************************
// printclient_sysv.c
//
// Purpose: Add jobs to be "printed" to the print queue
// Author: Daniel Lovegrove
// Version: Mar 19/2018
// *****************************************************************************

#include "manager.h"
#include "printsemaphores.h"
#include <assert.h>
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

PrintQueue * queue = NULL;
sem_t * mutex, * full, * empty;


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

    openSemaphores(&mutex, &empty, &full);

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

    closeSemaphores(&mutex, &empty, &full);

    // Detach segment
    if (shmdt(shmseg) != 0) {
        perror("shmdt failed");
        exit(1);
    }

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
