// *****************************************************************************
// printserver_sysv.c
//
// Purpose: "Print" jobs submitted by the client from the shared queue.
// Author: Daniel Lovegrove
// Version: Mar 15/2018
// *****************************************************************************

#include "manager.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>


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

    // Signal to client in the shared segment that we are ready.

    // Continuously print jobs from the queue

    printf("Print server exiting.\n");
}
