// *****************************************************************************
// shutdown.c
//
// Purpose: Send SIGUSR1 to the specified process. Created to tell the print 
//          server to shutdown.
// Author: Daniel Lovegrove
// Version: Mar 20/2018
// *****************************************************************************

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    long pid;
    char *eptr;

    if (argc != 1) {
        pid = strtol(argv[1], &eptr, 10);
        if ((pid == 0 && eptr == argv[1]) || (*eptr != '\0')) {
            printf("%s is not a valid PID\n", argv[1]);
        } else {
            printf("NOT IMPLEMENTED: sending SIGUSR1 to server\n");
        }
    } else {
        printf("shutdown requires a single argument.\n");
    }
}
