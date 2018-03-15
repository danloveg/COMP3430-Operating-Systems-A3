// *****************************************************************************
// printmanager.c
//
// Purpose: Create shared memory segment and start up print server/client
// Author: Daniel Lovegrove
// Version: Mar 15/2018
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
#include <unistd.h>

void startClientAndServerProcs();


int main(int argc, char *argv[]) {
    int shmid = 0;

    // Create the shared segment for client and server
    if ((shmid = shmget(KEY, SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget failed");
        exit(1);
    } else {
        printf("SysV segment created.\n");
    }

    // Start client and server
    startClientAndServerProcs();
}


/**
 * Starts the client and print server in separate processes
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
        pid = fork();

        if (pid == 0) {
            ret = execv("./server", serverArgs);
            assert(ret != 0 && "Client failed");
        }
    }
}
