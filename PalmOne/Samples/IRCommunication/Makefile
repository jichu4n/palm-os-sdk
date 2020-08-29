###############################################################
#
# Makefile for IRCommunication
#
# Targets:
#		all		-	build and install  
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

HS_COMMON_INCS_DIR = $(shell cat includes.txt) $(shell cat platform.txt) -I Rsc -I Src -I Incs

HS_PALM_RC_FLAGS =	$(HS_COMMON_INCS_DIR)

CFLAGS = -O2 -g -Wall $(HS_PALM_RC_FLAGS)
LDFLAGS = -g

# Directory paths
OUTPUTDIR	  = Obj/
SRCDIR		  = Src/
RSCDIR		  = Rsc/
TESTSDIR	  = Tests/
RESULTDIR     = Result/

APP1 = IRReceiverRaw
APPID1 = IRRR
APP2 = IRTransmitterRaw
APPID2 = IRTR
APP3 = IRUnframer
APPID3 = IRUF
APP4 = IRFramer
APPID4 = IRFR
APP5 = IRFramerZire71
APPID5 = IRFZ
APP6 = IRCOMMTransmitter
APPID6 = IRCT
APP7 = IRCOMMReceiver
APPID7 = IRCR

RCPFILE1=$(RSCDIR)$(APP1).rcp
RCPFILE2=$(RSCDIR)$(APP2).rcp
RCPFILE3=$(RSCDIR)$(APP3).rcp
RCPFILE4=$(RSCDIR)$(APP4).rcp
RCPFILE5=$(RSCDIR)$(APP5).rcp
RCPFILE6=$(RSCDIR)$(APP6).rcp
RCPFILE7=$(RSCDIR)$(APP7).rcp

OBJS1=$(OUTPUTDIR)$(APP1).o $(OUTPUTDIR)Common.o
OBJS2=$(OUTPUTDIR)$(APP2).o $(OUTPUTDIR)Common.o
OBJS3=$(OUTPUTDIR)$(APP3).o $(OUTPUTDIR)Common.o
OBJS4=$(OUTPUTDIR)$(APP4).o $(OUTPUTDIR)Common.o
OBJS5=$(OUTPUTDIR)$(APP5).o $(OUTPUTDIR)Common.o
OBJS6=$(OUTPUTDIR)$(APP6).o $(OUTPUTDIR)Common.o
OBJS7=$(OUTPUTDIR)$(APP7).o $(OUTPUTDIR)Common.o

all: $(OUTPUTDIR)$(APP1).prc $(OUTPUTDIR)$(APP2).prc $(OUTPUTDIR)$(APP3).prc $(OUTPUTDIR)$(APP4).prc $(OUTPUTDIR)$(APP5).prc $(OUTPUTDIR)$(APP6).prc $(OUTPUTDIR)$(APP7).prc

$(OUTPUTDIR):
	mkdir Obj
	
$(RESULTDIR):
	mkdir Result

$(OUTPUTDIR)%.o: $(SRCDIR)%.c 
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -c $(SRCDIR)Common.c -o $(OUTPUTDIR)Common.o
	$(CC) $(CFLAGS) -c $(SRCDIR)$(APP1).c  -o $(OUTPUTDIR)$(APP1).o 					
	$(CC) $(CFLAGS) -c $(SRCDIR)$(APP2).c  -o $(OUTPUTDIR)$(APP2).o 
	$(CC) $(CFLAGS) -c $(SRCDIR)$(APP3).c  -o $(OUTPUTDIR)$(APP3).o 
	$(CC) $(CFLAGS) -c $(SRCDIR)$(APP4).c  -o $(OUTPUTDIR)$(APP4).o 
	$(CC) $(CFLAGS) -c $(SRCDIR)$(APP5).c  -o $(OUTPUTDIR)$(APP5).o 
	$(CC) $(CFLAGS) -c $(SRCDIR)$(APP6).c  -o $(OUTPUTDIR)$(APP6).o 
	$(CC) $(CFLAGS) -c $(SRCDIR)$(APP7).c  -o $(OUTPUTDIR)$(APP7).o 					

$(OUTPUTDIR)$(APP1).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)$(APP1) $(OUTPUTDIR)$(APP1).stamp 
	build-prc -o $(RESULTDIR)$(APP1).prc -n $(APP1) -t appl -c $(IRRR) -n $(OUTPUTDIR)$(APP1) $(OUTPUTDIR)*.bin
$(OUTPUTDIR)$(APP1): $(OBJS1) $(RCPFILE1)
	$(CC) $(CFLAGS) -o $@ $(OBJS1)		
$(OUTPUTDIR)$(APP1).stamp: $(OBJS1) $(RCPFILE1)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE1))	
	touch $(OUTPUTDIR)$(APP1).stamp								

$(OUTPUTDIR)$(APP2).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)$(APP2) $(OUTPUTDIR)$(APP2).stamp 
	build-prc -o $(RESULTDIR)$(APP2).prc -n $(APP2) -t appl -c $(IRTR) -n $(OUTPUTDIR)$(APP2) $(OUTPUTDIR)*.bin
$(OUTPUTDIR)$(APP2): $(OBJS2) $(RCPFILE2)
	$(CC) $(CFLAGS) -o $@ $(OBJS2)		
$(OUTPUTDIR)$(APP2).stamp: $(OBJS2) $(RCPFILE2)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE2))	
	touch $(OUTPUTDIR)$(APP2).stamp								

$(OUTPUTDIR)$(APP3).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)$(APP3) $(OUTPUTDIR)$(APP3).stamp 
	build-prc -o $(RESULTDIR)$(APP3).prc -n $(APP3) -t appl -c $(IRUR) -n $(OUTPUTDIR)$(APP3) $(OUTPUTDIR)*.bin
$(OUTPUTDIR)$(APP3): $(OBJS3) $(RCPFILE3)
	$(CC) $(CFLAGS) -o $@ $(OBJS3)		
$(OUTPUTDIR)$(APP3).stamp: $(OBJS3) $(RCPFILE3)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE3))	
	touch $(OUTPUTDIR)$(APP3).stamp								

$(OUTPUTDIR)$(APP4).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)$(APP4) $(OUTPUTDIR)$(APP4).stamp 
	build-prc -o $(RESULTDIR)$(APP4).prc -n $(APP4) -t appl -c $(IRFR) -n $(OUTPUTDIR)$(APP4) $(OUTPUTDIR)*.bin
$(OUTPUTDIR)$(APP4): $(OBJS4) $(RCPFILE4)
	$(CC) $(CFLAGS) -o $@ $(OBJS4)		
$(OUTPUTDIR)$(APP4).stamp: $(OBJS4) $(RCPFILE4)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE4))	
	touch $(OUTPUTDIR)$(APP4).stamp								

$(OUTPUTDIR)$(APP5).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)$(APP5) $(OUTPUTDIR)$(APP5).stamp 
	build-prc -o $(RESULTDIR)$(APP5).prc -n $(APP5) -t appl -c $(IRFZ) -n $(OUTPUTDIR)$(APP5) $(OUTPUTDIR)*.bin
$(OUTPUTDIR)$(APP5): $(OBJS5) $(RCPFILE5)
	$(CC) $(CFLAGS) -o $@ $(OBJS5)		
$(OUTPUTDIR)$(APP5).stamp: $(OBJS5) $(RCPFILE5)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE5))	
	touch $(OUTPUTDIR)$(APP5).stamp								

$(OUTPUTDIR)$(APP6).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)$(APP6) $(OUTPUTDIR)$(APP6).stamp 
	build-prc -o $(RESULTDIR)$(APP6).prc -n $(APP6) -t appl -c $(IRCT) -n $(OUTPUTDIR)$(APP6) $(OUTPUTDIR)*.bin
$(OUTPUTDIR)$(APP6): $(OBJS6) $(RCPFILE6)
	$(CC) $(CFLAGS) -o $@ $(OBJS6)		
$(OUTPUTDIR)$(APP6).stamp: $(OBJS6) $(RCPFILE6)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE6))	
	touch $(OUTPUTDIR)$(APP6).stamp								

$(OUTPUTDIR)$(APP7).prc: $(OUTPUTDIR) $(RESULTDIR) $(OUTPUTDIR)$(APP7) $(OUTPUTDIR)$(APP7).stamp 
	build-prc -o $(RESULTDIR)$(APP7).prc -n $(APP7) -t appl -c $(IRCR) -n $(OUTPUTDIR)$(APP7) $(OUTPUTDIR)*.bin
$(OUTPUTDIR)$(APP7): $(OBJS7) $(RCPFILE7)
	$(CC) $(CFLAGS) -o $@ $(OBJS7)		
$(OUTPUTDIR)$(APP7).stamp: $(OBJS7) $(RCPFILE7)
	( cd $(OUTPUTDIR); rm *.bin; pilrc -I ../$(SRCDIR) -I ../$(RSCDIR) ../$(RCPFILE7))	
	touch $(OUTPUTDIR)$(APP7).stamp								

clean:
	rm -rf $(OUTPUTDIR)
	rm -rf $(RESULTDIR)
	
	
