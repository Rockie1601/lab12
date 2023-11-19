CC = gcc
CFLAGS = -Wall -pthread

all: lab_12

lab_12: lab_12.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f lab_12
