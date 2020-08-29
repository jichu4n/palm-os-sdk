###############################################################
#
# Makefile for BluetoothMgr
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

APP = BluetoothMgr
RCPFILE=$(RSCDIR)/$(APP).rcp
PRCFILE=${RESULTDIR}/${APP}.prc

OBJS=$(OUTPUTDIR)/$(APP).o $(OUTPUTDIR)/LogUtil.o $(OUTPUTDIR)/BtUtil.o


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
	$(CC) $(CFLAGS) -c $(SRCDIR)/LogUtil.c -o $(OUTPUTDIR)/LogUtil.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/BtUtil.c -o $(OUTPUTDIR)/BtUtil.o

$(OBJS): $(SRCDIR)/*.h

$(OUTPUTDIR)/Main.o: $(SRCDIR)/*.h $(RSCDIR)/*.h 

$(OUTPUTDIR)/bin.stamp: $(RCPFILE) $(OBJS)
	( cd $(OUTPUTDIR); rm -f *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE); )
	touch $(OUTPUTDIR)/bin.stamp

samples: ${PRCFILE}

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf ${RESULTDIR}
