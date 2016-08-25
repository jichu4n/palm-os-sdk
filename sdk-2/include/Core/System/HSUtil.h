/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1996-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		HSUtil.h
 *
 * Description:
 *		HotSync utility definitions.
 *
 *		Platform-independence data conversion macros.
 *
 * History:
 *   	7/19/96  vmk	Created by Vitaly Marty Kruglikov
 *
 *******************************************************************/

#ifndef __HS_UTIL_H__
#define __HS_UTIL_H__

// Cross-map Pilot types used in common headers to Microsoft VC++ types
#ifdef _MSC_VER
	typedef BYTE	Byte;
	typedef WORD	Word;
	typedef DWORD	DWord;
	typedef DWord	ULong;
	typedef void*	VoidPtr;
	typedef ULong*	ULongPtr;
	typedef Word	UInt;
#endif


//-----------------------------------------------------------
// Platform-independence macros
//-----------------------------------------------------------

#define	HSUtilReverseWord(__theWord__)						\
	(	(Word)															\
		(																	\
		(((Word)(__theWord__) << 8) & 0xFF00) |				\
		(((Word)(__theWord__) >> 8) & 0x00FF)					\
		)																	\
	)

#define	HSUtilReverseDWord(__theDWord__)						\
	(	(DWord)															\
		(																	\
		(((DWord)(__theDWord__) << 24) & 0xFF000000L) |		\
		(((DWord)(__theDWord__) << 8) & 0x00FF0000L) |		\
		(((DWord)(__theDWord__) >> 8) & 0x0000FF00L) |		\
		(((DWord)(__theDWord__) >> 24) & 0x000000FFL)		\
		)																	\
	)

#if ((defined(CPU_TYPE) && CPU_TYPE == CPU_68K) || defined(_MAC) || defined(_68K_) )
	#define	HSUtilPilotToHostWord(__theWord__)				\
			(__theWord__)
	
	#define	HSUtilPilotToHostDWord(__theDWord__)			\
			(__theDWord__)
	
	#define	HSUtilHostToPilotWord(__theWord__)				\
			(__theWord__)
	
	#define	HSUtilHostToPilotDWord(__theDWord__)			\
			(__theDWord__)
			
#else		// CPU_TYPE != CPU_68K
	#define	HSUtilPilotToHostWord(__theWord__)				\
			HSUtilReverseWord(__theWord__)
	
	#define	HSUtilPilotToHostDWord(__theDWord__)			\
			HSUtilReverseDWord(__theDWord__)
	
	#define	HSUtilHostToPilotWord(__theWord__)				\
			HSUtilReverseWord(__theWord__)
	
	#define	HSUtilHostToPilotDWord(__theDWord__)			\
			HSUtilReverseDWord(__theDWord__)
#endif	//CPU_TYPE == CPU_68K



#endif // __HS_UTIL_H__
