###############################################################
#
# Makefile for ExtenedSound
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

APP = MyPlayer
RCPFILE=$(RSCDIR)/$(APP).rcp
PRCFILE=${RESULTDIR}/${APP}.prc

SRCS=$(SRCDIR)/Common.c $(SRCDIR)/PlayForm.c $(SRCDIR)/RecordForm.c $(SRCDIR)/Wave.c

OBJS=$(OUTPUTDIR)/$(APP).o $(OUTPUTDIR)/Common.o $(OUTPUTDIR)/PlayForm.o $(OUTPUTDIR)/RecordForm.o $(OUTPUTDIR)/Wave.o

$(OUTPUTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp $(APP).def
	build-prc $(APP).def $(OUTPUTDIR)/$(APP) -o $(RESULTDIR)/$(APP).prc  $(OUTPUTDIR)/*.bin

$(OUTPUTDIR):
	mkdir ${OUTPUTDIR}
	
$(RESULTDIR):
	mkdir ${RESULTDIR}

$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@
#	${CC} ${CFLAGS} -c ${SRCS} # Yvonne debug: it can replace 4 lines below.
	$(CC) $(CFLAGS) -c $(SRCDIR)/Common.c -o $(OUTPUTDIR)/Common.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/PlayForm.c -o $(OUTPUTDIR)/PlayForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/RecordForm.c -o $(OUTPUTDIR)/RecordForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/Wave.c -o $(OUTPUTDIR)/Wave.o


$(OBJS): $(SRCDIR)/MyPlayer.h  $(RSCDIR)/$(APP)Rsc.h

$(OUTPUTDIR)/$(APP).o: $(SRCDIR)/$(APP).h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf $(RESULTDIR)

