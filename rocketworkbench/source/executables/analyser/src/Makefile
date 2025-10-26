
CC     = gcc
COPT   = -g -Wall -O3 #-pg 

LIB    = -lnum -lm -lgpcp -lconvert
ROOT   = ../..
LIBDIR = -L$(ROOT)/libnum/lib \
	 -L$(ROOT)/gpcp/src/ \
	 -L$(ROOT)/libconvert/lib/

INCDIR = -I$(ROOT)/libnum/include/ \
         -I$(ROOT)/gpcp/src/ \
         -I$(ROOT)/libconvert/include/

DEF    = -DGCC

PROG   = analyser

OBJS   = analyser.o

.SUFFIXES: .c

all: $(PROG)

.c.o:
	$(CC) $(DEF) $(INCDIR) $(COPT) -c $*.c -o $*.o

$(PROG): $(OBJS) $(LIBNAME)
	$(CC) $(COPT) $(OBJS) $(LIBDIR) $(LIB) -o $@

clean:
	rm -f *.o *~

deep-clean: clean
	rm -f $(PROG)
