#
# Makefile to build Debian package for Rocketworkbench utility suite
# Written by Geoff O'Callaghan <geoff@starbiz.com.au>
# Based on existing Makefile from Rocketworkbench project
# 

all:
	@echo "  --------------------"
	@echo "  Building analyser..."
	@echo "  --------------------"
	@$(MAKE) --no-print-directory -C analyser all

	@echo "  -------------------"
	@echo "  Building cpropep..."
	@echo "  -------------------"
	@$(MAKE) --no-print-directory -C cpropep all

#	@echo "  Building cpropep-2..."
#	@$(MAKE) --no-print-directory -C cpropep-2 all

	@echo "  -----------------------"
	@echo "  Building cpropep-web..."
	@echo "  -----------------------"
	@$(MAKE) --no-print-directory -C cpropep-web all

	@echo "  ---------------"
	@echo "  Building gba..."
	@echo "  ---------------"
	@$(MAKE) --no-print-directory -C gba all

#	@echo "  Building grainanalysis..."
#	@$(MAKE) --no-print-directory -C grainanalysis all

	@echo "  ---------------"
	@echo "  Building lrd..."
	@echo "  ---------------"
	@$(MAKE) --no-print-directory -C lrd all

	@echo "  -------------------"
	@echo "  Building lrd-web..."
	@echo "  -------------------"
	@$(MAKE) --no-print-directory -C lrd-web all

	@echo "  ---------------"
	@echo "  Building mcp..."
	@echo "  ---------------"
	@$(MAKE) --no-print-directory -C mcp all

	@echo "  --------------------"
	@echo "  Building profiler..."
	@echo "  --------------------"
	@$(MAKE) --no-print-directory -C profiler all

	@echo "  ----------------"
	@echo "  Building prop..."
	@echo "  ----------------"
	@$(MAKE) --no-print-directory -C prop all

	@echo "  ----------------------"
	@echo "  Building rockflight..."
	@echo "  ----------------------"
	@$(MAKE) --no-print-directory -C rockflight all

clean:
	@echo "  Cleaning analyser..."
	@$(MAKE) --no-print-directory -C analyser clean
	@echo "  Cleaning cpropep..."
	@$(MAKE) --no-print-directory -C cpropep clean
#	@echo "  Cleaning cpropep-2..."
#	@$(MAKE) --no-print-directory -C cpropep-2 clean
	@echo "  Cleaning cpropep-web..."
	@$(MAKE) --no-print-directory -C cpropep-web clean
	@echo "  Cleaning gba..."
	@$(MAKE) --no-print-directory -C gba clean
#	@echo "  Cleaning grainanalysis..."
#	@$(MAKE) --no-print-directory -C grainanalysis clean
	@echo "  Cleaning lrd..."
	@$(MAKE) --no-print-directory -C lrd clean
	@echo "  Cleaning lrd-web..."
	@$(MAKE) --no-print-directory -C lrd-web clean
	@echo "  Cleaning mcp..."
	@$(MAKE) --no-print-directory -C mcp clean
	@echo "  Cleaning profiler..."
	@$(MAKE) --no-print-directory -C profiler clean
	@echo "  Cleaning prop..."
	@$(MAKE) --no-print-directory -C prop clean
	@echo "  Cleaning rockflight..."
	@$(MAKE) --no-print-directory -C rockflight clean

install:
	@$(MAKE) --no-print-directory -C analyser install
	@$(MAKE) --no-print-directory -C cpropep install
	@$(MAKE) --no-print-directory -C cpropep-2 install
	@$(MAKE) --no-print-directory -C cpropep-web install
	@$(MAKE) --no-print-directory -C gba install
	@$(MAKE) --no-print-directory -C grainanalysis install
	@$(MAKE) --no-print-directory -C lrd install
	@$(MAKE) --no-print-directory -C lrd-web install
	@$(MAKE) --no-print-directory -C mcp install
	@$(MAKE) --no-print-directory -C profiler install
	@$(MAKE) --no-print-directory -C prop install
	@$(MAKE) --no-print-directory -C rockflight install
