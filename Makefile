CC = gcc
CCFLAGS = $(CFLAGS)

#include /etc/make.conf
#include /etc/makepkg.conf

toporobot2survex: main.o functions.o toporobot_reader.o container.o \
	survex_writer.o
	$(CC) $(CCFLAGS) -o toporobot2survex main.o functions.o toporobot_reader.o \
		container.o survex_writer.o

main.o: main.c toporobot_reader.h container.h survex_writer.h vector.h \
	 functions.h
	$(CC) $(CCFLAGS) -c main.c

functions.o: functions.c functions.h
	$(CC) $(CCFLAGS) -c functions.c

toporobot_reader.o: toporobot_reader.c toporobot_reader.h functions.h \
	container.h
	$(CC) $(CCFLAGS) -c toporobot_reader.c

container.o: container.c container.c vector.h
	$(CC) $(CCFLAGS) -c container.c

survex_writer.o: survex_writer.c survex_writer.h functions.h
	$(CC) $(CCFLAGS) -c survex_writer.c

clean:
	rm toporobot2survex main.o functions.o toporobot_reader.o container.o \
		survex_writer.o
