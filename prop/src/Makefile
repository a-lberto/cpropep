
CC   = gcc
COPT = -g -Wall -O3 #-pg -O6\
# -mpentium -ffast-math -funroll-loops -fnonnull-objects\
# -fno-exceptions -fforce-mem -fforce-addr -fcse-follow-jumps\
# -fexpensive-optimizations -march=pentium -fno-rtti #-fomit-frame-pointer

LIB  =  -lthermo -lm #-lcpropep -lnum #-lcruft -lf2c
LIBDIR = -L../../libnum/ -L../../libthermo/src/
INCLUDEDIR = -I../../libnum/ -I../../libcompat/include -I../../libcpropep/include -I../../libthermo/include/

DEF = -DGCC #-DTRUE_ARRAY

PROG = prop

OBJS = prop.o 

.SUFFIXES: .c

all: $(PROG)

.c.o:
	$(CC) $(DEF) $(INCLUDEDIR) $(COPT) -c $*.c -o $*.o

$(PROG): $(OBJS)
	$(CC) $(COPT) $(OBJS) $(LIBDIR) $(LIB) -o $@


clean:
	rm -f *.o *~ *.m

deep-clean: clean
	rm -f $(PROG)
