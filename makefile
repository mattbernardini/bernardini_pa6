CC = gcc
CFLAGS = -ggdb -std=c99 -Wall -Werror
TARGET = proc_fan
TARGET2 = testsim
OBJS = runsim.o makeargv.o
OBJS2 = testsim.o
default: all
all: $(TARGET) $(TARGET2)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
runsim.o: runsim.c
	$(CC) $(CFLAGS) -c runsim.c
$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2)
testsim.o: testsim.c
	$(CC) $(CFLAGS) -c testsim.c
makeargv.o: makeargv.c
	$(CC) $(CFLAGS) -c makeargv.c
clean:
	rm *.o
