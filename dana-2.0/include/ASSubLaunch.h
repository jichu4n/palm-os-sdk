/*************************************************************************
 *
 * Copyright (c) 2002 by AlphaSmart, Inc.
 * All rights reserved.
 *
 * FILE:            ASSubLaunch.h
 *
 * DESCRIPTION: AlphaSmart sub-launch parameter structure definitions.
 *
 ****************************************************************************/

#ifndef AS_SUB_LAUNCH_H
#define AS_SUB_LAUNCH_H

//------------------------------------------------------------------------
// AlphaSmart sub-launch parameter structure used to pass parameters to 
// sub-launched AlphaSmart IrDA and USB text transfer utilities.
//------------------------------------------------------------------------

typedef struct
{
	// pointer to the first byte of the text to transfer to the host
	UInt8 *					pubText;

	// pointer to a null-terminated ASCII string of that is to be displayed
	// as the name of the data being transferred to the host
	// NOTE: if this string is too long, it will be truncated to fit. In
	// the standard system font (FONT 0), approximately 30 characters will 
	// fit horizontally on the screen.
	UInt8 *					pubDisplayName;

	// flags that control the behavior of the sub-launched application
	// for valid flag values, please see the "asSubLaunch..." 
	// #defines below
	UInt32					ulFlags;
	
	// length, in bytes, of the text pointed to by "pubText."
	UInt16					uwTextLength;

} asSubLaunchTextTransferParms;

typedef asSubLaunchTextTransferParms * asSubLaunchTextTransferParmsP;

//----------------------------------------------------------------------
// bitwise flag value constants for use in the "ulFlags" field of the
// asSubLanuchTextTransferParms structure
//----------------------------------------------------------------------

// This flag value indicates that no flags are set. The sub-launched
// application executes its default behavior.
// 
#define asSubLaunchNoFlags						(0x00000000)

// This flag instructs sub-launched applications that normally wait for 
// some user input before performing their function to perform that
// function without waiting. 
//
// An example of this behavior is the USBEmu USB emulator and text 
// transfer application. It normally waits in emulation mode for the 
// user to press the "Send" key before transferring text to the host. 
// If this flag is set, USBEmu transfers any text it was given when 
// sub-launched without waiting for the user to press "Send."
//
#define asSubLaunchNoWaitToStart				(0x00000001)

// This flag instructs sub-launched applications that normally wait for
// some user input before exiting to exit as soon as their function is 
// performed.
//
// An example of this behavior is the USBEmu USB emulator and text
// transfer application. Once it finishes sending any text it was 
// sub-launched with to the host, it resumes emulation mode and does
// not return to its caller until the user presses a "Done" button.
// If this flag is set, USBEmu returns to its caller as soon as it
// has finished sending the text it was given.
//
#define asSubLaunchNoWaitToExit					(0x00000002)

// This value contains all of the as yet unused flag bits. This is used
// to validate the ulFlags field, i.e., none of the bits in this value
// may be set in a valid ulFlags field.
//
#define asSubLaunchUnusedFlags					( ~ ( asSubLaunchNoWaitToStart | \
												      asSubLaunchNoWaitToExit  ) )
#endif
