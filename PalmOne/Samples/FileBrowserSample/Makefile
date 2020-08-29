###############################################################
#
# Makefile for FileBroserSample
# 
# Targets:
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

APP = FileBrowserSample
RCPFILE=$(RSCDIR)/SampleApp.rcp
PRCFILE=$(RESULTDIR)/$(APP).prc

OBJS=$(OUTPUTDIR)/SampleMain.o $(OUTPUTDIR)/SampleDB.o $(OUTPUTDIR)/SampleTransfer.o


$(RESULTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp SampleCW9.def
	build-prc SampleCW9.def $(OUTPUTDIR)/$(APP) -o $(RESULTDIR)/$(APP).prc  $(OUTPUTDIR)/*.bin

$(OUTPUTDIR):
	mkdir ${OUTPUTDIR}
	
$(RESULTDIR):
	mkdir ${RESULTDIR}

$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/SampleMain.c 
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $(SRCDIR)/SampleDB.c -o $(OUTPUTDIR)/SampleDB.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/SampleTransfer.c -o $(OUTPUTDIR)/SampleTransfer.o

$(OBJS): $(SRCDIR)/SampleMain.h

$(OUTPUTDIR)/SampleMain.o: $(SRCDIR)/SampleMain.h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE); \
	pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RSCDIR)/SamplePrefer.rcp; \
	pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RSCDIR)/SampleMisc.rcp; \
	pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RSCDIR)/SampleList.rcp; \
	pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RSCDIR)/SampleEdit.rcp; \
	pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RSCDIR)/SampleDetails.rcp; \
	pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RSCDIR)/SampleDelete.rcp)
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf ${RESULTDIR}

