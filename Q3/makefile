CC=gcc
FLAGS=-g -Wall
LIBS=-lrt -pthread

default:
	$(CC) $(FLAGS) -o manager printmanager_sysv.c printsemaphores.c $(LIBS)
	$(CC) $(FLAGS) -o server printserver_sysv.c printsemaphores.c $(LIBS)
	$(CC) $(FLAGS) -o client printclient_sysv.c printsemaphores.c $(LIBS)
	$(CC) $(FLAGS) -o shutdown shutdown.c

clean:
	-rm manager
	-rm server
	-rm client
