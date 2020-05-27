###############################################################
#
# Makefile for HTTPLibTest
#
# Targets:
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

HS_PALM_RC_FLAGS =	$(HS_COMMON_INCS_DIR)

CFLAGS = -O2 -g -Wall $(HS_PALM_RC_FLAGS)
LDFLAGS = -g

# Directory paths
OUTPUTDIR	  = Obj
SRCDIR		  = Src
RSCDIR		  = Rsc
TESTSDIR	  = Tests
RESULTDIR     = Result

APP = HTTPLibTest
RCPFILE=$(RSCDIR)/$(APP).rcp
PRCFILE=$(RESULTDIR)/$(APP).prc

OBJSLIST =$(OUTPUTDIR)/$(APP).o \
		  $(OUTPUTDIR)/AppStdIO.o \
		  $(OUTPUTDIR)/httptest.o \
		  $(OUTPUTDIR)/peer.o

$(OUTPUTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OBJSLIST) $(OUTPUTDIR)/bin.stamp $(APP).def
	$(CC) $(CFLAGS) $(OBJSLIST) -o $(OUTPUTDIR)/temp.o 
# Add next line if generating symbols for debugging
	$(CC) $(CFLAGS) $(OBJSLIST) -o $(OUTPUTDIR)/$(APP).sym
	build-prc $(APP).def $(OUTPUTDIR)/temp.o -o $(RESULTDIR)/$(APP).prc  $(OUTPUTDIR)/*.bin
	
$(OUTPUTDIR):
	mkdir ${OUTPUTDIR}
	
$(RESULTDIR):
	mkdir ${RESULTDIR}

$(OUTPUTDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@


$(OUTPUTDIR)/$(APP).o: $(SRCDIR)/$(APP).h $(SRCDIR)/httptest.h $(SRCDIR)/AppStdIO.h
$(OUTPUTDIR)/peer.o :  $(SRCDIR)/$(APP).h $(SRCDIR)/peer.h 
$(OUTPUTDIR)/AppStdIO.o :  $(SRCDIR)/AppStdIO.h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf ${OUTPUTDIR}
	rm -rf ${RESULTDIR}


