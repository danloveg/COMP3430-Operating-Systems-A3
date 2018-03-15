CC=gcc
FLAGS=-Wall
LIBS=-lrt

default:
	$(CC) $(FLAGS) -o manager printmanager_sysv.c $(LIBS)
	$(CC) $(FLAGS) -o server printserver_sysv.c $(LIBS)
	$(CC) $(FLAGS) -o client printclient_sysv.c $(LIBS)

clean:
	-rm manager
	-rm server
	-rm client
