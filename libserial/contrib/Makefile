#
# libserial
# Outils de manipulation des ports séries
#

CC=gcc

CFLAGS=-O2 -Wall -fPIC -shared

INC=-I../

SRCS=serialwatch.c
OBJS=serialwatch.o

default	: 	serialwatch

.SUFFIXES: 	.c

.c.o:
	$(CC) $(INC) -c $(CFLAGS) $< -o $*.o

depend:
	@makedepend -Y $(SRCS) 2> /dev/null

serialwatch:	../libserial.h serialwatch.c
	$(CC) $(INC) serialwatch.c -o serialwatch -L../ -lserial

clean:
	rm -f serialwatch *.o *.so core

#
# ---
#
# DO NOT DELETE

libserial.o: libserial.h
