// *****************************************************************************
// printmanager.c
//
// Purpose: Create shared memory segment and start up print server/client
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
    int shmid = 0;

    /*
     * Create the segment (IPC_CREAT)
    */
    if ((shmid = shmget(KEY, SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget failed");
        exit(1);
    } else {
        printf("SysV segment created.\n");
    }
}
