
CC     = gcc
COPT   = -g -Wall -O3 #-pg 

ROOT   = ../..
LIB    = -lrockflight -lnum -lm
LIBDIR = -L$(ROOT)/libnum/lib \
	 -L../lib/
INCDIR = -I$(ROOT)/libnum/include \
	 -I../include/
LIBNAME = librockflight.a

DEF    =
PROG   = rocflight
OBJS   = rocflight.o

LIBOBJS  = eom.o atmos.o aero.o engine.o

all: $(LIBNAME) $(PROG) 

.c.o:
	$(CC) $(DEF) $(INCDIR) $(COPT) -c $*.c -o $*.o

$(LIBNAME): $(LIBOBJS)
	ar -r $@ $(LIBOBJS)
	ranlib $@
	mv $(LIBNAME) ../lib

	
$(PROG): $(OBJS) $(LIBNAME)
	$(CC) $(COPT) $(OBJS) $(LIBDIR) $(LIB) -o $@

clean:
	rm -f *.o *~

deep-clean: clean
	rm -f ../lib/$(LIBNAME) $(PROG)
