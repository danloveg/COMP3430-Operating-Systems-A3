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
#include <string.h>
#include <unistd.h>

void initQueue(char * shmAddress);
void startClientAndServerProcs();


int main(int argc, char *argv[]) {
    int shmid, status = 0;
    char * shmseg;

    // Create the shared segment for client and server
    if ((shmid = shmget(KEY, SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget failed");
        exit(1);
    } else {
        printf("SysV segment created.\n");
    }

    // Attach segment
    if ((shmseg = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat failed");
        exit(1);
    }

    initQueue(shmseg);

    // Detach segment
    if ((status = shmdt(shmseg)) != 0) {
        perror("shmdt failed");
        exit(1);
    }

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
