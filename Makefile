CC = gcc
CCFLAGS = $(CFLAGS)

toporobot2survex: main.o survey.o survex_writer.o functions.o
	$(CC) $(CCFLAGS) -o toporobot2survex main.o functions.o survey.o survex_writer.o

main.o: main.c functions.h survey.h survex_writer.h
	$(CC) $(CCFLAGS) -c main.c

functions.o: functions.c functions.h
	$(CC) $(CCFLAGS) -c functions.c

survey.o: survey.c survey.h
	$(CC) $(CCFLAGS) -c survey.c

survex_writer.o: survex_writer.c survex_writer.h
	$(CC) $(CCFLAGS) -c survex_writer.c


clean:
	rm toporobot2survex main.o functions.o survey.o survex_writer.o

