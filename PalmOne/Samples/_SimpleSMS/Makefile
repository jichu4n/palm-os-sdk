###############################################################
#
# Makefile for SimpleSMS
#
# Targets:
#		clean		-	clean everything
#		samples		- 	build sample code
#
# This makefile uses the following environment variables:
#
#   PALMTOOLSU - path to directory where palm tools should go.
#       ex: "/usr/local/PalmTools"
#
#	
# Conventions:
#	 "ALLUPPER"  case variable names are generally exported or provided by
#				        the environment
#	 "initLower" case variables are local to the makefile
#
#
# Commonly used  Automatic Variables:
#	  $< represents the first dependency
#	  $^ represents all dependencies
#	  $? represents all dependencies that are newer than the target
#	  $@ represents the target 
#
#
###################################################################


# ====================================================
# Common Make variables for PalmOS executables
# ====================================================
# Tools we use
CC = m68k-palmos-gcc
AS = m68k-palmos-as


# Directory paths
OUTPUTDIR	  = Obj
SRCDIR		  = Src
RSCDIR		  = Rsc
TESTSDIR	  = Tests
RESULTDIR     = Result

HS_COMMON_INCS_DIR = $(shell cat includes.txt) $(shell cat platform.txt)

HS_PALM_RC_FLAGS =	$(HS_COMMON_INCS_DIR)

CFLAGS = -O2 -g -Wall $(HS_PALM_RC_FLAGS) 
LDFLAGS = -g


APP = SimpleSMS
RCPFILE=$(RSCDIR)/$(APP).rcp
PRCFILE=${RESULTDIR}/${APP}.prc

OBJS=$(OUTPUTDIR)/$(APP).o
SYMS=$(OUTPUTDIR)/$(APP).sym

$(OUTPUTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp $(APP).def
	build-prc $(APP).def $(OUTPUTDIR)/$(APP) -o $(RESULTDIR)/$(APP).prc  $(OUTPUTDIR)/*.bin
	
$(OUTPUTDIR):
	mkdir ${OUTPUTDIR}
	
$(RESULTDIR):
	mkdir ${RESULTDIR}
	
$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
# Add next line if generating symbols for debugging
	$(CC) $(CFLAGS) $(OBJS) -o $(SYMS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(SRCDIR)/$(APP).h

$(OUTPUTDIR)/$(APP).o: $(SRCDIR)/$(APP).h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -q -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf ${RESULTDIR}
