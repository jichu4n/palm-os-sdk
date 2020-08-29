###############################################################
#
# Makefile for MLDeviceApp
#
# Targets:
#		all			-	build and install  
#		clean		-	clean everything
#		check		-	run self-tests
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
SRCDIR		  = src
RSCDIR		  = rsc
TESTSDIR	  = Tests
RESULTDIR     = Result

APP = MusicLib
RCPFILE=$(RSCDIR)/AppResources.rcp
PRCFILE=${RESULTDIR}/${APP}.prc

OBJS=$(OUTPUTDIR)/AppMain.o $(OUTPUTDIR)/AppUtilities.o $(OUTPUTDIR)/DbUtilities.o $(OUTPUTDIR)/MusicLibUtilities.o $(OUTPUTDIR)/TableUtilities.o


$(RESULTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp $(APP).def
	$(HUSH)$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUTDIR)/$(APP).code.1.sym
	build-prc $(APP).def $(OUTPUTDIR)/$(APP) -o ${PRCFILE}  $(OUTPUTDIR)/*.bin

$(OUTPUTDIR):
	mkdir ${OUTPUTDIR}
	
$(RESULTDIR):
	mkdir ${RESULTDIR}

$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/AppMain.c 
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $(SRCDIR)/AppUtilities.c -o $(OUTPUTDIR)/AppUtilities.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/DbUtilities.c -o $(OUTPUTDIR)/DbUtilities.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/MusicLibUtilities.c -o $(OUTPUTDIR)/MusicLibUtilities.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/TableUtilities.c -o $(OUTPUTDIR)/TableUtilities.o

$(OBJS): $(SRCDIR)/*.h $(RSCDIR)/*.h

$(OUTPUTDIR)/AppMain.o: $(SRCDIR)/*.h $(RSCDIR)/*.h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf ${RESULTDIR}
