
CC     = gcc
COPT   = -g -Wall -O3 #-pg 

LIB    =  -lthermo -lm 
ROOT   = ../..
LIBDIR = -L$(ROOT)/libnum/lib/ \
         -L$(ROOT)/libthermo/lib/

INCDIR = -I$(ROOT)/libnum/include/ \
         -I$(ROOT)/libcompat/include/ \
         -I$(ROOT)/libcpropep/include/ \
         -I$(ROOT)/libthermo/include/

DEF = -DGCC 

PROG = prop

OBJS = prop.o 

.SUFFIXES: .c

all: $(PROG)

.c.o:
	$(CC) $(DEF) $(INCDIR) $(COPT) -c $*.c -o $*.o

$(PROG): $(OBJS)
	$(CC) $(COPT) $(OBJS) $(LIBDIR) $(LIB) -o $@


clean:
	rm -f *.o *~ *.m

deep-clean: clean
	rm -f $(PROG)
