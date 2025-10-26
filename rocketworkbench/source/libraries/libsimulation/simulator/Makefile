

CXX      = c++
CC       = gcc
CXXFLAGS = -g -Wall
CFLAG    = -g -Wall 

LIBS     = -lm -lrocket -lnum -lcruft -lf2c -ldl
LIBDIR   = -L/usr/lib -L/usr/lib/octave-2.0.14.93 -L../ -L../../libnum/



INCLUDE  += -I../ -I../../libnum/

SRC      = c++rocket.cc
OBJS     = c++rocket.o #ccmalloc.o

PROG     = c++rocket


.cc.o:
	$(CXX) $(DEF) $(CXXFLAGS) $(INCLUDE) -c $*.cc -o $*.o


$(PROG): $(OBJS)
	$(CXX) $(DEF) $(OBJS) $(LIBDIR) $(LIBS)  -o $@


clean:
	rm -f *.o *~

clean-all: clean
	rm $(PROG)

.PHONY: clean
