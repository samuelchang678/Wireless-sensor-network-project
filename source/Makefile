C = mpicc

CFLAGS += -c -Wall -Wextra -std=c99 -pedantic -g
LDFLAGS =

all: main

main: main_file.o slave.o master.o
	$(C) $(LDFLAGS) main_file.o slave.o master.o -o main

main_file.o: main_file.c
	$(C) $(CFLAGS) main_file.c

slave.o: slave.c
	$(C) $(CFLAGS) slave.c

master.o: master.c
	$(C) $(CFLAGS) master.c

.PHONY: clean

clean:
	rm -f *.o *~

run:
	mpirun -np 61 main
