CC     = gcc
COPT   = -g -Wall
LIB    =  -lcpropep -lthermo -lm -lnum -lcgi 
ROOT   = ../..
LIBDIR = -L$(ROOT)/libnum/lib/ \
         -L$(ROOT)/libcpropep/lib/ \
         -L$(ROOT)/libthermo/lib/

INCDIR = -I$(ROOT)/libnum/include/ \
         -I$(ROOT)/libcpropep/include/ \
         -I$(ROOT)/libthermo/include/ \
         -I$(ROOT)/libcompat/include/

DEF = -DGCC -DSOURCEFORGE 

PROG = cgitest

OBJS = cgitest.o


.SUFFIXES: .c

all: $(PROG)

.c.o:
	$(CC) $(DEF) $(INCDIR) $(COPT) -c $*.c -o $*.o


$(PROG): $(OBJS)
	$(CC) $(COPT) $(OBJS) $(LIBDIR) $(LIB) -o $@


clean:
	rm -f *.o *~

deep-clean: clean
	rm -f cgitest

upload:
	scp $(PROG) antoine@rocketworkbench.sourceforge.net:/home/groups/rocketworkbench/cgi-bin/
