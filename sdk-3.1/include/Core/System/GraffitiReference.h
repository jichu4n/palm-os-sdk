/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     GraffitiReference.h
 * AUTHOR:	 Roger Flores: June 25, 1996
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *	  This file defines the Graffiti Reference routines.
 *
 ***********************************************************************/

#ifndef __GRR_H__
#define __GRR_H__

typedef enum 
	{
	referenceAlpha, 
	referencePunc1, 
	referencePunc2,
	
#if COUNTRY != COUNTRY_JAPAN
	referencePunc3,
#endif

	referenceExtended, 
	
#if COUNTRY != COUNTRY_JAPAN
	referenceAccent,
#endif

	referenceDefault = 0xff		// based on graffiti mode (referenceAlpha)
	} ReferenceType;

#define referenceFirst		referenceAlpha

#if COUNTRY != COUNTRY_JAPAN
#define referenceLast		referenceAccent
#else
#define referenceLast		referenceExtended
#endif


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
