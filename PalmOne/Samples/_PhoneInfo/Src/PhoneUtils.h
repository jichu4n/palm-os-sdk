/**************************************************************************************************
 *
 * Copyright (c) 2004 palmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/**
 @ingroup _PhoneInfo
 */

/**
 * @brief This is the header file for the Phone Utility functions.
 *	    All applications using the telephony API will use these utilities.
 *	    
 *
 *
 *
 * @version Version 1.0 	Initial version.
 *										"TelIsFunctionSupported"
 *
 *
 *
 * @file PhoneUtils.h
 *
 *
 **/



// -------------------------------------------------------------
// Prototypes
// -------------------------------------------------------------

/**
 * Function RomVersionCompatible
 *
 *
 * @param requiredVersion:	IN:     Required version of ROM
 * @param launchFlags:		IN:     Launch flags to be used
 * @retval				Err:    The error returned from the function
 **/

Err			RomVersionCompatible (UInt32 requiredVersion, UInt16 launchFlags);

/**
 * Function DisplayError
 *
 *
 * @param msg:	IN:      message to be displayed
 * @param err:	IN:      error related to message
 * @retval		void:    void
 **/

void 		DisplayError (CharPtr msg, Err err);

/**
 * Function GetResourceCopyHandle
 *
 *
 * @param resType:	IN:           	type of resource
 * @param resID:		IN:     		resource id
 * @retval			MemHandle:    	The handle returned
 **/

MemHandle	GetResourceCopyHandle (UInt32 resType, UInt16 resID);

/**
 * Function CopyPtrToHandle
 *
 *
 * @param srcP:		IN:           	pointer to source
 * @param srcLen:		IN:     		length of source
 * @retval			MemHandle:    	The handle returned
 **/

MemHandle	CopyPtrToHandle (MemPtr srcP, UInt32 srcLen);

/**
 * Function AppendPtrToHandle
 *
 *
 * @param dstH:		IN:           	MemHandle to dst
 * @param srcP:		IN:           	pointer to source
 * @param srcLen:		IN:     		length of source
 * @retval			MemHandle:    	The handle returned
 **/

MemHandle	AppendPtrToHandle (MemHandle dstH, MemPtr srcP, UInt32 srcLen);

/**
 * Function CheckPhonePower
 *
 *
 * @param PhoneLibRefNum:	IN:     	reference number of phone library
 * @retval			Boolean:    	boolean value returned
 **/

Boolean		CheckPhonePower (UInt16	PhoneLibRefNum);

/**
 * Function GetPhoneType
 *
 *
 * @param void:	IN:     	void
 * @retval		UInt32:    	integer value indicating phone type
 **/

UInt32		GetPhoneType (void);

/**
 * Function FormatNumber
 *
 *
 * @param number:			IN:        	input number
 * @param fullNumber:		IN:     	full phone number
 * @retval			VoidHand:    	void
 **/

VoidHand	FormatNumber (CharPtr number, Boolean fullNumber);

/**
 * Function FormatNumberInsertAt
 *
 *
 * @param str:		IN:   	string with number
 * @param chr:		IN:     	char (to be inserted?)
 * @param pos:		IN:     	position (to insert the char?)
 * @retval			void:    	void
 **/

void 		FormatNumberInsertAt (CharPtr str, char chr, Word pos);

/**
 * Function ShowCDMAPhoneInfo
 *
 *
 * @param PhoneLibRefNum:	IN:        	reference number of phone library
 * @param frmP:			IN:     	form pointer
 * @retval				void:    	void
 **/


void 		ShowCDMAPhoneInfo (UInt16 PhoneLibRefNum, FormPtr frmP); 

/**
 * Function ShowGSMPhoneInfo
 *
 *
 * @param PhoneLibRefNum:	IN:        	reference number of phone library
 * @param frmP:			IN:     	form pointer
 * @retval				void:    	void
 **/

void 		ShowGSMPhoneInfo (UInt16 PhoneLibRefNum, FormPtr frmP); 

/**
 * Function ShowPhoneInfo
 *
 *
 * @param PhoneLibRefNum:	IN:        	reference number of phone library
 * @param frmP:			IN:     	form pointer
 * @retval				void:    	void
 **/

void 		ShowPhoneInfo (UInt16 PhoneLibRefNum, FormPtr frmP);

