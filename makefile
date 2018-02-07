CC = gcc
CFLAGS = -ggdb -std=c99 -Wall -Werror
TARGET = proc_fan
TARGET2 = testsim
OBJS = runsim.o
OBJS2 = testsim.o
default: all
VALGRIND = valgrind --tool=memcheck --verbose --leak-check=full --track-origins=yes --show-leak-kinds=all

all: $(TARGET) $(TARGET2) memcheck
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
runsim.o: runsim.c
	$(CC) $(CFLAGS) -c runsim.c
$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2)
testsim.o: testsim.c
	$(CC) $(CFLAGS) -c testsim.c
memcheck: 
	$(VALGRIND) ./$(TARGET) -n 20 < testing.data > NULL
clean:
	rm *.o
