// *****************************************************************************
// manager.h
//
// Author: Daniel Lovegrove
// Version: Mar 19/2018
// *****************************************************************************

#ifndef PRINT_MANAGER_SYSV

#define PRINT_MANAGER_SYSV

#define KEY 7763168
#define SIZE 512
#define QUEUE_LEN 4
#define BYTES_PER_SEC 4000
#define BUF_SIZE 32
#define NUM_ITERATIONS 6
#define FILE_SIZE_MAX 40000
#define FILE_SIZE_MIN 500
#define SLEEP_TIME_MAX 3
#define TIME_FORMAT "%Y/%m/%d %H:%M:%S"
#define FILENAME_SIZE 64

typedef struct __PRINT_REQUEST {
    long clientID;
    char filename[FILENAME_SIZE];
    int fileSize;
} PrintRequest;

typedef struct __PRINT_JOB_QUEUE {
    int currLen;
    int currIndex;
    int maxLen;
    PrintRequest queueArray[QUEUE_LEN];
} PrintQueue;

#endif
