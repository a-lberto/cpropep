#
# Makefile to build Debian package for Rocketworkbench utility suite
# Written by Geoff O'Callaghan <geoff@starbiz.com.au>
# Based on existing Makefile from Rocketworkbench project
# 

.PHONY: clean

all:

	@echo "  --------------------"
	@echo "  Compiling libnum ..."
	@echo "  --------------------"
	@$(MAKE) --no-print-directory -C libnum all

	@echo "  -----------------------"
	@echo "  Compiling libcpropep..."
	@echo "  -----------------------"
	@$(MAKE) --no-print-directory -C libcpropep all

	@echo "  ----------------------"
	@echo "  Compiling libthermo..."
	@echo "  ----------------------"
	@$(MAKE) --no-print-directory -C libthermo all

	@echo "  -----------------------"
	@echo "  Compiling libconvert..."
	@echo "  -----------------------"
	@$(MAKE) --no-print-directory -C libconvert all

	@echo "  ----------------------"
	@echo "  Compiling libcompat..."
	@echo "  ----------------------"
	@$(MAKE) --no-print-directory -C libcompat all

	@echo "  -------------------"
	@echo "  Compiling liblrd..."
	@echo "  -------------------"
	@$(MAKE) --no-print-directory -C liblrd all

	@echo "  --------------------------"
	@echo "  Compiling librockflight..."
	@echo "  --------------------------"
	@$(MAKE) --no-print-directory -C librockflight all

	@echo "  -------------------"
	@echo "  Compiling libmcp..."
	@echo "  -------------------"
	@$(MAKE) --no-print-directory -C libmcp all


clean:
	@echo "  Cleaning libnum..."
	@$(MAKE) --no-print-directory -C libnum clean
	@echo "  Cleaning libcpropep..."
	@$(MAKE) --no-print-directory -C libcpropep clean
	@echo "  Cleaning libthermo..."
	@$(MAKE) --no-print-directory -C libthermo clean
	@echo "  Cleaning libconvert..."
	@$(MAKE) --no-print-directory -C libconvert clean
	@echo "  Cleaning libcompat..."
	@$(MAKE) --no-print-directory -C libcompat clean
	@echo "  Cleaning liblrd..."
	@$(MAKE) --no-print-directory -C liblrd clean
	@echo "  Cleaning librockflight..."
	@$(MAKE) --no-print-directory -C librockflight clean
	@echo "  Cleaning libmcp..."
	@$(MAKE) --no-print-directory -C libmcp clean

