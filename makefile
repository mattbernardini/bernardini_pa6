CC = gcc
CFLAGS = -g -std=c99
TARGET = runsim
TARGET2 = testsim
OBJS = runsim.o
OBJS2 = testsim.o
all: $(TARGET) $(TARGET2) clean
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
main.o: runsim.c
	$(CC) $(CFLAGS) -c runsim.c
$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2)
main2.o: testsim.c
	$(CC) $(CFLAGS) -c testsim.c
clean:
	rm *.o