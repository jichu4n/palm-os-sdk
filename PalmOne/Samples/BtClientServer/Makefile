#CW_REG_VALUE = $(shell reg query "HKLM\\SOFTWARE\\Metrowerks\\CodeWarrior\\Product Versions\\Palm OS 9.0" /v PATH | grep PATH)
#CW_PATH_WS = $(strip $(subst PATH,, $(subst REG_SZ,, $(CW_REG_VALUE))))
#CW_PATH = $(subst Program Files,PROGRA~1, $(CW_PATH_WS))

#sample:
	#$(HUSH)$(CW_PATH)/Bin/CmdIDE.exe MiniADPCM.mcp /r /b /c /q /v y
	#$(HUSH)sleep 2

###############################################################
#
# Makefile for BtClientServer
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
SRCDIR		  = Src
RSCDIR		  = Rsc
TESTSDIR	  = Tests
RESULTDIR     = Result

APP = BtApp
RCPFILE=$(RSCDIR)/$(APP).rcp
PRCFILE=${RESULTDIR}/${APP}.prc

OBJS=$(OUTPUTDIR)/$(APP).o $(OUTPUTDIR)/Common.o $(OUTPUTDIR)/FileBrowserForm.o $(OUTPUTDIR)/LogUtil.o $(OUTPUTDIR)/SrmUtil.o


$(RESULTDIR)/$(APP).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)/$(APP) $(OUTPUTDIR)/bin.stamp BtClientServer.def
	$(HUSH)$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUTDIR)/$(APP).code.1.sym
	build-prc BtClientServer.def $(OUTPUTDIR)/$(APP) -o ${PRCFILE}  $(OUTPUTDIR)/*.bin

$(OUTPUTDIR):
	mkdir ${OUTPUTDIR}
	
$(RESULTDIR):
	mkdir ${RESULTDIR}

$(OUTPUTDIR)/$(APP): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OUTPUTDIR)/%.o: $(SRCDIR)/BtApp.c 
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $(SRCDIR)/Common.c -o $(OUTPUTDIR)/Common.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/FileBrowserForm.c -o $(OUTPUTDIR)/FileBrowserForm.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/LogUtil.c -o $(OUTPUTDIR)/LogUtil.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/SrmUtil.c -o $(OUTPUTDIR)/SrmUtil.o

$(OBJS): $(SRCDIR)/*.h $(RSCDIR)/*.h

$(OUTPUTDIR)/Main.o: $(SRCDIR)/*.h $(RSCDIR)/*.h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE); \
	pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RSCDIR)/FileBrowserForm.rcp)
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf ${RESULTDIR}
