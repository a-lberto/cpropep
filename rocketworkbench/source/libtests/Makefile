#
# Makefile to build Debian package for Rocketworkbench utility suite
# Written by Geoff O'Callaghan <geoff@starbiz.com.au>
# Based on existing Makefile from Rocketworkbench project
# 

all:
	@echo "  Building libnum test..."
	@$(MAKE) --no-print-directory -C libnum all
	@echo "  Building libconvert test..."
	@$(MAKE) --no-print-directory -C libconvert all

clean:
	@echo "  Cleaning libnum test..."
	@$(MAKE) --no-print-directory -C libnum clean
	@echo "  Cleaning libconvert test..."
	@$(MAKE) --no-print-directory -C libconvert clean

install:
	@$(MAKE) --no-print-directory -C libnum install
	@$(MAKE) --no-print-directory -C libconvert install
