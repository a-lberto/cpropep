SRC = hwm87dist.f hwm90dist.f hwm93dist.f hwm87dist_test.f

OBJS = hwm87dist.o hwm90dist.o hwm93dist.o hwm87dist_test.o


.c.o : 
	$(FF) -c $< -o $@

libtest.a : $(OBJS)
	ar -r $@ $(LIBOBJS)
