#
# Makefile to build Debian package for Rocketworkbench utility suite
# Written by Geoff O'Callaghan <geoff@starbiz.com.au>
# Based on existing Makefile from Rocketworkbench project
# 

all:
	@echo " Building libraries..."
	@$(MAKE) -C libraries all
	@echo " Building executables..."
	@$(MAKE) -C executables all
	@echo " Building libraries test..."
	@$(MAKE) -C libtests all

clean:
	@echo " Cleaning libraries..."
	@$(MAKE) -C libraries clean
	@echo " ...done"
	@echo " Cleaning executables..."
	@$(MAKE) -C executables clean
	@echo " ...done"
	@echo " Cleaning libraries test..."
	@$(MAKE) -C libtests clean
	@echo " ...done"

install:	all
	@$(MAKE) -C libraries install
	@$(MAKE) -C executables install
	@$(MAKE) -C libtests install
