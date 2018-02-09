CC = gcc
CFLAGS = -ggdb -std=c11 -Wall -Werror
TARGET = proc_fan
TARGET2 = testsim
OBJS = proc_fan.o
OBJS2 = testsim.o
default: all

all: $(TARGET) $(TARGET2)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
proc_fan.o: proc_fan.c
	$(CC) $(CFLAGS) -c proc_fan.c
$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2)
testsim.o: testsim.c
	$(CC) $(CFLAGS) -c testsim.c
clean:
	rm *.o $(TARGET) $(TARGET2)
