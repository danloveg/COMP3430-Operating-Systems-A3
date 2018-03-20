// *****************************************************************************
// semaphores.h
//
// Author: Daniel Lovegrove
// Version: Mar 19/2018
// *****************************************************************************

#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <semaphore.h>

#define MUTEX_SEM_NAME "/mutex.sem"
#define EMPTY_SEM_NAME "/empty.sem"
#define FULL_SEM_NAME "/full.sem"

void createSemaphores();
void createNamedSemaphore(char * name, int value);
void openSemaphores(sem_t ** mutex, sem_t ** empty, sem_t ** full);
void closeSemaphores(sem_t ** mutex, sem_t ** empty, sem_t ** full);

#endif
