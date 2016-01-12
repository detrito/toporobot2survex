CC = gcc
CCFLAGS = $(CFLAGS)

toporobot2survex: main.o survey.o
	$(CC) $(CCFLAGS) -o toporobot2survex main.o survey.o

main.o: main.c survey.h
	$(CC) $(CCFLAGS) -c main.c

survey.o: survey.c survey.h
	$(CC) $(CCFLAGS) -c survey.c

clean:
	rm toporobot2survex main.o survey.o

