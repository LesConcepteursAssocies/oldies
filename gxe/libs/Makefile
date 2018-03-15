#
# Gnu Xml Engine
# philippe Guillaumaud <pguillaumaud@april.org>
#
# Makefile generique des modules de gxe
#

CC=gcc

# CFGLIB=$(shell /usr/bin/glib-config --cflags)
CFGLIB=$(shell /usr/local/bin/glib-config --cflags)
CFXML=$(shell /usr/bin/xml-config --cflags)
CFLAGS=-O2 -Wall -fPIC -shared -D_REENTRANT

LFLAGS=-lgmodule -lpthread
# LFGLIB=$(shell /usr/bin/glib-config --libs)
LFGLIB=$(shell /usr/local/bin/glib-config --libs)
LFXML=$(shell /usr/bin/xml-config --libs)

INC=-I../src/

SRCS_L1=libgxetest.c
OBJS_L1=libgxetest.o

default	: 	libgxetest

.SUFFIXES: 	.c

.c.o:
	$(CC) $(INC) -c $(CFLAGS) $(CFGLIB) $(CFXML) $< -o $*.o

depend:
	@makedepend -Y $(SRCS_L1) 2> /dev/null

libgxetest:	$(OBJS_L1)
	$(CC) $(INC) $(CFLAGS) -Wl,-soname,libgxetest.so -o libgxetest.so $(OBJS_L1) $(LFLAGS) $(LFGLIB) $(LFXML)

clean:
	rm -f *.o *.so core

install:
	cp -f *.so ../modules
#
# ---
#
# DO NOT DELETE
