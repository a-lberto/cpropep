#
# Makefile to build Debian package for Rocketworkbench utility suite
# Written by Geoff O'Callaghan <geoff@starbiz.com.au>
# Based on existing Makefile from Rocketworkbench project
# 

#ROOT = $(PWD) 
#export ROOT

.PHONY: all clean debug

optimize: TYPE="optimize"
optimize: all

debug: TYPE="debug"
debug: all

profile: TYPE="profile"
profile: all

all:
	@echo "Building sources in" $(TYPE) "mode."
	@$(MAKE) --no-print-directory -C source all

clean:
	@echo "Cleaning sources..."
	@$(MAKE) --no-print-directory -C source clean
	@echo "...done"	
	@rm -f lib/*
	@rm -f bin/*

install: all
	@$(MAKE) --no-print-directory -C source install
