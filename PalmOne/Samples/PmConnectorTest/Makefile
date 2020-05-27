#CW_REG_VALUE = $(shell reg query "HKLM\\SOFTWARE\\Metrowerks\\CodeWarrior\\Product Versions\\Palm OS 9.0" /v PATH | grep PATH)
#CW_PATH_WS = $(strip $(subst PATH,, $(subst REG_SZ,, $(CW_REG_VALUE))))
#CW_PATH = $(subst Program Files,PROGRA~1, $(CW_PATH_WS))

#sample:
	#$(HUSH)$(CW_PATH)/Bin/CmdIDE.exe PmConnectorTest.mcp /r /b /c /q /v y
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

HS_COMMON_INCS_DIR = $(shell cat includes.txt) $(shell cat platform.txt) -I Rsc -I Src

HS_PALM_RC_FLAGS =	$(HS_COMMON_INCS_DIR)

CFLAGS = -O2 -g -Wall $(HS_PALM_RC_FLAGS)
LDFLAGS = -g

# Directory paths
OUTPUTDIR	  = Obj/
SRCDIR		  = Src/
RSCDIR		  = Rsc/
TESTSDIR	  = Tests/
RESULTDIR     = Result/

APP = PmConnectorTest
UTIL = PmUtil
LOG  = LogUtil

RCPFILE=$(RSCDIR)$(APP).rcp

OBJS=$(OUTPUTDIR)$(APP).o $(OUTPUTDIR)$(UTIL).o $(OUTPUTDIR)$(LOG).o


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

$(RSCDIR)$(APP)_Rsc.h:
	( cd $(RSCDIR); pilrc -q -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))

$(OBJS): $(SRCDIR)$(APP).h $(RSCDIR)$(APP)_Rsc.h

$(OUTPUTDIR)$(APP).o: $(SRCDIR)$(APP).h 

$(OUTPUTDIR)bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE))
	touch $(OUTPUTDIR)/bin.stamp

all: $(OUTPUTDIR)$(APP).prc 

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf $(RESULTDIR)
	rm -f $(RSCDIR)$(APP)_Rsc.h
