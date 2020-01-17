CC = gcc
CFLAGS = -Wall -O2
LFLAGS =

all:	calculate_green500

calculate_green500: calculate_green500.o
	$(CC) $(LFLAGS) -o calculate_green500 calculate_green500.o

calculate_green500.o:	calculate_green500.c
	$(CC) $(CFLAGS) -c calculate_green500.c

clean:	
	rm -f *.o *~ calculate_green500


