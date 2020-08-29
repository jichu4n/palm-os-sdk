###############################################################
#
# Makefile for Text2Hex
#
# Targets:
#		all		-	build and install  
#		clean		-	clean everything
#		samples		-	build sample code
#
###################################################################

# ====================================================
# Common Make variables for PalmOS executables
# ====================================================
# Tools we use
CC = m68k-palmos-gcc
AS = m68k-palmos-as

HS_COMMON_INCS_DIR = $(shell cat includes.txt) $(shell cat platform.txt) -I Rsc

HS_PALM_RC_FLAGS =	$(HS_COMMON_INCS_DIR)

CFLAGS = -O2 -g -Wall $(HS_PALM_RC_FLAGS)
LDFLAGS = -g

# Directory paths
OUTPUTDIR	  = Obj
SRCDIR		  = Src
RSCDIR		  = Rsc
TESTSDIR	  = Tests
RESULTDIR     = Result

APP = RadioSample
RCPFILE=$(RSCDIR)/$(APP)_Rsc.rcp
PRCFILE=$(RESULTDIR)/$(APP).prc

OBJS=$(OUTPUTDIR)/$(APP).o $(OUTPUTDIR)/Common.o $(OUTPUTDIR)/RadioSampleMain.o


$(OUTPUTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp $(APP).def
	build-prc $(APP).def $(OUTPUTDIR)/$(APP) -o $(RESULTDIR)/$(APP).prc  $(OUTPUTDIR)/*.bin

$(OUTPUTDIR):
	mkdir $(OUTPUTDIR)
	
$(RESULTDIR):
	mkdir $(RESULTDIR)

$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $(SRCDIR)/Common.c -o $(OUTPUTDIR)/Common.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/RadioSampleMain.c -o $(OUTPUTDIR)/RadioSampleMain.o

$(RSCDIR)/$(APP)_Rsc.h:
	( cd $(RSCDIR); pilrc -q -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))

$(OBJS): $(SRCDIR)/$(APP).h $(RSCDIR)/$(APP)_Rsc.h

$(OUTPUTDIR)/$(APP).o: $(SRCDIR)/$(APP).h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf $(RESULTDIR)
	rm -f $(RSCDIR)/$(APP)_Rsc.h
