#CW_REG_VALUE = $(shell reg query "HKLM\\SOFTWARE\\Metrowerks\\CodeWarrior\\Product Versions\\Palm OS 9.0" /v PATH | grep PATH)
#CW_PATH_WS = $(strip $(subst PATH,, $(subst REG_SZ,, $(CW_REG_VALUE))))
#CW_PATH = $(subst Program Files,PROGRA~1, $(CW_PATH_WS))

#sample:
	#$(HUSH)$(CW_PATH)/Bin/CmdIDE.exe FotoFinishCW9.mcp /r /b /c /q /v y
	#$(HUSH)sleep 2

###############################################################
#
# Makefile for Text2Hex
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

HS_COMMON_INCS_DIR = $(shell cat includes.txt) $(shell cat platform.txt) -I Incs -I Rsc

HS_PALM_RC_FLAGS =	$(HS_COMMON_INCS_DIR)

CFLAGS = -O2 -g -Wall $(HS_PALM_RC_FLAGS)
LDFLAGS = -g

# Directory paths
OUTPUTDIR	  = Obj/
SRCDIR		  = Src/
RSCDIR		  = Rsc/
TESTSDIR	  = Tests/
RESULTDIR     = Result/

APP = FotoFinishV2
RCPFILE=$(RSCDIR)$(APP).rcp

OBJS=$(OUTPUTDIR)$(APP).o $(OUTPUTDIR)AddTextForm.o $(OUTPUTDIR)AlbumsOrganiseForm.o $(OUTPUTDIR)DetailsForm.o $(OUTPUTDIR)MainForm.o $(OUTPUTDIR)SavePhotoForm.o $(OUTPUTDIR)SaveUtils.o $(OUTPUTDIR)utils.o


$(OUTPUTDIR)$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)$(APP) $(OUTPUTDIR)bin.stamp $(APP).def
	build-prc $(APP).def $(OUTPUTDIR)$(APP) -o $(RESULTDIR)$(APP).prc  $(OUTPUTDIR)*.bin

$(OUTPUTDIR):
	mkdir Obj
	
$(RESULTDIR):
	mkdir Result

$(OUTPUTDIR)$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)%.o: $(SRCDIR)%.c 
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $(SRCDIR)AddTextForm.c -o $(OUTPUTDIR)AddTextForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)AlbumsOrganiseForm.c -o $(OUTPUTDIR)AlbumsOrganiseForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)DetailsForm.c -o $(OUTPUTDIR)DetailsForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)MainForm.c -o $(OUTPUTDIR)MainForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)SavePhotoForm.c -o $(OUTPUTDIR)SavePhotoForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)SaveUtils.c -o $(OUTPUTDIR)SaveUtils.o
	$(CC) $(CFLAGS) -c $(SRCDIR)utils.c -o $(OUTPUTDIR)utils.o

$(OBJS): Incs/$(APP).h

$(OUTPUTDIR)/$(APP).o: Incs/$(APP).h 

$(OUTPUTDIR)bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	touch $(OUTPUTDIR)/bin.stamp

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf ${RESULTDIR}
	rm -f $(RSCDIR)/$(APP)_Rsc.h
