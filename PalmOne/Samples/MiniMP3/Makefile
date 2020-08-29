###############################################################
#
# Makefile for MiniMP3
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

CFLAGS = -O2 -g -o -d -Wall $(HS_PALM_RC_FLAGS)
LDFLAGS = -g -d

# Directory paths
OUTPUTDIR	  = Obj
SRCDIR		  = Src
RSCDIR		  = Rsc
TESTSDIR	  = Tests
RESULTDIR     = Result

APP = MiniMP3
RCPFILE=$(RSCDIR)/$(APP).rcp
PRCFILE=${RESULTDIR}/${APP}.prc

OBJS=$(OUTPUTDIR)/$(APP).o $(OUTPUTDIR)/Common.o $(OUTPUTDIR)/FileBrowserForm.o $(OUTPUTDIR)/id3.o $(OUTPUTDIR)/mp3.o


$(RESULTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp $(APP).def
	$(HUSH)$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUTDIR)/$(APP).code.1.sym
	build-prc $(APP).def $(OUTPUTDIR)/$(APP) -o ${PRCFILE}  $(OUTPUTDIR)/*.bin

$(OUTPUTDIR):
	mkdir ${OUTPUTDIR}
	
$(RESULTDIR):
	mkdir ${RESULTDIR}

$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/Main.c 
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $(SRCDIR)/Common.c -o $(OUTPUTDIR)/Common.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/FileBrowserForm.c -o $(OUTPUTDIR)/FileBrowserForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/id3.c -o $(OUTPUTDIR)/id3.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/mp3.c -o $(OUTPUTDIR)/mp3.o

#$(OBJS): $(SRCDIR)/MiniMP3.h
$(OBJS): $(SRCDIR)/*.h $(RSCDIR)/*.h

#$(OUTPUTDIR)/Main.o: $(SRCDIR)/MiniMP3.h 
$(OUTPUTDIR)/Main.o: $(SRCDIR)/*.h $(RSCDIR)/*.h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE); \
	pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RSCDIR)/FileBrowserForm.rcp)
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf ${RESULTDIR}
