###############################################################
#
# Targets:
#		all		-	build and install  
#		clean		-	clean everything
#		samples		- 	build sample code
#
###################################################################

# ====================================================
# Common Make variables for PalmOS executables
# ====================================================
# Tools we use
CC = m68k-palmos-gcc
AS = m68k-palmos-as

HS_COMMON_INCS_DIR = $(shell cat includes.txt) $(shell cat platform.txt)

HS_PALM_RC_FLAGS =	$(HS_COMMON_INCS_DIR) -I Rsc

CFLAGS = -O2 -g -Wall $(HS_PALM_RC_FLAGS)
LDFLAGS = -g

HUSH = @

# Directory paths
OUTPUTDIR	  = MLDeviceApp/Obj
SRCDIR		  = MLDeviceApp/Src
RSCDIR		  = MLDeviceApp/Rsc
TESTSDIR	  = MLDeviceApp/Tests
RESULTDIR     = MLDeviceApp/Result

all: 
	$(MAKE) -C MLDeviceApp

clean:
	rm -rf MLDeviceApp/Debug/*.prc
	rm -rf $(OUTPUTDIR)
	rm -rf $(RESULTDIR)