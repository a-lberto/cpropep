# --
# DESTDIR is the place we want to install into 'prefix'
# BIN is where our executables go
# DATA is where the program data go
# CONF is where the program config files go
# (note: This was added for Debian build, but should work ok on other
#        platforms - goc)

DESTDIR=
BIN=$(DESTDIR)/usr/bin
DATA=$(DESTDIR)/usr/share/rocketworkbench/cpropep
CONF=$(DESTDIR)/etc/rocketworkbench

# --

all:

clean:

deep-clean: clean

# --
# Install rules added for Debian build - should be ok on other platforms (goc)
# --

install:	all
	install -d $(DATA)
	install -m 0644 propellant.dat $(DATA)
	install -m 0644 thermo.dat $(DATA)

# --
