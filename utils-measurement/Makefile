CC = gcc
CFLAGS = -O2 -Wall -g
LFLAGS =

all:	freq_temp_log


freq_temp_log:	freq_temp_log.o
	$(CC) -o freq_temp_log freq_temp_log.o $(LFLAGS)

freq_temp_log.o:	freq_temp_log.c
	$(CC) $(CFLAGS) -c freq_temp_log.c


clean:	
	rm -f *~ *.o freq_temp_log

