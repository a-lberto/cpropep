CXX      = g++
CC       = gcc
CXXFLAGS = -g -Wall
CFLAG    = -g -Wall


LEXER    = motor

CXXSRC   = simulation.cc modeles.cc rocket.cc lsode.cc
CXXOBJS  = simulation.o modeles.o rocket.o lsode.o


COBJS    = motor.yy.o

LIBNAME = librocket.a

.SUFFIXES: .cc

.cc.o:
	$(CXX) $(DEF) $(INCLUDE) $(CXXFLAGS) -c $*.cc -o $*.o

$(LIBNAME):  $(CXXOBJS) $(COBJS)
	ar -r $@ $(CXXOBJS) $(COBJS)
	ranlib $@

$(COBJS): $(LEXER).yy.c
	$(CXX) $(CFLAG) -c $< -o $@ 

$(LEXER).yy.c: $(LEXER).l
	flex -o$(LEXER).yy.c $<

clean:
	rm -f *.o *~

clean-all: clean
	rm $(LIBNAME)

.PHONY: clean





