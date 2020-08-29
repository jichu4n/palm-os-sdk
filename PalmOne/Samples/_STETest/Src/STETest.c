/******************************************************************************
 *
 * Copyright (c) 2001 Handspring, Inc.
 * All rights reserved.
 *
 * File: STETest.c
 *
 *****************************************************************************/
#include <PalmOS.h>
#include <StdIOPalm.h>
#include <HsCreators.h>
#include <HsNav.h>
#include <HsExt.h>
#include <SmartTextEngine.h>
#include "STETest.h"

static Boolean	gSTELoaded;
static UInt16	gSTERefNum;
static UInt16	gEngineRefNum;
//static UInt16	gEngineBRefNum;

static FontID currentFont;

#define appFileCreator			   'HSTE'
#define appVersionNum			   0x01


// Define the minimum OS version we support (3.1 for now).
#define ourMinVersion	sysMakeROMVersion(3,1,0,sysROMStageRelease,0)


/***********************************************************************
 *
 * FUNCTION:	RomVersionCompatible
 *
 * DESCRIPTION: This routine checks that a ROM version is meet your
 *				minimum requirement.
 *
 * PARAMETERS:	requiredVersion - minimum rom version required
 *								  (see sysFtrNumROMVersion in SystemMgr.h 
 *								  for format)
 *				launchFlags 	- flags that indicate if the application 
 *								  UI is initialized.
 *
 * RETURNED:	error code or zero if rom is compatible
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
	UInt32 romVersion;

	// See if we're on in minimum required version of the ROM or later.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
		{
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
			{
			FrmAlert (RomIncompatibleAlert);
		
			// Palm OS 1.0 will continuously relaunch this app unless we switch to 
			// another safe one.
			if (romVersion < ourMinVersion)
				{
				AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
				}
			}
		
		return sysErrRomIncompatible;
		}

	return errNone;
}



static Err
LoadSmartTextEngine(UInt16* refNumP, Boolean* libLoadedP)
{
  Err error = 0;

  // Routine is pointless without this parameter
  if (!refNumP)
	return memErrInvalidParam;

  // always load the library, so we have our own instance of the engine
  error = SysLibLoad(sysFileTLibrary, hsFileCSmartTextEngine, refNumP);
  if (error)
	return error;

  *libLoadedP = true;

  return error;
}


static void TestToggleFont ();
static void
TestToggleFont ()
{
  FontID		  useFont;

  useFont = HsTwoFontSelect (currentFont);
  if (useFont == currentFont)
	return;

  currentFont = useFont;
  if (currentFont == largeFont)
	STESetFlags(gSTERefNum, gEngineRefNum, steLargeFont, 0);
  else
	STESetFlags(gSTERefNum, gEngineRefNum, 0, steLargeFont);

  STERerenderList(gSTERefNum, gEngineRefNum);
}




static void HyperlinkCallback(UInt32 refNum)
{
  if (refNum == 1)
	SndPlaySystemSound(sndError);
  else if (refNum == 2)
	SndPlaySystemSound(sndStartUp);
}





static Boolean MainFormHandleEvent(EventPtr eventP)
{
  Boolean handled = false;
  RectangleType r;
  FormPtr formP;
  UInt16 cardNo;
  LocalID dbID;
  DmSearchStateType searchState;
  UInt32 flags;
  UInt32 err;

  DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appFileCreator, true, &cardNo, &dbID);

  handled = STEHandleEvent(gSTERefNum, gEngineRefNum, eventP);
  
  if (!handled)
	switch (eventP->eType) 
		{
		case menuEvent:
		  switch (eventP->data.menu.itemID)
			{
			  case MainOptionsAboutSTETest:
				  MenuEraseStatus (0);
				  HsAboutHandspringApp(cardNo, dbID, "2002", "");
				  handled = true;
				  break;
			  case MainEditCopy:
				  STECopySelectionToClipboard(gSTERefNum, gEngineRefNum);
				  handled = true;
				  break;
			  case MainEditSelect510:
				  STESetCurrentTextSelection(gSTERefNum, gEngineRefNum, 1, 5, 5);
				  handled = true;
				  break;
			  case MainEditSelect10100:
				  STESetCurrentTextSelection(gSTERefNum, gEngineRefNum, 1, 10, 90);
				  handled = true;
				  break;
			  case MainEditSelect50100:
				  STESetCurrentTextSelection(gSTERefNum, gEngineRefNum, 1, 50, 50);
				  handled = true;
				  break;
			  case MainEditSelectAll:
				  STESetCurrentTextSelection(gSTERefNum, gEngineRefNum, 1, 0, 0xFFFFFFFF);
				  handled = true;
				  break;
			  case MainEditToggleFont:
				  TestToggleFont();
				  handled = true;
			}
		  break;

		case frmOpenEvent:

			formP = FrmGetActiveForm();
			FrmDrawForm (formP);

			currentFont = stdFont;

			// Initialize the smart text engine
			flags = (steParsePhoneNumbers | steParseURLs | steParseEmail | steParseEmoticons |
					 steParseMoreEmoticons | steScrollWalksLinks);
			RctSetRectangle(&r,0, 30, 152, 120);   // the bounds of the list
			err = STEInitializeEngine(gSTERefNum, &gEngineRefNum, &r, MainFormScrollBar, flags,
									kSTEGreen /* phone numbers */, kSTEBlue /* urls */, kSTEPurple /* email */);
			
			if (err) break;

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Date: //STESTDFONT////STERIGHTALIGN=30//8/2/02 5:30 pm", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//To: //STESTDFONT////STERIGHTALIGN=30////STELEFTINDENT=30//Testing, John Doe, Sherwin Stoole", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Cc: //STESTDFONT////STERIGHTALIGN=30////STELEFTINDENT=30//Robert Tester, Bob Fuller", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//From: //STESTDFONT////STERIGHTALIGN=30////STELEFTINDENT=30//Testing Again", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Subj: //STESTDFONT////STERIGHTALIGN=30////STELEFTINDENT=30//Testing the STE", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=1000//   //STERIGHTALIGN=30//Document.txt (10k)", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=1000//   //STERIGHTALIGN=30//OtherStuff.html (32k)", false);
		   
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "One of the available system sounds is //STEHYPERLINK=sndError\x01 1//.  Does it work?", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "And the other sound is //STEHYPERLINK=sndStartUp\x01 2//.  Pretty cool, eh?", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STECURRENTFONT//This is the current font color.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBLACKFONT//This is the black font color.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "go check <http://www.cnn.com>", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "go check <<http://www.cnn.com>>", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "go check \"http://www.cnn.com\"", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "go check \'http://www.cnn.com\'", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "go check \"http://www.cnn.com\'", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "<mailto:superfun@super.com>, dfdfdf", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Call \"650 123-4567\"", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Call \'650 321-4576\'", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This is a test.\nThis is another test.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This is a test.//STELINEBREAK//This is another test.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This is a test. This is a test of the //STEREDFONT//emergency//STECURRENTFONT// broadcast system. This is only a test. If this was a real emergency, we would all be dead!", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "www.ebay.com is cool.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "my email: superfun@super.com.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This is another test.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This is a word wrapping test.  But does it work?", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This sentence    has a big space between     some words.", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Name://STESTDFONT// Rick", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEREDFONT//red", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBLUEFONT//blue", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEYELLOWFONT//yellow", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEGREENFONT//green", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEPURPLEFONT//purple", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEORANGEFONT//orange", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEGRAYFONT//gray", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBLACKFONT//black", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//ebay.com//STESTDFONT// ebay.com", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT////STESTDFONT////STEBOLDFONT//ebay.com//STESTDFONT////STEGREENFONT// ebay.com GREEN!!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//ebay.com ebay.com", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "e//STEBOLDFONT//bay.c//STESTDFONT//om", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Name://STESTDFONT// Rick", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEREDFONT//R//STEORANGEFONT//a//STEYELLOWFONT//i//STEGREENFONT//n//STEBLUEFONT//bo//STEPURPLEFONT//w", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//This is a really long line of text that is also in bold font... so this should wrap around a few times, ok?", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "The coolest thing is that we can display text in //STEREDFONT//red, //STEBLUEFONT//blue, //STEORANGEFONT//orange, //STEPURPLEFONT//purple, //STEGREENFONT//green, //STEYELLOWFONT//yellow, //STEBLACKFONT//and black colors. //STEBOLDFONT////STEREDFONT//VERY COOL!!!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This engine can display //STEBOLDFONT//bold text//STESTDFONT//. We will also see links displayed as www.yahoo.com. And an email address appears as rick@yahoo.com. You can call me at 123 123-1234 or 345-343-3456.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT////STEPURPLEFONT//This is a really long line //STEREDFONT//of text that is also in //STEGREENFONT//bold font and also has //STEBLUEFONT//many different colors... //STEYELLOWFONT//so this should wrap //STEORANGEFONT//around a few times, ok?", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//my BOLD email address is superfun@super.com.", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Call Joe at 408 555-1212 or 650 555-1234.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Call Jimbob today at 408 555-1212 but watch out!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Call Rick at 123 123 4567 or 343 343-4567 or try him at work with 123 456-7898.  Later!!.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "email: superfun@super.com", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "my email address is superfun@super.com.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Hi!!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "ebay.com is cool.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "OK... now try //STEGREENFONT//green followed by www.abcdefghijklmnopqrstuvwxyz.com... is that cool???", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Don\'t forget about //STEHYPERLINK=sndError\x01 1// and //STEHYPERLINK=sndStartUp\x01 2//.  OR ELSE!!!!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Don\'t forget about //STEHYPERLINK=sndError\x01 1// and //STEHYPERLINK=sndStartUp\x01 2//.  OR ELSE!!!!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEPURPLEFONT//Don\'t forget about //STEHYPERLINK=sndError\x01 1// and //STEHYPERLINK=sndStartUp\x01 2//.  OR ELSE!!!!", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEHORIZONTALLINE////STECENTERALIGN//Pong", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEHORIZONTALLINE//", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This is a test. This is a test of the //STEREDFONT//emergency//STECURRENTFONT// broadcast system. This is only a test. If this was a real emergency, we would all be dead!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Hey, after work, go get some beer and pizza.  Then we can go watch the game at my place.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This is a test. This is a test of the emergency broadcast system. This is only a test. If this was a real emergency, we would all be dead!", false);


			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Dino 1:", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=9000//", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=9001//", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=9002//", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=9003//", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=9004//", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEHORIZONTALLINE//", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Dino 2:\n//STEBITMAP=9000//\n//STEBITMAP=9001//\n//STEBITMAP=9002//\n//STEBITMAP=9003//\n//STEBITMAP=9004//", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEHORIZONTALLINE//", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Dino 3:\n//STEBITMAP=9000////STEBITMAP=9001////STEBITMAP=9002////STEBITMAP=9003////STEBITMAP=9004//", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEHORIZONTALLINE//", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Dino 4://STEBITMAP=9000////STEBITMAP=9001////STEBITMAP=9002////STEBITMAP=9003////STEBITMAP=9004//", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ">:)", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":>:)", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":)", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":(:)", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ">:):)>:):(:):(>:):(:):(:)>:)>:):(:):(>:):)", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(:):(", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "yeah..is", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "www.cnn..com", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "www.cnn..", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "http://..www.cnn.com", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "215.53.5.147 <-- still a bug", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEHORIZONTALLINE//", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "<http://www.averylongurl.somethingelse.com/thisisdoingeijjeoijefei/eieijeirerererere/werijgoejgoejgoergj/ergtoijregoejrgoerjgoerjgoergjergoij/index.html>", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This will have some text at the beginning before we display the URL.\n\n<http://www.averylongurl.somethingelse.com/thisisdoingeijjeoijefei/eieijeirerererere/werijgoejgoejgoergj/ergtoijregoejrgoerjgoerjgoergjergoij/index.html>", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "This will have some text at the beginning before we display the URL.\n\nhttp://www.averylongurl.somethingelse.com/thisisdoingeijjeoijefei/eieijeirerererere/werijgoejgoejgoergj/ergtoijregoejrgoerjgoerjgoergjergoij/index.html", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "What do I have to do to get this long url thing working?  Does it have to be www.averylongurl.somethingelse.com/thisisdoingeijjeoijefei/eieijeirerererere/werijgoejgoejgoergj/ergtoijregoejrgoerjgoerjgoergjergoij/index.html", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "What do I have to do to get this long url thing working?  Does it have to be <www.averylongurl.somethingelse.com/thisisdoingeijjeoijefei/eieijeirerererere/werijgoejgoejgoergj/ergtoijregoejrgoerjgoerjgoergjergoij/index.html>", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "What do I have to do to get this long url thing working?  Maybe it might have to be <www.averylongurl.somethingelse.com/thisisdoingeijjeoijefei/eieijeirerererere/werijgoejgoejgoergj/ergtoijregoejrgoerjgoerjgoergjergoij/index.html>", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D:D", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D :D", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D:D ", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D :D ", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D  :D", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D   :D", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D:D  ", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it easy! :D   :D   ", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it:D easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it:D:D easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it :D :Deasy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it :D:D  easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it :D :D  easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it :D  :Deasy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it :D   :D easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it :D:D  easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "take it :D   :D    easy!", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":Dtake it easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":D:Dtake it easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":D :Dtake it easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":D:D take it easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":D :D take it easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":D  :Dtake it easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":D   :Dtake it easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":D:D  take it easy!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, ":D   :D   take it easy!", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Call Rick at 123 123-1234 or 123 112-2345 or try him at work with 123 123-6798.  Later!!.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "email: superfun@super.com", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "my email address is superfun@super.com.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Hi!!", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "ebay.com is cool.", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "OK... now try //STEGREENFONT//green followed by www.abcdefghijklmnopqrstuvwxyz.com... is that cool???", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Date: //STESTDFONT////STERIGHTALIGN=30//8/2/02 5:30 pm", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//To: //STESTDFONT////STERIGHTALIGN=30////STELEFTINDENT=30//Testing, John Doe, Sherwin Stoole", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Cc: //STESTDFONT////STERIGHTALIGN=30////STELEFTINDENT=30//Mitch, Rob, Ron", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//From: //STESTDFONT////STERIGHTALIGN=30////STELEFTINDENT=30//John Doe", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBOLDFONT//Subj: //STESTDFONT////STERIGHTALIGN=30////STELEFTINDENT=30//Testing the STE", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=1000//   //STERIGHTALIGN=30//Document.txt (10k)", false);
			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "//STEBITMAP=1000//   //STERIGHTALIGN=30//OtherStuff.html (32k)", false);

			STEAppendTextToEngine(gSTERefNum, gEngineRefNum, "Hi there! :)", false);

//			  STESetScroll(gSTERefNum, gEngineRefNum, kScrollEnd);
//			  STESetScrollToRecordNum(gSTERefNum, gEngineRefNum, 4);

			STERenderList(gSTERefNum, gEngineRefNum);

//			oldTextColor = WinSetTextColor (kSTERed);
//			STERenderList(gSTERefNum, gEngineBRefNum);
//			WinSetTextColor (oldTextColor);

			// hyperlink callback routine
			STESetCustomHyperlinkCallback(gSTERefNum, gEngineRefNum, (STECallbackType)HyperlinkCallback, false);


			handled = true;
			break;

	  case frmCloseEvent:
		STEResetEngine(gSTERefNum, gEngineRefNum);
//		STEResetEngine(gSTERefNum, gEngineBRefNum);

		break;
		default:
			break;
		
		}
	
	return handled;
}


/***********************************************************************
 *
 * FUNCTION:	AppHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and set the event
 *				handler for the form loaded.
 *
 * PARAMETERS:	event  - a pointer to an EventType structure
 *
 * RETURNED:	true if the event has handle and should not be passed
 *				to a higher level handler.
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static Boolean AppHandleEvent(EventPtr eventP)
{
	UInt16 formId;
	FormPtr frmP;

	if (eventP->eType == frmLoadEvent)
		{
		// Load the form resource.
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		// Set the event handler for the form.	The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId)
			{
			case MainForm:
				FrmSetEventHandler(frmP, MainFormHandleEvent);
				break;

			default:
//				ErrFatalDisplay("Invalid Form Load Event");
				break;

			}
		return true;
		}
	
	return false;
}


/***********************************************************************
 *
 * FUNCTION:	AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:	nothing
 *
 * RETURNED:	nothing
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static void AppEventLoop(void)
{
	UInt16 error;
	EventType event;

	do {
		EvtGetEvent(&event, evtWaitForever);

		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:	 AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * PARAMETERS:	 nothing
 *
 * RETURNED:	 Err value 0 if nothing went wrong
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static Err AppStart(void)
{
  Err error = 0;

  gSTELoaded = false;
  gSTERefNum = 0;
  
  // load the GSM Library
  error = LoadSmartTextEngine(&gSTERefNum, &gSTELoaded);
  if (error)
	FrmAlert (CantLoadSTEAlert);

  STEOpen(gSTERefNum);

  return error;
}


/***********************************************************************
 *
 * FUNCTION:	AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:	nothing
 *
 * RETURNED:	nothing
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static void AppStop(void)
{
  // Close all the open forms.
  FrmCloseAllForms ();

  STEClose(gSTERefNum);

  // Release the Smart Text Library, if we loaded it
  if (gSTELoaded)
	SysLibRemove(gSTERefNum);
}




/***********************************************************************
 *
 * FUNCTION:	STETestMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:	cmd - word value specifying the launch code. 
 *				cmdPBP - pointer to a structure that is associated with the launch code. 
 *				launchFlags -  word value providing extra information about the launch.
 *
 * RETURNED:	Result of launch
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
static UInt32 STETestMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error;

	cmdPBP = cmdPBP;		// Prevent compiler warning of unused var
	
	error = RomVersionCompatible (ourMinVersion, launchFlags);
	if (error) return (error);

	switch (cmd)
		{
		case sysAppLaunchCmdNormalLaunch:
			// normal application launch
			error = AppStart();
			if (error) 
				return error;
				
			FrmGotoForm(MainForm);
			AppEventLoop();
			AppStop();
			break;

		default:
			break;

		}
	
	return errNone;
}


/***********************************************************************
 *
 * FUNCTION:	PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:	cmd - word value specifying the launch code. 
 *				cmdPB - pointer to a structure that is associated with the launch code. 
 *				launchFlags -  word value providing extra information about the launch.
 * RETURNED:	Result of launch
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	return STETestMain(cmd, cmdPBP, launchFlags);
}

