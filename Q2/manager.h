// *****************************************************************************
// printmanager.c
//
// Author: Daniel Lovegrove
// Version: Mar 15/2018
// *****************************************************************************

#ifndef PRINT_MANAGER_SYSV

#define PRINT_MANAGER_SYSV

#define KEY 7763168
#define SIZE 1024
#define QUEUE_LEN 4

typedef struct __PRINT_REQUEST {
    long clientID;
    char filename[64];
    int fileSize;
} PrintRequest;

typedef struct __PRINT_JOB_QUEUE {
    int currLen;
    int currIndex;
    int maxLen;
    PrintRequest queueArray[QUEUE_LEN];
} PrintQueue;

#endif
