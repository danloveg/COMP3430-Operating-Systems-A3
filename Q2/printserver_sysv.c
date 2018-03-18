// *****************************************************************************
// printserver_sysv.c
//
// Purpose: "Print" jobs submitted by the client from the shared queue.
// Author: Daniel Lovegrove
// Version: Mar 18/2018
// *****************************************************************************

#include "manager.h"
#include <assert.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

PrintQueue * queue;


int main(int argc, char *argv[]) {
    char *shmseg;
    int shmid;

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

    // Continuously print jobs from the queue
    while (1) {

    }
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
