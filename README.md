# Scheduling and SysV Memory Segments in C
### COMP 3430 Winter 2018 Assignment 3

## Overview
Assignment 3 involves synchronization, signalling, and SysV memory segments in C. Q2 and Q3 are written in code (all in C). Question 2 is a recreation of the print server/client from Assignment 2, this time built using processes instead of threads. Named semaphores are used to allow safe access to a print job queue in a SysV shared memory segment. Question 3 adds another program to question 2 that can send a SIGUSR1 signal to the server to allow it to clean up and shutdown.

## Details
All of the code has only been tested on Linux (Scientific and Ubuntu) and cannot be guaranteed to work on Mac or Windows. The assignment questions that are not in code are in the PDF file at the root of this repository.

## Running the Code

### Question 2

From a terminal, descend into the Q2 directory: `cd Q2`. From there, build the project files: `make`. Finally, run the print server/client by executing the manager: `./manager`.

**Do not** make the mistake of running the client or the server on their own. This will not work since the manager sets up the environment for the client and server before physically running them with an `execv()` call. As well, the manager depends on the client and the server being in the same directory as it, so do not move the `client` or `server` executable files.

### Question 3

From a terminal, descend into the Q3 directory: `cd Q3`. The process of running question 3 is the exact same as question 2, although question 3 adds another executable program called `shutdown`.

After the server and client are running, you can use the `shutdown` executable to effectively shut the server down. The shutdown program takes one argument, to run it use: `./shutdown <printer pid>`. To get the printer's Process ID (pid), look at the messages the printer outputs. Each message starts with `Printer  <number> ...`, that number is the printer's pid.
