/** 
 * @file	PhoneInfo.c
 *
 * Phone Info
 *
 * Phone Info Application. Display some information about the phone.
 *
 ***************************************************************/
 
#include <PalmOS.h>
#include <HsPhone.h>
#include <HsNav.h>
#include <HsExt.h>
#include <palmOneResources.h>

#define NON_PORTABLE
#include <HwrMiscFlags.h>  // For hwrOEMCompanyIDHandspring
#undef NON_PORTABLE

#include "PhoneInfo.h"
#include "PhoneUtils.h"

// -------------------------------------------------------------
// Constants
// -------------------------------------------------------------
// Define the minimum OS version we support

#define MIN_VERSION  sysMakeROMVersion(5,0,0,sysROMStageRelease,0)
#define LAUNCH_FLAGS (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)

static void		DisplaySignalStrength (UInt16 PhoneLibRefNum, FormPtr frmP);


/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: This routine checks that a ROM version is meet your
 *              minimum requirement.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags that indicate if the application 
 *                                UI is initialized.
 *
 * RETURNED:    error code or zero if rom is compatible
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
Err 
RomVersionCompatible (UInt32 requiredVersion, UInt16 launchFlags )
{
	
	UInt32 rom = 0, hal = 0, company = 0, device = 0, hsFtrVersion;

	if (FtrGet (hsFtrCreator, hsFtrIDVersion, &hsFtrVersion) != 0)
	{
		DisplayError ("Not a Handspring Device", (UInt16) 
hsFtrVersion);
		return (1);
	}
    FtrGet (sysFtrCreator, sysFtrNumOEMHALID, &hal);
    FtrGet (sysFtrCreator, sysFtrNumOEMCompanyID, &company);
    FtrGet (sysFtrCreator, sysFtrNumOEMDeviceID, &device);
	FtrGet (sysFtrCreator, sysFtrNumROMVersion, &rom);
	if (device == hsDeviceIDOs5Device1Sim)
	{
		DisplayError ("This will not work on the simulator", 0);
		return (1);
	}	
	if (	rom		  >= requiredVersion
		&&	company	  == hwrOEMCompanyIDHandspring     
		&&	device	  == hsDeviceIDOs5Device1
		&&	(	hal	  == hsHALIDHandspringOs5Rev1
			 ||	hal	  == hsHALIDHandspringOs5Rev1Sim) )
	  {
		return 0;
	  }

	if ((launchFlags & LAUNCH_FLAGS) == LAUNCH_FLAGS)
	{
		FrmAlert (rscAlertRomIncompatible);
	
		// Pilot 1.0 will continuously relaunch this app unless we switch to 
		// another safe one.
		if (rom < sysMakeROMVersion(2,0,0,sysROMStageRelease,0))

			AppLaunchWithCommand (sysFileCDefaultApp, 
					sysAppLaunchCmdNormalLaunch, NULL);
	}
	
	return sysErrRomIncompatible;
}



/***********************************************************************
 *
 * FUNCTION:    DisplayError
 *
 * DESCRIPTION: This routine displays an error dialog.
 *
 * PARAMETERS:  msg - error message
 *				err - error code
 *
 * RETURNED:    void
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
void DisplayError (Char * msg, Err err)
{
  Char		  errStr[32];


  StrPrintF (errStr, "0x%x", err);
  FrmCustomAlert (rscCustomText, msg, errStr, NULL);
}



/****************************************************************/
/* GetResourceCopyHandle */
/**
 * This routine returns a copy of the specified resource in
 * a newly allocated memory block.
 *
 * \param	resType		  IN	Resource Type
 * \param	resID		  IN	Resource ID
 *
 * \retval  MemHandle
 *
 ****************************************************************/
MemHandle
GetResourceCopyHandle (UInt32 resType, UInt16 resID)
{
	UInt32		size;
	MemHandle	h;
	MemHandle	resH;
	MemPtr		resP;
	MemPtr		ptr;

	resH = DmGetResource (resType, resID);
	resP = MemHandleLock (resH);
	size = MemPtrSize (resP);
	h = MemHandleNew (size);
	ptr = MemHandleLock (h);
	MemMove (ptr, resP, (Int32) size);
	MemPtrUnlock (ptr);
	MemPtrUnlock(resP);
	DmReleaseResource (resH);
	
	return (h);
}


/****************************************************************/
/* CopyPtrToHandle */
/**
 * This routine copy the number of specified byte from the pointer
 * passed to a newly allocated handle
 *
 * \param	srcP		  IN	Pointer to array to copy
 * \param	srcLen		  IN	Number of bytes to copy
 *
 * \retval  MemHandle
 *
 ****************************************************************/
MemHandle
CopyPtrToHandle (MemPtr srcP, UInt32 srcLen)
{
	MemHandle	dstH;
	MemPtr		dstP;

	dstH = MemHandleNew (srcLen);
	dstP = MemHandleLock (dstH);
	MemMove (dstP, srcP, (Int32) srcLen);
	MemPtrUnlock(dstP);

	return (dstH);
}




/****************************************************************/
/* AppendPtrToHandle */
/**
 * This routine append the number of specified byte from the pointer
 * passed to a newly allocated handle
 *
 * \param	srcP		  IN	Pointer to array to copy
 * \param	srcLen		  IN	Number of bytes to copy
 *
 * \retval  MemHandle
 *
 ****************************************************************/
MemHandle
AppendPtrToHandle (MemHandle dstH, MemPtr srcP, UInt32 srcLen)
{
//	MemHandle	dstH;
	MemPtr		dstP;
	Int32		origLen;
	Err			err = errNone;

	origLen = StrLen (MemHandleLock (dstH));
	err = MemHandleResize (dstH, origLen + srcLen + 1);
	if (err == errNone)
	{
		dstP = MemHandleLock (dstH);
		MemMove ( (CharPtr)dstP+origLen, srcP, (Int32) srcLen);
		MemPtrUnlock(dstP);
	}

	return (dstH);
}




/***********************************************************************
 *
 * FUNCTION:    FormatNumberInsertAt
 *
 * DESCRIPTION: This routine inserts the a character into a string 
 *              at a specified position.
 *
 * PARAMETERS:  str -> string to insert into
 *              chr -> character to insert
 *              pos -> position to insert at.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *		Name	Date	Description
 *		----	----	-----------
 *		art		7/12/99	Initial Revision
 *
 ***********************************************************************/
void FormatNumberInsertAt (CharPtr str, Char chr, Word pos)
{
	Word len;
	
	len = StrLen (str);

	if (len > pos)
		{
		MemMove (&str[pos+1], &str[pos], len - pos + 1);
		str[pos] = chr;
		}
}


/***********************************************************************
 *
 * FUNCTION:    FormatNumber
 *
 * DESCRIPTION: This routine format the passed number by added hyphens at 
 *              the appropriate positions.
 *
 * PARAMETERS:  number -> string of digits.
 *              fullNumber -> true if complete number false if partial 
 *
 * RETURNED:    handle of a block containing the format string or NULL
 *              if the string is empty.
 *
 * REVISION HISTORY:
 *		Name	Date	Description
 *		----	----	-----------
 *		art		7/12/99	Initial Revision
 *      dh      4/15/02  Allow the + to be any character.
 *
 ***********************************************************************/
VoidHand FormatNumber (CharPtr number, Boolean fullNumber)
{
	Char		chr;
	Word		i;
	Word		len;
	Word		maxLen;
	Word		unformatedLen;
	Boolean		formatIt;
	VoidHand 	formatedH;
	CharPtr		formatedP;
	CountryType	country;

	len = StrLen (number);
	if (len == 0) return (0);
	

	country = (CountryType)PrefGetPreference(prefCountry);


	// Allocate a string large enough for the largest formatted string.
	// add a maximum of three hyphens plus a null-terminator.
	maxLen = len + 4;
	formatedH = MemHandleNew (maxLen);
	formatedP= MemHandleLock (formatedH);
	MemSet (formatedP, maxLen, 0);


	// Remove all characters that are not 0-9, asterisk, number sign,  or
	// plus sign.
	unformatedLen = 0;
	for (i = 0; i < len; i++)
		{
		chr = number[i];
		
		if ( (chr >= chrDigitZero && chr <= chrDigitNine) ||
			  (chr == chrPlusSign) ||
			  (chr == chrAsterisk) ||
			  (chr == chrNumberSign) )
			{
			formatedP[unformatedLen++] = chr;
			}
		}


	if (unformatedLen && (country == cUnitedStates || country == cCanada))
		{
		if (*formatedP != '1')
			formatIt = (unformatedLen == 7 || unformatedLen == 10 || 
							((*formatedP == chrPlusSign) && unformatedLen == 12) ||
							((! fullNumber) && unformatedLen < 10));
		else
			formatIt = (unformatedLen == 11 ||
							((! fullNumber) && unformatedLen < 11));

		if (formatIt) 
			{
			// If the first digit is a one, inset hyphens after the 1st, 
			// 4th and 7th digits.
			if (*number == chrDigitOne)
				{
				FormatNumberInsertAt (formatedP, chrHyphenMinus, 7);
				FormatNumberInsertAt (formatedP, chrHyphenMinus, 4);
				FormatNumberInsertAt (formatedP, chrHyphenMinus, 1);
				}
				
			// If the first digit is not a zero or one, inset hyphens after the 3st, 
			// 6th digits.
			else if (*number >= chrDigitTwo && *number <= chrDigitNine)
				{
				if (unformatedLen > 7)
					FormatNumberInsertAt (formatedP, chrHyphenMinus, 6);
				FormatNumberInsertAt (formatedP, chrHyphenMinus, 3);
				}

			// If the first digit is a + and the second digit is a 1, inset hyphens
			// after the 2nd, 5th and 8th digits.
			else if (*number == chrPlusSign && number[1] == chrDigitOne)
				{
				if (unformatedLen > 9)
					FormatNumberInsertAt (formatedP, chrHyphenMinus, 8);
				FormatNumberInsertAt (formatedP, chrHyphenMinus, 5);
				FormatNumberInsertAt (formatedP, chrHyphenMinus, 2);
				}
			}
		}

	if (StrLen (formatedP))
		{
		MemPtrUnlock (formatedP);
		}
	else
		{
		if (formatedH)
		  MemHandleFree (formatedH);

		formatedH = NULL;
		}
	
	return (formatedH);
}



/****************************************************************/
/* CheckPhonePower */
/**
 * Check if the phone is powered and the SIM is ready
 *
 * \retval  Boolean
 *
 ****************************************************************/
Boolean CheckPhonePower (UInt16	PhoneLibRefNum)
{
	UInt16 isSimReady = false;
	
	// Make sure that the phone is powered...
	if (PhnLibModulePowered (PhoneLibRefNum) != phnPowerOn )
	{
		if (resAlertPhoneNotReadyTurnOn == FrmAlert (resAlertPhoneNotReady))
		{
			PhnLibSetModulePower (PhoneLibRefNum, 1);
		}
		else
			return false;
	}

	//Check if radio has initiliazed the SIM on GSM phone
	//if not, will ask the user if he wants to check again or cancel out
	while (!isSimReady)  
	{
		if ((PhnLibGetSIMStatus(PhoneLibRefNum) != simReady) && (GetPhoneType() == hsAttrPhoneTypeGSM))
		{
			if (resAlertSimNotReadyCheck == FrmAlert (resAlertSimNotReady))
			{
				isSimReady = false;
			}
			else
			{
				return false;
			}
		}
		else
		{
			isSimReady = true;
		}

	}
	return true;
}

/***************************************************************
 * Function:
 *	  GetPhoneType 
 * Summary: Return the phone type: cdma or gsm?
 * Parameters:
 * Returns: 

 *
 **************************************************************/
UInt32 GetPhoneType (void)
{

  UInt32 phnType = hsAttrPhoneTypeGSM; 
  
  HsAttrGet (hsAttrPhoneType, 0, &phnType);
  
  return phnType;
}


/****************************************************************/
/* DisplaySignalStrength */
/**
 * Display the signal strength on the given form. The form must have
 * the appropriate resources in it.
 *
 * \param	frmP		  IN	Form to display info on.
 *
 *
 ****************************************************************/
static void 
DisplaySignalStrength (UInt16 PhoneLibRefNum, FormPtr frmP)
{
  Err				err;
  UInt16			quality;
  MemHandle			textH;
  MemHandle			oldTextH;
  Char				qualityStr[4];
  FieldPtr			fld;

  err = PhnLibSignalQuality (PhoneLibRefNum, &quality);
  if (!err)
	{
	  StrIToA (qualityStr, quality);

	  if (quality == 99)
		{
		  textH = GetResourceCopyHandle (strRsc, resStrUnknownCSQ);
		}
	  else
		{
		  textH = CopyPtrToHandle (qualityStr, StrLen (qualityStr)+1);
		}

	  fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldSignalStrength));

	  // Get the old text handle
	  oldTextH = FldGetTextHandle (fld);

	  // Update the display
	  FldSetTextHandle (fld, textH);

	  // Free the old handle
	  if (oldTextH != NULL)
		MemHandleFree (oldTextH);
	}
}



/***********************************************************************
 *
 * FUNCTION:    ShowCDMAPhoneInfo
 *
 * DESCRIPTION: This routine returns the main voice Phone # for a CDMA phone.
 *
 * PARAMETERS:  string to return the phone # to.
 *
 * RETURNED:    phone #.
 *
 ***********************************************************************/
void ShowCDMAPhoneInfo (UInt16 PhoneLibRefNum, FormPtr frmP) 
{
	Err					err;
	PhnAddressList  	list;
	PhnAddressHandle	address;
	Boolean				radioPowered = PhnLibModulePowered (PhoneLibRefNum);

	MemHandle			textH=NULL;
	CharPtr				number=NULL;
	FieldPtr			fld=NULL;

	CharPtr		        esn;
    CharPtr		        modemSWRev, hostSWRev, modemHWRev;
  	
	// Get the list of "own number" (voice, data, and fax numbers).
	if (radioPowered)
	{	
		err = PhnLibGetOwnNumbers (PhoneLibRefNum, &list);
		if (!err)
		{
			err = PhnLibAPGetNth (PhoneLibRefNum, list, 1, &address);
			if (!err)
			{	
				if (address)
				{	// Get the voice phone number
					number = PhnLibAPGetField (PhoneLibRefNum, address, phnAddrFldPhone);
					MemHandleFree (address);
				}
			}			
			MemHandleFree (list);
		}
		if (number)
		{
			textH = FormatNumber (number, true);
			MemPtrFree (number);
			number = NULL;
		}			
		if (!textH)
			textH = GetResourceCopyHandle (strRsc, resStrNumberNotOnSim);
			
		fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldPhoneNumber));
		FldSetTextHandle (fld, textH);


	  /*
	  // ESN:

	  // PRL REV:

	  // PRI CHECKSUM:

	  */
  	  err = PhnLibCardInfoEx (PhoneLibRefNum, 0, 0, &modemSWRev, &esn, NULL, &hostSWRev, &modemHWRev, NULL, NULL, NULL);
	  if (esn && *esn)
		{
		  textH = CopyPtrToHandle (esn, StrLen (esn) + 1);
		  fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldESN));
		  FldSetTextHandle (fld, (Handle) textH);
		  MemPtrFree (esn);
		}


#if DISPLAY_RADIO_INFO
	  {
	  
	  if (hostSWRev && *hostSWRev)
		{
		  textH = CopyPtrToHandle (hostSWRev, StrLen (hostSWRev) + 1);
		  fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldRadioHost));
		  FldSetTextHandle (fld, (Handle) textH);
		  MemPtrFree (hostSWRev);
		}

	  if (modemSWRev && *modemSWRev)
		{
		  textH = CopyPtrToHandle (modemSWRev, StrLen (modemSWRev) + 1);
		  fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldRadioModem));
		  FldSetTextHandle (fld, (Handle) textH);
		  MemPtrFree (modemSWRev);
		}

	  if (modemHWRev && *modemHWRev)
		{
		  textH = CopyPtrToHandle (modemHWRev, StrLen (modemHWRev) + 1);
		  fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldRadioHardware));
		  FldSetTextHandle (fld, (Handle) textH);
		  MemPtrFree (modemHWRev);
		}
	  }
#endif

#if DO_LATER_PCS
	  /*
	  // PCS VISION USER NAME:
	  */
	  {
	  // <chg 08/27/2002 AGM> Display username in Phone Info dialog. The "Plan" username
	  // is the username that was provisioned into slot 1, so use this. 
	  Err err;

	  PhnPDPPayload * pdpPayLoad = MemPtrNew (sizeof (PhnPDPPayload));
	  PhnPDPReadAck pdpReadAck;
	  err = PhnLibGetPDP (PhoneLibRefNum, 1, &pdpReadAck, pdpPayLoad);
	  if (!err && pdpReadAck == phnPDPReadOK)
		{		  
		  if (!pdpPayLoad->restrict_nai)
			{
			  Char * usernameP;
			  Char * atSign;
			  Boolean fitsOnLine;
			  RectangleType bounds; 
			  UInt16  usernamePixels;
			  UInt16  usernameLength;
			  Char * secondLine = NULL;
			  UInt16 secondLineLength = 0;
			 
			  fld = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, PhoneInfo1UsernameField));
 
			  textH = MemHandleNew (pdpPayLoad->nai_length + 1);
			  usernameP = MemHandleLock (textH);
			  MemMove (usernameP, pdpPayLoad->nai, pdpPayLoad->nai_length);
			  usernameP[pdpPayLoad->nai_length] = 0;
			  atSign = StrChr (usernameP, '@');
			  if (atSign)
				  *atSign = 0;


              // check if the username fits on one line
			  FldGetBounds (fld, &bounds); 
		      usernamePixels =  bounds.extent.x;
			  usernameLength = StrLen (usernameP);
			  
			  FntCharsInWidth (usernameP, &usernamePixels, &usernameLength,
				  &fitsOnLine);

			  // if it does not fit on one line, put only the part that does
			  if (!fitsOnLine)
				{
				  secondLineLength = StrLen (usernameP) - usernameLength;
				  secondLine = MemPtrNew ((secondLineLength+1)* sizeof (Char));
				  StrNCopy (secondLine, usernameP+(usernameLength * sizeof (Char)), secondLineLength);
				  usernameP[usernameLength] = 0; 
				}
			  
			  MemHandleUnlock (textH);
			  FldSetTextHandle (fld, (Handle) textH);

			  // if the user name does not fit on one line, write out the second
			  // line
              if (!fitsOnLine)
				{
				  // check what fits on the second line
			      usernamePixels = bounds.extent.x;  
				  usernameLength = StrLen (secondLine);
				  FntCharsInWidth (secondLine, &usernamePixels, &usernameLength,
				  &fitsOnLine);
				 
				  if (!fitsOnLine)
					{
					  StrCopy (secondLine + (usernameLength - 3)*sizeof (Char),
						  kEllipsis); 
						
					}
			
				  textH = MemHandleNew (secondLineLength + 1);
				  usernameP = MemHandleLock (textH);
				  MemMove (usernameP, secondLine, secondLineLength); 
				  usernameP[secondLineLength] = 0;
				  fld = FrmGetObjectPtr (frm, FrmGetObjectIndex (frm, PhoneInfo2UsernameField));

				  MemPtrUnlock (usernameP);
				  FldSetTextHandle (fld, (Handle) textH);
				  MemPtrFree (secondLine);
				}
			  MemHandleUnlock (textH);
			  FldSetTextHandle (fld, (Handle) textH);
			  MemPtrFree (pdpPayLoad);
			}
		}
	  else
		  // < TODO AGM> : Display a string to tell the user that the username does
		  // not exist 
		  MemPtrFree (pdpPayLoad);
	  }
#endif	// DO_LATER_PCS

		
	} // if (radioPowered)
	
	
}



/***********************************************************************
 *
 * FUNCTION:    ShowGSMPhoneInfo
 *
 * DESCRIPTION: This routine returns the main voice Phone # for a GSM phone.
 *
 * PARAMETERS:  string to return the phone # to.
 *
 * RETURNED:    phone #.
 *
 ***********************************************************************/
void ShowGSMPhoneInfo (UInt16 PhoneLibRefNum, FormPtr frmP) 
{
	Err					err;
	PhnAddressList  	list;
	PhnAddressHandle	address;
	Boolean				radioPowered = PhnLibModulePowered (PhoneLibRefNum);
	
	MemHandle			textH=NULL;
	CharPtr				number=NULL;
	FieldPtr			fld=NULL;

	CharPtr		        emei, firmware, imsi;
	
	// Get the list of "own number" (voice, data, and fax numbers).
	if (radioPowered)
	{	
		err = PhnLibGetOwnNumbers (PhoneLibRefNum, &list);
		if (!err)
		{
			err = PhnLibGetNth (PhoneLibRefNum, list, 1, &address);
			if (!err)
			{	
				if (address)
				{	// Get the voice phone number
					number = PhnLibGetField (PhoneLibRefNum, address, phnAddrFldPhone);
					MemHandleFree (address);
				}
			}			
			MemHandleFree (list);
		}
		if (number)
		{
			textH = FormatNumber (number, true);
			MemPtrFree (number);
			number = NULL;
		}			
		if (!textH)
			textH = GetResourceCopyHandle (strRsc, resStrNumberNotOnSim);
		fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldPhoneNumber));
		FldSetTextHandle (fld, textH);
		
		
	  	if (PhnLibCardInfo (PhoneLibRefNum, 0, 0, &firmware, &emei))
		{

#if LATER		
		  if (firmware && *firmware)
			{
			  textH = CopyPtrToHandle (firmware, StrLen (firmware) + 1);

			  fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, PhoneInfoFirewarefirmwareField));
			  FldSetTextHandle (fld, (Handle) textH);
			  MemPtrFree (firmware);
			}
#endif // LATER

		  if (emei && *emei)
			{
			  textH = CopyPtrToHandle (emei, StrLen (emei) + 1);
			  fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldIMEI));
			  FldSetTextHandle (fld, (Handle) textH);
			  MemPtrFree (emei);
			}
		}
		
		if (PhnLibSIMInfo (PhoneLibRefNum, &imsi))
		{
			textH = CopyPtrToHandle( imsi, StrLen (imsi) + 1);
			fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldIMSI));
			FldSetTextHandle (fld, (Handle) textH);
			MemPtrFree (imsi);
		}
		
	} // if (radioPowered)
}



/***********************************************************************
 *
 * FUNCTION:    ShowhoneInfo
 *
 * DESCRIPTION: This routine displays the common information for both
 *				GSM & CDMA phones and calls other routines to display
 *				specific GSM & CDMA info.
 *
 * PARAMETERS:  string to return the phone # to.
 *
 * RETURNED:    phone #.
 *
 ***********************************************************************/
void ShowPhoneInfo (UInt16 PhoneLibRefNum, FormPtr frmP) 
{
	Err					err;
	Boolean				radioPowered = PhnLibModulePowered (PhoneLibRefNum);

	MemHandle			textH=NULL;
	FieldPtr			fld=NULL;

  	PhnOperatorID	    id = 0;
  	CharPtr				CurrentOperator = NULL;
	PhnRoamStatus	  	roamStatus;
	Char			  	versionStr[hsVersionStringSize];
  	

	if (radioPowered)
	{	
		err = PhnLibCurrentOperator (PhoneLibRefNum, &id, &CurrentOperator, NULL);
		textH = CopyPtrToHandle (CurrentOperator, StrLen (CurrentOperator)+1);

		fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldCurrOperator));
		FldSetTextHandle (fld, textH);

		DisplaySignalStrength(PhoneLibRefNum, frmP);		


		PhnLibRoaming (PhoneLibRefNum, &roamStatus);
  
		if (roamStatus != PhnRoamStatusOff)
			textH = CopyPtrToHandle ("Roaming", StrLen ("Roaming")+1);
		else
			textH = CopyPtrToHandle ("Not Roaming", StrLen ("Not Roaming")+1);
			
  		fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldRoam));
		FldSetTextHandle (fld, textH);
			
	
	  /*
	  // SOFTWARE REV:
	  */
	  MemSet (versionStr, hsVersionStringSize, 0);
	  if (HsGetVersionString (hsVerStrComplete, versionStr, NULL) == errNone)
		{
		  if (versionStr[0])
		  {
			textH = CopyPtrToHandle (versionStr, StrLen (versionStr) + 1);
		    fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldSoftware));
		    FldSetTextHandle (fld, (Handle) textH);
		  }
		}

	  /*
	  // HARDWARE REV:
	  */
	  MemSet (versionStr, hsVersionStringSize, 0);
	  if (HsGetVersionString (hsVerStrHardware, versionStr, NULL) == errNone)
		{
		  if (versionStr[0])
		  {
			textH = CopyPtrToHandle (versionStr, StrLen (versionStr) + 1);
		    fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldHardware));
		    FldSetTextHandle (fld, (Handle) textH);
		  }
		}
		
	  /*
	  // HANDSPRING SN:
	  */

	  MemSet (versionStr, hsVersionStringSize, 0);
	  if (HsGetVersionString (hsVerStrSerialNo, versionStr, NULL) == errNone)
		{
		  if (versionStr[0])
		  {
			textH = CopyPtrToHandle (versionStr, StrLen (versionStr) + 1);
		    fld = FrmGetObjectPtr (frmP, FrmGetObjectIndex (frmP, resFieldSerial));
		    FldSetTextHandle (fld, (Handle) textH);
		  }
		}

		if (GetPhoneType() == hsAttrPhoneTypeCDMA)
		{
			ShowCDMAPhoneInfo (PhoneLibRefNum, frmP);
		}
		else // default is GSM 
		{
			ShowGSMPhoneInfo (PhoneLibRefNum, frmP);
		}
		

		
	} // if (radioPowered)
	
	
}

