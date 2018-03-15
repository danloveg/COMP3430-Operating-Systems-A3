#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// SIGINT is generated from the keyboard using CTRL-C
// SIGQUIT is generated from the keyboard using CTRL-\

int main ( void ) {
  int i ;
  void catch_signal(int);
  if (signal(SIGINT,catch_signal)==SIG_ERR){
    perror("setup of SIGINT handler failed"); exit(1);
  }
  if (signal(SIGQUIT,catch_signal)== SIG_ERR) {
    perror("setup of SIGQUIT handler failed"); exit(1);
  }
  for (i=0;;i++) {/*loop forever*/
    printf("%d\n",i); sleep(1); //sleep 1 second
  }
}

void catch_signal(int the_signal ) {
  /*arrange to catch signal again*/
  signal(the_signal,catch_signal); 
  printf("\nSignal %d received.\n",
		   the_signal);
  if (the_signal==SIGQUIT) {
    printf("Exiting...\n") ; 
    exit(0);
  }
}

