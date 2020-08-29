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
OUTPUTDIR	  = Obj
SRCDIR		  = Src
RSCDIR		  = Rsc
TESTSDIR	  = Tests
RESULTDIR     = Result

APP = GSMStatus
RCPFILE=$(RSCDIR)/$(APP)_Rsc.rcp
PRCFILE=$(RESULTDIR)/$(APP).prc

OBJS=$(OUTPUTDIR)/$(APP).o $(OUTPUTDIR)/Common.o $(OUTPUTDIR)/MainInit.o

all: $(OUTPUTDIR)/$(APP).prc

$(OUTPUTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp $(APP).def
	$(HUSH)$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUTDIR)/$(APP).code.1.sym
	build-prc $(APP).def $(OUTPUTDIR)/$(APP) -o $(PRCFILE) $(OUTPUTDIR)/*.bin

$(RSCDIR)/$(APP).prc: $(RSCDIR)/$(APP).xrd PalmRC $(RSCDIR)/$(APP).xrd -o $(RSCDIR)/$(APP).prc

$(OUTPUTDIR):
	mkdir $(OUTPUTDIR)
	
$(RESULTDIR):
	mkdir $(RESULTDIR)

$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/Common.c -o $(OUTPUTDIR)/Common.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/MainInit.c -o $(OUTPUTDIR)/MainInit.o
	$(CC) $(CFLAGS) -c $< -o $@

$(RSCDIR)/$(APP)_Rsc.h:
	( cd $(RSCDIR); pilrc -q -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))

$(OBJS): $(SRCDIR)/$(APP).h $(RSCDIR)/$(APP)_Rsc.h

$(OUTPUTDIR)/$(APP).o: $(SRCDIR)/$(APP).h

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -q -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	$(HUSH)touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf $(RESULTDIR)
	rm -f $(RSCDIR)/$(APP)_Rsc.h
