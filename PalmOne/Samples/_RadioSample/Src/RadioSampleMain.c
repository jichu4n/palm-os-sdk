/******************************************************************************
 * Copyright (c) 2004-2006 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *****************************************************************************/

/**
 @ingroup RadioSample
 */
 
/**
 *  @brief 	This file contains code that shows how to detect if the radio is off or on,
 *		how to turn on the radio, how to turn off the radio, how to query the sim card for
 *		the phone number of the device, how to check if the device has service available
 *		using the signal level.
 *
 *
 *  @version 1.0	- Initial Revision (02/18/04)
 *  @version 1.1	- GSM or CDMA check in PrvGetMobileInfo (10/31/06) - Usha Shekar
 *					- Separated common functions from specific RadioSample related ones.
 *
 *  @file RadioSample.c
 */


#include <PalmOS.h>
#include <HsNav.h>
#include <HsExt.h>
#include <HsPhone.h>
#include "Common.h"
#include "RadioSample.h"	 					
#include "RadioSample_Rsc.h"
#include <TelephonyMgr.h>
#include "MobileServices.h"

extern void MainFormInit(void );
extern void MainFormDeinit(void );
Boolean ProcessEvent(EventType* eventP);

typedef struct
{
	Char		String	[ 500 ];
} DBRecordType;

static void PrvFunctionPrintMessage( Char 	*stringOneP,
									 Char	*stringTwoP,
									 Char	*stringThreeP );


/**
 * @brief PrvTurnRadioOnOff
 *		This function turns the radio on or off depending on the request
 *
 * @param 	turnRadioOn:   IN:  turnRadioOn, yes or no?
 * @retval 			Err:	Error code if there is error, else errNone
 *
 * @version This function was added in version 1.0
 *
 **/

static Err PrvTurnRadioOnOff( Boolean	turnRadioOn )


{
	UInt16						telRefNum;
	Err							error				= errNone;
	FieldPtr					fieldDebugP			= (FieldPtr)NULL;
	FieldPtr					fieldRadioP			= (FieldPtr)NULL;
	FormType				   *frmP				= (FormType *)FrmGetActiveForm( );
	Boolean						 phoneStartPowerState;
	UInt32						 cmd				= 0;
	Char						 conversionString	[ 50 ];
	
			
	fieldDebugP			= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	fieldRadioP			= (FieldPtr)FrmGetPtr( frmP, MainRadioField );

	error = HsGetPhoneLibrary(&telRefNum);
	if (error)
	{
		PrvFunctionPrintMessage( "Couldn't load Telephony Library!", "", "");
		return error;
	}
		
		
	error = (UInt8)PhnLibOpen(telRefNum);
	if (error)
	{
		PrvFunctionPrintMessage( "Couldn't open Telephony Library!", "", "");
		return error;
	}
	
	/* Is the radio on or off? */
	phoneStartPowerState = (unsigned char)PhnLibModulePowered(telRefNum);
	
	/* turn radio on if not already on */
	if (turnRadioOn == true) 
	{ 	if (!phoneStartPowerState)
		{
			error = (UInt8)PhnLibSetModulePower(telRefNum, true);
			SetFieldTextFromStr( fieldRadioP, "Radio on", true);
			SetFieldTextFromStr( fieldDebugP, StrIToH (conversionString, (UInt32) cmd), true);
		}
		else
		{
			SetFieldTextFromStr( fieldRadioP, "Radio already on", true);
			SetFieldTextFromStr( fieldDebugP, StrIToH (conversionString, (UInt32) cmd), true);
		}			
	}
	/* turn radio off if not already off */
	else
	{	if (phoneStartPowerState)
		{
			error = (UInt8)PhnLibSetModulePower(telRefNum, false);
			SetFieldTextFromStr( fieldRadioP, "Radio off", true);
			SetFieldTextFromStr( fieldDebugP, StrIToH (conversionString, (UInt32) cmd), true);
		}
		else
		{
			SetFieldTextFromStr( fieldRadioP, "Radio already off", true);
			SetFieldTextFromStr( fieldDebugP, StrIToH (conversionString, (UInt32) cmd), true);
		}
	}
	
	if ( error )
	{
		Char conversionString[ 30 ];
		SetFieldTextFromStr( fieldDebugP, "Error Turning Phone On/Off", true );
		FrmCustomAlert( RomIncompatibleAlert, "Error Turning Phone On/Off = ",
						StrIToH( conversionString, error ), "" );
	}
		
	PhnLibClose(telRefNum);
	return ( error );
}



/**
 * @brief PrvGetMobileInfo
 *	      This function gets the following info:
 *				1. Is the radio on or off?
 *				2. What is the phone number stored in the Sim card?
 *				3. What is the signal level?
 *
 *
 *
 * @param void:   IN:  	empty
 * @retval		Err:	Error code if there is error, else errNone
 *
 * @version This function was added in version 1.0
 * @version 1.1	- GSM or CDMA check (10/31/06) - Usha Shekar
 *
 **/

static Err PrvGetMobileInfo( void )
{
	Err							 err				= errNone;
	FieldPtr					 fieldRadioP		= (FieldPtr)NULL;
	FieldPtr					 fieldPhoneP		= (FieldPtr)NULL;
	FieldPtr					 fieldSignalLevelP	= (FieldPtr)NULL;
	FormType					*frmP				= (FormType *)FrmGetActiveForm( );

	UInt16						 telRefNum			= 0;
	UInt32						 cmd				= 0;
	Char						 conversionString	[ 50 ];
	FieldPtr					 fieldDebugP		= (FieldPtr)NULL;
	Boolean						 phoneStartPowerState;
	PhnAddressList aList;
	PhnAddressHandle address;
	CharPtr number = NULL;
	UInt16 quality = 0;
	
	UInt32 phnType;
	
	fieldDebugP			= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	fieldRadioP			= (FieldPtr)FrmGetPtr( frmP, MainRadioField );
	fieldPhoneP			= (FieldPtr)FrmGetPtr( frmP, MainPhoneField );
	fieldSignalLevelP	= (FieldPtr)FrmGetPtr( frmP, MainSignalLevelField );
	
	
	err = HsGetPhoneLibrary(&telRefNum);
	if (err)
	{
		PrvFunctionPrintMessage( "Couldn't load Telephony Library!", "", "");
		return err;
	}
	
	/* Is the radio on or off? */
	phoneStartPowerState = (unsigned char)PhnLibModulePowered(telRefNum);
	
	/* Open the phone library */
	err = (unsigned short)PhnLibOpen(telRefNum);
	
	
	if (phoneStartPowerState)
	{
		SetFieldTextFromStr( fieldRadioP, "Radio is on", true);
		SetFieldTextFromStr( fieldDebugP, StrIToH (conversionString, (UInt32) cmd), true);
	}
	else
	{
		SetFieldTextFromStr( fieldRadioP, "Radio is off", true);
		SetFieldTextFromStr( fieldDebugP, StrIToH (conversionString, (UInt32) cmd), true);
	}
	
	
		
	/* What is the phone number stored in the Sim card? */
		
	err = (unsigned short)PhnLibGetOwnNumbers(telRefNum, &aList);
	
	
	if (err)
		SetFieldTextFromStr(fieldPhoneP, "Unavailable", true);
	else
	{
		/* GSM or CDMA? */
		
		HsAttrGet (hsAttrPhoneType, 0, &phnType);
		
		if ( phnType == hsAttrPhoneTypeGSM)
		{
			err = (unsigned short)PhnLibGetNth(telRefNum, aList, 1, &address);
			if (!err)
			{
				number = (char *)PhnLibGetField(telRefNum, address, phnAddrFldPhone);
			}
		
		}
		 else
    	{
			err = (unsigned short)PhnLibAPGetNth(telRefNum, aList, 1, &address);
			if (!err)
			{
				number = (char *)PhnLibAPGetField(telRefNum, address, phnAddrFldPhone);
			}
		}
		MemHandleFree(address);
		MemHandleFree(aList);
		
		
		if (number == NULL)
		{
			SetFieldTextFromStr(fieldPhoneP, "Unavailable", true);
			MemPtrFree(number);
		}
		else
		{
			SetFieldTextFromStr(fieldPhoneP, number, true);
			MemPtrFree(number);
			number = NULL;
		}
	
	}
	
	/* What is the signal level? */
	err = (unsigned short)PhnLibSignalQuality (telRefNum, &quality);
	if (!err)
		SetFieldTextFromStr(fieldSignalLevelP, StrIToA(conversionString, quality), true);
	else
		SetFieldTextFromStr(fieldSignalLevelP, "No signal", true);
	
	PhnLibClose(telRefNum);	
	return ( err );
}


/**
 * @brief ProcessEvent
 *		This function processes the event
 *
 * @param 	eventP   IN:  the event to be processed
 * @retval 	Boolean:	true if event was handled
 *
 * @version This function was added in version 1.1
 *
 **/
Boolean ProcessEvent(EventType * eventP)
{
	Boolean	handled	= false;
	Err	err = errNone;
	FormType	*frmP					= FrmGetActiveForm( );
	FieldPtr 	 fieldDebugP		    = (FieldPtr)NULL;
	fieldDebugP							= (FieldPtr)FrmGetPtr( frmP, MainDebugField );
	
	switch (eventP->data.ctlSelect.controlID)
	{
		case MainMobileButton:		
			// The "Get Mobile Info" button was hit.
			// Get the following mobile info - is the radio on?, phone # and signal level
			err = PrvGetMobileInfo( );
			if (err != errNone) 
			{
				SetFieldTextFromStr( fieldDebugP, "No Mobile Info", true );
			}
			handled = true;		
			break;

		case MainRadioOnButton:
			// The "Radio On" button was hit
			// Turn the radio On
			err = PrvTurnRadioOnOff( true );
			if (err != errNone)
			{
				SetFieldTextFromStr( fieldDebugP, "Error Turning Phone On/Off", true );
			}
			handled = true;
			break;
		case MainRadioOffButton:
			// The "Radio Off" button was hit
			// Turn the radio Off
			err = PrvTurnRadioOnOff( false );
			if (err != errNone)
			{
				SetFieldTextFromStr( fieldDebugP, "Error Turning Phone On/Off", true );
			}
			handled = true;
			break;

		default:
			break;
	}
	
	return handled;
}
					
		

/*
 * FUNCTION: MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:
 *
 * command
 *     menu item id
 */

Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
	UInt16  cardNo;
	LocalID dbID;
	DmSearchStateType searchState;

	
	DmGetNextDatabaseByTypeCreator(true, &searchState, sysFileTApplication,
								 appCreatorID, true, &cardNo, &dbID);

	switch (command)
	{
	case MainOptionsAbout:
		MenuEraseStatus(0);
		HsAboutHandspringApp(cardNo, dbID, "2006", "Palm DTS Team");
		handled = true;
		break;
	
	default:
		break;
	
	}

	return handled;
}




/**
 * @brief MainFormHandleEvent
 *	     Main form event handler.
 *
 *
 * @param *eventP :   IN:  	Pointer to an event.
 * @retval 		boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.0
 *
 **/

Boolean MainFormHandleEvent( EventType	*eventP )
{
	Boolean		 handled				= false;
	FormType	*frmP					= FrmGetActiveForm( );
	

	switch (eventP->eType)
	{
		case frmOpenEvent:
			MainFormInit( );
			
			// Set focus on MainRadioOnButton button
			FrmSetFocus(frmP, noFocus);
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, MainRadioOnButton));
						
			FrmDrawForm(frmP);
			handled = true;
			break;
			
		case menuEvent:
			return MainFormDoCommand(eventP->data.menu.itemID);
			
		case frmCloseEvent:
			MainFormDeinit( );
			handled = true;
			break;
						
		case keyDownEvent:
			
			if (eventP->data.keyDown.keyCode == vchrRockerCenter)
				handled = ProcessEvent(eventP);
			
			break;

		case ctlSelectEvent:
		
			handled = ProcessEvent(eventP);
			break;
			
		default:
			break;
	}
	
	return ( handled );
}

#if 0
	#pragma mark -
#endif


/**
 * @brief AppHandleEvent
 *	     Main application event handler.
 *
 *
 * @param *const eventP:   IN:  	Pointer to an event.
 * @retval 			boolean:	True if event was handled.
 *
 *
 * @version This function was added in version 1.0
 *
 **/

Boolean AppHandleEvent( const EventType	*const eventP )
{
	UInt16		 formId	= 0;
	FormType	*frmP  	= (FormType *)NULL;

	
	if (eventP->eType == frmLoadEvent)
	{
		formId = eventP->data.frmLoad.formID;
		frmP   = FrmInitForm( formId );
		FrmSetActiveForm( frmP );
		
		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler( frmP, MainFormHandleEvent );
				break;
			
			default:
				break;
		}

		return ( true );
	}
	
	return ( false );
}



/**
 * @brief PrvFunctionPrintMessage
 *	      Prints a message for the user.
 *
 *
 * @param *stringOneP :  	 IN:  	Pointer to the first string of the message.
 * @param *stringTwoP :  	 IN:  	Pointer to the second string of the message.
 * @param *stringThreeP :  	 IN:  	Pointer to the third string of the message.
 * @retval 		void:				No value returned.
 *
 *
 * @version This function was added in version 1.0
 *
 **/


static void PrvFunctionPrintMessage( Char 	*stringOneP,
									 Char	*stringTwoP,
									 Char	*stringThreeP )
{
#ifndef TUNGSTEN_W_SCREEN

	Err						error 			= errNone;
#ifdef TUNGSTEN_W_DATABASE
	DmOpenRef				database;
	MemHandle				h;
	DBRecordType			r;  // this is our database record
	UInt16					index			= dmMaxRecordIndex;
#else
	FileHand				file;
#endif
#endif



#ifdef TUNGSTEN_W_SCREEN
	FrmCustomAlert( RomIncompatibleAlert, stringOneP, stringTwoP, stringThreeP );
#else
		
	#ifdef TUNGSTEN_W_DATABASE
	database = DmOpenDatabaseByTypeCreator( 'logt',
											sysFileTApplication,
											dmModeReadWrite );
	if ( !database )
	{
		// Tell the user that we're creating a new DB
		error = DmCreateDatabase( 0,
								  "Tungsten W Log",
								  sysFileTApplication,
								  'logt',
								  false );
	
		if ( !error )
		{
			database = DmOpenDatabaseByTypeCreator( 'logt',
													sysFileTApplication,
													dmModeReadWrite );
			if ( !database )
			{
				error = DmGetLastErr( );
			}
		}
	}

	if ( database )
	{
		h = DmNewRecord( database, &index, sizeof( r ) );
	
		
		// fill the record with data
		StrCopy( r.String, stringOneP );
		StrCat( r.String, " , " );
		StrCat( r.String, stringTwoP );
		StrCat( r.String, " , " );
		StrCat( r.String, stringThreeP );
		StrCat( r.String, "        " );
	
		if ( h )
		{ // could fail due to insufficient memory!
			MemPtr		p		= MemHandleLock( h );
			Err			err		= DmWrite( p, 0, &r,
										   StrLen( stringOneP ) +
										   StrLen( " , " ) * 2  +
										   StrLen( stringTwoP ) +
										   StrLen( stringThreeP ) +
										   StrLen( "        " )  );
	
			MemPtrUnlock( p );
			DmReleaseRecord( database, index, true);
		}
	}


	if ( database )
		DmCloseDatabase( database );
	#else
	
    file = FileOpen( 0, "Tungsten_W_Log.csv", sysFileTFileStream, 0, fileModeAppend, &error );
	if ( error != errNone )
	{
		FrmCustomAlert( RomIncompatibleAlert, "Error opening Tungsten_W_Log.csv", " ", " " );
		return;
	}
	
	FileWrite( file, stringOneP, StrLen( stringOneP ), 1, &error  );
	FileWrite( file, " , ", StrLen( " , " ), 1, &error );
	FileWrite( file, stringTwoP, StrLen( stringTwoP ), 1, &error );
	FileWrite( file, " , ", StrLen( " , " ), 1, &error );
	FileWrite( file, stringThreeP, StrLen( stringThreeP ), 1, &error );
	FileWrite( file, "\r\n", StrLen( "\r\n" ), 1, &error );

    FileClose( file );
	#endif
#endif
}


/* EOF *******************************************************************************************/
