###############################################################
#
# Makefile for FotoFinish
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

HS_COMMON_INCS_DIR = $(shell cat includes.txt) $(shell cat platform.txt) -I Incs -I Rsc

HS_PALM_RC_FLAGS =	$(HS_COMMON_INCS_DIR)

CFLAGS = -O2 -g -Wall $(HS_PALM_RC_FLAGS)
LDFLAGS = -g

# Directory paths
OUTPUTDIR	  = Obj
SRCDIR		  = Src
RSCDIR		  = Rsc
HEADERDIR	  = Incs
RESULTDIR     = Result

APP = FotoFinish
RCPFILE=$(RSCDIR)/$(APP).rcp
PRCFILE=${RESULTDIR}/${APP}.prc

OBJS=$(OUTPUTDIR)/$(APP).o $(OUTPUTDIR)/AddTextForm.o $(OUTPUTDIR)/AlbumsOrganiseForm.o $(OUTPUTDIR)/DetailsForm.o $(OUTPUTDIR)/MainForm.o $(OUTPUTDIR)/SavePhotoForm.o $(OUTPUTDIR)/SaveUtils.o $(OUTPUTDIR)/utils.o


$(RESULTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp $(APP).def
	build-prc $(APP).def $(OUTPUTDIR)/$(APP) -o ${PRCFILE}  $(OUTPUTDIR)/*.bin

$(OUTPUTDIR):
	mkdir ${OUTPUTDIR}
	
$(RESULTDIR):
	mkdir ${RESULTDIR}

$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $(SRCDIR)/AddTextForm.c -o $(OUTPUTDIR)/AddTextForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/AlbumsOrganiseForm.c -o $(OUTPUTDIR)/AlbumsOrganiseForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/DetailsForm.c -o $(OUTPUTDIR)/DetailsForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/MainForm.c -o $(OUTPUTDIR)/MainForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/SavePhotoForm.c -o $(OUTPUTDIR)/SavePhotoForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/SaveUtils.c -o $(OUTPUTDIR)/SaveUtils.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/utils.c -o $(OUTPUTDIR)/utils.o

$(OBJS): ${HEADERDIR}/${APP}.h $(RSCDIR)/$(APP)Rsc.h

$(OUTPUTDIR)/$(APP).o: ${HEADERDIR}/${APP}.h

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}
	
clean:
	rm -rf $(OUTPUTDIR)
	rm -rf $(RESULTDIR)
