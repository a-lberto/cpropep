
CC     = gcc
COPT   = -g -Wall -O3 #-pg 

ROOT   = ../..

LIB    = -lMagick -lrockflight -lnum -lm

LIBDIR = -L$(ROOT)/libnum/lib -L/usr/X11R6/lib/ -L../lib/

INCDIR = -I$(ROOT)/libnum/include -I../src/ \
         -I/usr/X11R6/include/magick -I/usr/X11R6/include/

DEF    =

PROG   = viewer

OBJS   = viewer.o

all: $(PROG) $(PROG2)

.c.o:
	$(CC) $(DEF) $(INCDIR) $(COPT) -c $*.c -o $*.o

$(PROG): $(OBJS) $(LIBNAME)
	 $(CC) $(COPT) $(OBJS) $(LIBDIR) $(LIB) -o $@

clean:
	rm -f *.o *~

deep-clean: clean
	rm -f $(PROG)
