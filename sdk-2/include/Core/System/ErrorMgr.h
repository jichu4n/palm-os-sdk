/*******************************************************************
 * 							 Pilot Software
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:
 *		ErrorMgr.h
 *
 * Description:
 *		Include file for Error Management 
 *
 * History:
 *   	10/25/94  RM - Created by Ron Marianetti
 *
 *-----------------------------------------------------------------------
 *	Exception Handling
 *
 *		This unit implements an exception handling mechanism that is similar
 *		to "real" C++ Exceptions. Our Exceptions are untyped, and there
 *		must be one and only one Catch block for each Try block.
 *
 *	Try/Catch Syntax:
 *
 *		ErrTry {
 *			// Do something which may fail.
 *			// Call ErrThrow() to signal failure and force jump
 *			// to the following Catch block.
 *		}
 *
 *		ErrCatch(inErr) {
 *			// Recover or cleanup after a failure in the above Try block.
 *			// "inErr" is an ExceptionCode identifying the reason
 *			// for the failure.
 *			
 *			// You may call Throw() if you want to jump out to
 *			// the next Catch block.
 *
 *			// The code in this Catch block does not execute if
 *			// the above Try block completes without a Throw.
 *
 *		} ErrEndCatch
 *
 *		You must structure your code exactly as above. You can't have a
 *		ErrTry { } without a ErrCatch { } ErrEndCatch, or vice versa.
 *
 *
 *	ErrThrow
 *
 *		To signal failure, call ErrThrow() from within a Try block. The
 *		Throw can occur anywhere in the Try block, even within functions
 *		called from the Try block. A ErrThrow() will jump execution to the
 *		start of the nearest Catch block, even across function calls.
 *		Destructors for stack-based objects which go out of scope as
 *		a result of the ErrThrow() are called.
 *
 *		You can call ErrThrow() from within a Catch block to "rethrow"
 *		the exception to the next nearest Catch block.
 *
 *
 *	Exception Codes
 *
 *		An ExceptionCode is a 32-bit number. You will normally use
 *		Pilot error codes, which are 16-bit numbers. This allows
 *		plently of room for defining codes for your own kinds of errors.
 *
 *
 *	Limitations
 *
 *		Try/Catch and Throw are based on setjmp/longjmp. At the
 *		beginning of a Try block, setjmp saves the machine registers.
 *		Throw calls longjmp, which restores the registers and jumps
 *		to the beginning of the Catch block. Therefore, any changes
 *		in the Try block to variables stored in registers will not
 *		be retained when entering the Catch block. 
 *
 *		The solution is to declare variables that you want to use
 *		in both the Try and Catch blocks as "volatile". For example:
 *
 *		volatile long	x = 1;		// Declare volatile local variable
 *		ErrTry {
 *			x = 100;						// Set local variable in Try
 *			ErrThrow(-1);
 *		}
 *
 *		ErrCatch(inErr) {
 *			if (x > 1) {				// Use local variable in Catch 	
 *				SysBeep(1);
 *			}
 *		} ErrEndCatch
 *
 *
 *******************************************************************/

#ifndef __ERRORMGR_H__
#define __ERRORMGR_H__


// Include elementary types
#include <Common.h>					// Basic types
#include <SysTraps.h>				// Trap Numbers.



#if EMULATION_LEVEL != EMULATION_NONE
#include <setjmp.h>
#endif

/************************************************************
 * Macros for Showing Errors
 *
 ********************************************************************/


//------------------------------------------------------------
// Full Error Checking
//------------------------------------------------------------
#if ERROR_CHECK_LEVEL == ERROR_CHECK_FULL

#define ErrFatalDisplayIf(condition, msg) \
	if (condition) ErrDisplayFileLineMsg(__FILE__, __LINE__, msg)

#define ErrNonFatalDisplayIf(condition, msg) \
	if (condition) ErrDisplayFileLineMsg(__FILE__, __LINE__, msg)

#define ErrDisplay(msg) \
		ErrDisplayFileLineMsg(__FILE__, __LINE__, msg)




//------------------------------------------------------------
// Fatal  Error Checking Only
//------------------------------------------------------------
#elif ERROR_CHECK_LEVEL == ERROR_CHECK_PARTIAL

#define ErrFatalDisplayIf(condition, msg) \
	if (condition) ErrDisplayFileLineMsg(__FILE__, __LINE__, msg)

#define ErrNonFatalDisplayIf(condition, msg) 

#define ErrDisplay(msg) \
		ErrDisplayFileLineMsg(__FILE__, __LINE__, msg)



//------------------------------------------------------------
// No  Error Checking  
//------------------------------------------------------------
#elif ERROR_CHECK_LEVEL == ERROR_CHECK_NONE

#define ErrFatalDisplayIf(condition, msg) 

#define ErrNonFatalDisplayIf(condition, msg) 

#define ErrDisplay(msg)  


//------------------------------------------------------------
// Not Defined...
//------------------------------------------------------------
#else
#error	ERROR: the compiler define 'ERROR_CHECK_LEVEL' must be defined!

#endif // ERROR_CHECK_LEVEL



				
/********************************************************************
 * Try / Catch / Throw support
 *
 * ---------------------------------------------------------------------
 * Exception Handler structure
 *  
 *	An ErrExceptionType object is created for each ErrTry & ErrCatch block.
 *	At any point in the program, there is a linked list of
 *	ErrExceptionType objects. GErrFirstException points to the
 *	most recently entered block. A ErrExceptionType blocks stores
 *	information about the state of the machine (register values)
 *	at the start of the Try block
 ********************************************************************/
typedef long* ErrJumpBuf[12];			// D3-D7,PC,A2-A7
	
#if EMULATION_LEVEL != EMULATION_NONE
	#define	ErrJumpBuf	jmp_buf
#endif
	
// Structure used to store Try state.
typedef struct ErrExceptionType {
	struct ErrExceptionType*	nextP;	// next exception type
	ErrJumpBuf						state;	// setjmp/longjmp storage
	Long								err;		// Error code
	} ErrExceptionType;
typedef ErrExceptionType* ErrExceptionPtr;


// Try & Catch macros
#define	ErrTry																\
	{																				\
		ErrExceptionType	_TryObject;										\
		_TryObject.err = 0;													\
		_TryObject.nextP = (ErrExceptionPtr)*ErrExceptionList();	\
		*ErrExceptionList() = (Ptr)&_TryObject;						\
		if (ErrSetJump(_TryObject.state) == 0) {
		

#define	ErrCatch(theErr)												\
			*ErrExceptionList() = (Ptr)_TryObject.nextP;			\
			} 																	\
		else {																\
			Long	theErr = _TryObject.err;							\
			*ErrExceptionList() = (Ptr)_TryObject.nextP;
			
			
#define	ErrEndCatch														\
			}																	\
	}



/********************************************************************
 * Error Manager Routines
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#if EMULATION_LEVEL != EMULATION_NONE
	#define	ErrSetJump(buf)			setjmp(buf)
	#define	ErrLongJump(buf,res) 	longjmp(buf,res)

#else
	Int	ErrSetJump(ErrJumpBuf buf)
								SYS_TRAP(sysTrapErrSetJump);
								
	void	ErrLongJump(ErrJumpBuf buf, Int result)
								SYS_TRAP(sysTrapErrLongJump);
#endif
						
Ptr*	ErrExceptionList(void)
							SYS_TRAP(sysTrapErrExceptionList);	
							
void  ErrThrow(Long err) 
							SYS_TRAP(sysTrapErrThrow);

void	ErrDisplayFileLineMsg(CharPtr filename, UInt lineno, CharPtr msg)
							SYS_TRAP(sysTrapErrDisplayFileLineMsg);

#ifdef __cplusplus 
}
#endif




#endif // __ERRORMGR_H__




