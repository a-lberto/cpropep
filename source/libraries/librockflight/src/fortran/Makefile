
SRC = hwm93dist.f hwm93dist_test.f
OBJS = hwm93dist.o hwm93dist_test.o

COBJ = test.o

PROG = hwm93dist_test

CTEST = ctest

OPT = -g -Wall

all: $(PROG) $(CTEST) libtest.a

.c.o :
	$(CC) $(OPT) -c $< -o $@

.f.o : 
	f77 $(OPT) -c $< -o $@

$(PROG): $(OBJS)
	f77 $(OBJS) -o $@

$(CTEST): $(OBJS) $(COBJ)
	$(CC) $(COBJ) $(OBJS) -lm -lg2c -o $@

libtest.a : $(OBJS)
	ar -r $@ $(LIBOBJS)

clean:
	rm -f *.o *~ *.a hwm93dist_test
