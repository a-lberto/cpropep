
LIBDIR = -L$(ROOT)/lib/
INCDIR = -I$(ROOT)/source/libraries

LIBFILE = $(ROOT)/lib/$(LIBNAME)

DEF = -DLINUX -DGCC

include make_source

all: $(LIBFILE) 

.c.o:
#	@echo $<
	@echo -n "  "
	$(CC) $(DEF) $(INCDIR) $(COPT) -c $< -o $@

$(LIBFILE): $(LIBOBJS)
	@echo -n "  " 
	ar -r $@ $(LIBOBJS) 
	@echo -n "  " 
	ranlib $@
#	@echo -n "  " 
#	mv $(LIBNAME) $(ROOT)/lib/
	@echo ""
clean:
	@rm -f *.o *~

deep-clean: clean
	rm -f $(ROOT)/lib/$(LIBNUM)

