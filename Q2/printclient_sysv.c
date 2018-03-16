// *****************************************************************************
// printclient_sysv.c
//
// Purpose: Add jobs to be "printed" to the print queue
// Author: Daniel Lovegrove
// Version: Mar 15/2018
// *****************************************************************************

#include "manager.h"
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
    int shmid;
    char *shmseg;

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

    // Wait for the printer to have signalled

    // Add 6 print jobs to the queue

    // Exit when done

    printf("Client exiting.\n");
}
