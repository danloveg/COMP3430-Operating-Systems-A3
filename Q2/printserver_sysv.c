// *****************************************************************************
// printserver_sysv.c
//
// Purpose: "Print" jobs submitted by the client from the shared queue.
// Author: Daniel Lovegrove
// Version: Mar 19/2018
// *****************************************************************************

#include "manager.h"
#include "printsemaphores.h"
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

void removeFromBoundedBuffer(PrintRequest ** req);
bool leave(PrintRequest **req);

PrintQueue * queue = NULL;
sem_t * mutex, * full, * empty;


int main(int argc, char *argv[]) {
    char *shmseg;
    int shmid;
    long pid = getpid();
    int timeToPrint;
    time_t currentTime;
    struct tm * tmInfo;
    char time_buf[BUF_SIZE];
    char * timeString = &time_buf[0];
    PrintRequest * job = NULL;
    PrintRequest * currentJob = NULL;

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

    if ((currentJob = (PrintRequest *) malloc(sizeof(PrintRequest))) == NULL) {
        printf("malloc failed\n");
        exit(1);
    }

    // Continuously print jobs from the queue
    while (1) {
        removeFromBoundedBuffer(&job);
        memcpy(currentJob, job, sizeof(PrintRequest)); // Copy in case shared data changes
        assert(currentJob != NULL && "Job cannot be NULL");

        time(&currentTime);
        tmInfo = localtime(&currentTime);
        strftime(timeString, BUF_SIZE, TIME_FORMAT, tmInfo);

        printf("Printer %ld STARTED print job:\n", pid);
        printf("\tTime:      %s\n", timeString);
        printf("\tClient ID: %ld\n", currentJob -> clientID);
        printf("\tFilename:  %s\n", &(currentJob -> filename)[0]);
        printf("\tFile size: %d\n", currentJob -> fileSize);

        // Sleep to mimic printing
        timeToPrint = (currentJob -> fileSize) / BYTES_PER_SEC;
        sleep(timeToPrint);

        time(&currentTime);
        tmInfo = localtime(&currentTime);
        strftime(timeString, BUF_SIZE, TIME_FORMAT, tmInfo);

        printf("Printer %ld FINISHED print job:\n", pid);
        printf("\tTime:      %s\n", timeString);
        printf("\tClient ID: %ld\n", currentJob -> clientID);
        printf("\tFilename:  %s\n", &(currentJob -> filename)[0]);
        printf("\tFile size: %d\n", currentJob -> fileSize);
    }
}


/**
 * Removes a print request from the bounded queue. Uses the three named
 * semaphores created by the manager to facilitate safe access to the queue.
 */
void removeFromBoundedBuffer(PrintRequest ** req) {
    assert(queue != NULL && "Queue must be initialized");

    sem_wait(full);  // Wait until at least one full space
    sem_wait(mutex); // Lock the mutex
    leave(req);
    sem_post(mutex); // Unlock the mutex
    sem_post(empty); // Increment the amount of empty spaces
}


/**
 * Remove an item from the queue.
 */
bool leave(PrintRequest **req) {
    assert(queue != NULL && "Queue must be initialized");
    bool reqReturned = false;

    // If not empty...
    if ((queue -> currLen) != 0) {
        (*req) = &(queue -> queueArray[queue -> currIndex]);
        queue -> currIndex = (queue -> currIndex + 1) % queue -> maxLen;
        queue -> currLen--;
        reqReturned = true;
    }

    return reqReturned;
}
