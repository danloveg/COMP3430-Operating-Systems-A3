// *****************************************************************************
// semaphores.h
//
// Author: Daniel Lovegrove
// Version: Mar 19/2018
// *****************************************************************************

#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#define MUTEX_SEM_NAME "/mutex.sem"
#define EMPTY_SEM_NAME "/empty.sem"
#define FULL_SEM_NAME "/full.sem"

void createSemaphores();
void createNamedSemaphore(char * name, int value);

#endif
