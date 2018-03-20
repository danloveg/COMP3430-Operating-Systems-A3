// *****************************************************************************
// printsemaphores.c
//
// Purpose: Provide semaphore creation, opening, and closing for print manager
//          and client/server.
// Author: Daniel Lovegrove
// Version: Mar 19/2018
// *****************************************************************************

#include "manager.h"
#include "printsemaphores.h"
#include <assert.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

void createNamedSemaphore(char * name, int value);

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
 * Open the three named semaphores.
 */
void openSemaphores(sem_t ** mutex, sem_t ** empty, sem_t ** full) {
    if ((*mutex = sem_open(MUTEX_SEM_NAME, O_RDWR)) == SEM_FAILED) {
        printf("Client could not get mutex semaphore\n");
        exit(1);
    }
    if ((*full = sem_open(FULL_SEM_NAME, O_RDWR)) == SEM_FAILED) {
        printf("Client could not get full semaphore\n");
        exit(1);
    }
    if ((*empty = sem_open(EMPTY_SEM_NAME, O_RDWR)) == SEM_FAILED) {
        printf("Client could not get empty semaphore\n");
        exit(1);
    }
}


/**
 * Close the three named semaphores.
 */
void closeSemaphores(sem_t ** mutex, sem_t ** empty, sem_t ** full) {
    if (*mutex != NULL) {
        sem_close(*mutex);
    }
    if (*full != NULL) {
        sem_close(*full);
    }
    if (*empty != NULL) {
        sem_close(*empty);
    }
}
