#
# libserial
# Outils de manipulation des ports séries
#

CC=gcc

CFLAGS=-O2 -Wall -fPIC -shared

INC=-I../

SRCS=libserial.c
OBJS=libserial.o

default	: 	libserial

.SUFFIXES: 	.c

.c.o:
	$(CC) $(INC) -c $(CFLAGS) $< -o $*.o

depend:
	@makedepend -Y $(SRCS) 2> /dev/null

libserial:	$(OBJS)
	$(CC) $(INC) $(CFLAGS) -Wl,-soname,libserial.so -o libserial.so $(OBJS)

test:	libserial.h test.c
	$(CC) test.c -o test -L./ -lserial

clean:
	rm -f test *.o *.so core

#
# ---
#
# DO NOT DELETE

libserial.o: libserial.h
