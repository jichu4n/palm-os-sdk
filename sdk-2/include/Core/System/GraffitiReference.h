/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		GraffitiReference.h
 *
 * Description:
 *		This file defines the Graffiti Reference routines
 *
 * History:
 *   	6/25/96  roger - Created
 *
 *******************************************************************/  


#ifndef __GRR_H__
#define __GRR_H__




typedef enum 
	{
	referenceAlpha, 
	referencePunc1, 
	referencePunc2, 
	referencePunc3, 
	referenceExtended, 
	referenceAccent,
	referenceDefault = 0xff		// based on graffiti mode (referenceAlpha)
	} ReferenceType;


#define referenceFirst		referenceAlpha
#define referenceLast		referenceAccent


/************************************************************
 * Graffiti Reference procedures
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


extern void SysGraffitiReferenceDialog (ReferenceType referenceType)
							SYS_TRAP(sysTrapSysGraffitiReferenceDialog);


#ifdef __cplusplus
}
#endif

#endif // __GRR_H__
