// *****************************************************************************
// shutdown.c
//
// Purpose: Send SIGUSR1 to the specified process. Created to tell the print 
//          server to shutdown.
// Author: Daniel Lovegrove
// Version: Mar 20/2018
// *****************************************************************************

#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    long pid;
    int status;
    char *eptr;

    if (argc != 1) {
        pid = strtol(argv[1], &eptr, 10);
        if ((pid == 0 && eptr == argv[1]) || (*eptr != '\0')) {
            printf("%s is not a valid PID\n", argv[1]);
        } else {
            // Kill the process after getting valid PID
            if ((status = kill(pid, SIGUSR1)) != 0) {
                if (errno == EINVAL) {
                    printf("Invalid signal\n");
                } else if (errno == EPERM) {
                    printf("You do not have permission to kill the targeted process(es)\n");
                } else if (errno == ESRCH) {
                    printf("The process(es) do(es) not exist\n");
                }
            } else {
                printf("Sent SIGUSR1 to process %ld\n", pid);
            }
        }
    } else {
        printf("shutdown requires a single argument.\n");
    }
}
