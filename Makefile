CC=gcc

all: executable clean

executable: main.o client.o server.o appareil.o
	$(CC) -Wall -o main main.o sources/* -lm

client.o: sources/Client.c
	$(CC) -Wall -c sources/Client.c

server.o: sources/Serveur.c
	$(CC) -Wall -c sources/Serveur.c

appareil.o: sources/Appareil.c
	$(CC) -Wall -c sources/Appareil.c

main.o: main.c
	$(CC) -Wall -c main.c

clean:
	rm -f *.o