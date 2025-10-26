LIBDIR = -L$(ROOT)/lib \
	 -L$(ROOT)/source/libraries/gpcp/src

INCDIR = -I$(ROOT)/source/libraries -I$(ROOT)/source/libraries/gpcp/src

DEF    = -DGCC

.SUFFIXES: .c .cxx

include make_source

EXECUTABLE = $(ROOT)/bin/$(PROG)

all: $(EXECUTABLE)

.c.o:
	@echo -n "  "
	$(CC) $(DEF) $(INCDIR) $(COPT) -c $*.c -o $*.o

.cxx.o: @echo -n "  "
	$(CC) $(DEF) $(INCDIR) $(COPT) -c $*.cxx -o $*.o

$(EXECUTABLE): $(OBJS)
	@echo -n "  "
	$(CC) $(COPT) $(OBJS) $(LIBDIR) $(LIB) -o $@
#	@echo -n "  "
#	cp $(PROG) $(ROOT)/bin

clean:
	@rm -f *.o *~

