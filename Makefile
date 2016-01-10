CC = gcc
CCFLAGS = $(CFLAGS)

toporobot2survex: main.o
	$(CC) $(CCFLAGS) -o toporobot2survex main.o

main.o: main.c
	$(CC) $(CCFLAGS) -c main.c

clean:
	rm toporobot2survex main.o

