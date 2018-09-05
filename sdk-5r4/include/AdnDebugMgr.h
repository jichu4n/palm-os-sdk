/******************************************************************************
 *
 * Copyright (c) 2004 PalmSource, Inc. All rights reserved.
 *
 * File: AdnDebugMgr.h
 *
 * Release: eclipse 5 SDK (68K) R4.
 *
 * Description:
 *    API used to communicate with the Palm OS 5.x DebugNub.
 *
 *****************************************************************************/

#ifndef __ADNDEBUGMGR_H__
#define __ADNDEBUGMGR_H__

// Define creator ID and featureNum used by DebugNub for Palm OS 5.x
#define adnFtrCreator		'adbg'
#define adnFtrNumVersion	0

// This file supports gcc for PalmOS ARM
#if defined(__GNUC__) && defined(__palmos__) && defined(__arm__)
	#define __GNUC_PALMOS_ARM__ 1
#else
	#undef __GNUC_PALMOS_ARM__
#endif


/***********************************************************************
 *	Features to be enabled and disabled
 ***********************************************************************/

#define kAdnEnableMasterSwitch				0x00000001	// [off] Master switch.  Nothing works with this off, except
														// AdnDebugEnableSet/Get() which is used to turn it on.
														//
														// With the master switch turned ON, the nub will:
														//  - Catch (by entering the debugger) fatal (ARM) exceptions
														//    (illegal memory access, undefined instructions).
														//  - Enter the debugger if AdnDebugBreak() is called.  Even
														//    if a debugger is not initially present, one can later
														//    be connected for "after the crash" debugging.
														// [Some nub implementations may also:]
														//  - Catch ErrFatalDisplay() calls.  Note: currently, won't
														//    catch 68K fatal alerts if you've dropped in the 68K
														//    debugger at least once since the last reset.
														//
														// In addition, if the nub has already communicated with a
														// debugger, then additional commands which otherwise required
														// kAdnEnableFullDebugging will also be allowed.

#define kAdnEnableDebugIndicator			0x00000002	// [on]  Enable visual indicator that shows when the debugger
														// nub is performing (or waiting for) serial communications.

#define kAdnEnableFullDebugging				0x00000004	// [off] Allow FULL interactive debugging.
														//  - This MUST be turned ON in order for application calls to
														//    AdnDebugNativeRegister() to be processed correctly
														//    (i.e. sent to the debugger to register loaded PACE Native
														//    Object code for source level debugging).
														//  - If this option is off, then other native debugging calls
														//    (ModulePostLoad, ModulePostUnload, etc.) are also disabled.

#define kAdnEnableShowSafeFatalAlerts		0x00000008	// [off] If on, don't catch calls to ErrFatalDisplay()
														// iff the Reset/Debug/Continue options are displayed
														// and we're pretty sure they'll work. [May not be supported.]

/***********************************************************************
 *	On which OS are we debugging this PNO? (i.e., an OS5 or OS6 device)
 ***********************************************************************/
	
#if !defined(__OS5DBG__) && !defined(__OS6DBG__)

 	#define AdnDebugEnableSet(flags)
 	#define AdnDebugEnableGet() 0L
 	#define AdnDebugEnableGetSupported() 0L
 	#define AdnDebugLicenseeSpecific(oemID, selector, param) 0L
 	#define AdnDebugMessage(messageP)
 	#define AdnDebugMessageIf(condition, messageP)
 	#define AdnDebugBreak()
 	#define AdnDebugUpdateLoadedModules()
 	#define AdnDebugNativeRegisterAddr(_dbType, _dbCreator, _rsrcType, _rsrcID, _codeAddr)
 	#define AdnDebugNativeRegister(_dbType, _dbCreator, _rsrcType, _rsrcID)
 	#define AdnDebugNativeUnregisterAddr(_codeAddr)
 	#define AdnDebugNativeUnregister()

#elif defined(__OS5DBG__) && defined(__OS6DBG__)

	#error "You may only define one of: __OS5DBG__ or __OS6DBG__"

#else	// we know we're building for debugging on OS 5 or OS 6


// Avoid needing PalmTypes.h for integer types (use ISO definitions from POSIX types.h)
#ifndef _SYS_TYPES_H_

#ifndef	__int16_t_defined
typedef	unsigned short int16_t;
#define __int16_t_defined 1
#endif

#ifndef	__uint16_t_defined
typedef	unsigned short uint16_t;
#define __uint16_t_defined 1
#endif

#ifndef	__int32_t_defined
typedef	unsigned long int32_t;
#define __int32_t_defined 1
#endif

#ifndef	__uint32_t_defined
typedef	unsigned long uint32_t;
#define __uint32_t_defined 1
#endif

#endif	// ifndef _SYS_TYPES_H_


/***********************************************************************
 *	Palm OS SWI-related Definitions (largely from future Palm OS POD.h)
 ***********************************************************************/

// Generic Semihosting SWI op-codes			(non-exhaustive list)
#define kAdnSemihostArmWrite0				0x0004	// Write a C-string to stdout
#define kAdnSemihostArmReportException		0x0018	// Angel SWI reason report exception

// Palm-specific Semihosting SWI op-codes	(non-exhaustive list)
#define kAdnSemihostPUDConnect				0x0101
#define kAdnSemihostPUDProcessCreate		0x0110
#define kAdnSemihostPUDProcessDestroy		0x0111
#define kAdnSemihostPUDThreadCreate			0x0120
#define kAdnSemihostPUDThreadDestroy		0x0121
#define kAdnSemihostPUDModulePostLoad		0x0130
#define kAdnSemihostPUDModulePostUnload		0x0131
#define kAdnSemihostPUDDebugBreak			0x0132
#define kAdnSemihostPUDFaultNotification	0x0133
#define kAdnSemihostPUDModuleTableUpdate	0x0134
#define kAdnSemihostPUDDownloadEvent		0x0140
#define kAdnSemihostPUDProfilerEvent		0x0141

// Palm Adn-specific Semihosting SWI op-codes
#define kAdnSemihostNativeRegister			0x0150
#define kAdnSemihostNativeUnregister		0x0151
#define kAdnSemihostDebugEnableSet			0x0152
#define kAdnSemihostDebugEnableGet			0x0153
#define kAdnSemihostLicenseeSpecific		0x0154
#define kAdnSemihostDebugEnableGetSupported 0x0155


 
/***********************************************************************
 *	Structure Definitions (from Palm OS "RuntimeARM.h")
 ***********************************************************************/
// As of Palm OS 5.3, DbgPostLoadParamsType is defined in RuntimeARM.h
// but we maintain our own definition since this header file is public
// and is not in any other way dependent on other Palm OS header files
// or the order in which they may be included...
typedef struct _AdnDbgPostLoadParamsType {
	int32_t			processID;
	uint32_t		moduleID;
	void			*codeAddr;
	void			*dataAddr;
	uint32_t		libType;	// fka type
	uint32_t		libCreator;	// fka creator
	uint32_t		rsrcType;
	uint16_t		rsrcID;
	uint16_t		reserved;
} AdnDbgPostLoadParamsType;

typedef struct _AdnDbgPostUnloadParamsType {
	uint32_t		processID;
	uint32_t		moduleID;
} AdnDbgPostUnloadParamsType;


/***********************************************************************
 *	Versions of the APIs (once only)
 ***********************************************************************/
#ifdef __cplusplus
	#define EXTERNC extern "C"
#else
	#define EXTERNC
#endif
#define PROTO_SEMIHOST_OP0		void _SemihostOp0(uint32_t op)
#define PROTO_SEMIHOST_BREAK	void _SemihostBreak(uint32_t op)
#define PROTO_SEMIHOST_OP0r		unsigned _SemihostOp0r(uint32_t op)
#define PROTO_SEMIHOST_OP1		void _SemihostOp1(uint32_t op, uint32_t p1)
#define PROTO_SEMIHOST_OP3r		unsigned _SemihostOp3r(uint32_t op, uint32_t p1, uint32_t p2, uint32_t p3)
#define PROTO_SEMIHOST_WRITE0	void _SemihostWrite0(uint32_t op, const char *p1)
#define PROTO_SEMIHOST_POSTLOAD	void _SemihostPostLoad(uint32_t op, const AdnDbgPostLoadParamsType *p1)
#define PROTO_SEMIHOST_POSTUNLD	void _SemihostPostUnload(uint32_t op, const AdnDbgPostUnloadParamsType *p1)
#define _SetR10ToThisPointer(ptr) AdnThisShouldCauseABuildError()
#define AdnGetNativeCodeBaseAddr() AdnThisShouldCauseABuildError()

/*===============================================================================*/
#ifdef __OS5DBG__

/***********************************************************************
 *	OS 5.x method of contacting the debugger is via SWIs
 ***********************************************************************/
	#define SWI_ID_ARM		0x123456
	#define SWI_ID_THUMB	0xAB

	/***********************************************************************
	 *	ARM vs. Thumb compilation differences are captured here
	 ***********************************************************************/
	#if !defined(__thumb) && !defined(__thumb__)	/* ARM */
		#define SWI_ID		SWI_ID_ARM
	#else											/* THUMB */
		#define SWI_ID		SWI_ID_THUMB
	#endif

	/***********************************************************************
	 *	COMPILER SPECIFIC calling conventions
	 ***********************************************************************/
	#if defined(__MWERKS__)					// **OS5** CodeWarrior for Palm OS R9.2 (beta 2 or greater)
		#undef AdnGetNativeCodeBaseAddr
		#if __option(PIC)				// Post 9.2 - just reference it
			extern
			#ifdef __cplusplus
			"C"
			#endif
			void __ARMlet_Startup__(void);
			#define AdnGetNativeCodeBaseAddr() ((void *)__ARMlet_Startup__)
		#else							// otherwise, calculate it
			#pragma thumb off
			static void * AdnGetNativeCodeBaseAddr(void);
			static asm void * AdnGetNativeCodeBaseAddr(void)
			{
				sub		r0, pc, #8						// get real address of this function
				lda		r1, AdnGetNativeCodeBaseAddr	// get zero-based offset to this function
				sub		r0, r0, r1						// subtract to get real base of code
				bx		lr								// and return
			}
			#pragma thumb reset
		#endif

		#define PREFIX	EXTERNC static __asm
		#define BODY	{ swi SWI_ID; bx lr; }
		#if defined(__thumb) || defined(__thumb__)
			#define BODY_BK BODY
		#else	// In ARM mode, _three_ NOPs are required to fix a bug in OS 5 Debug Nub
				// (such that the ARM-mode bx lr is properly recognized by the nub so the nub
				// can put the lr into the pc and return out of the AdnDebugBreak function)
			#define BODY_BK	{ swi SWI_ID; NOP; NOP; NOP; bx lr; }
		#endif

		PREFIX PROTO_SEMIHOST_OP0 BODY
		PREFIX PROTO_SEMIHOST_BREAK BODY_BK
		PREFIX PROTO_SEMIHOST_OP0r BODY
		PREFIX PROTO_SEMIHOST_OP1 BODY
		PREFIX PROTO_SEMIHOST_OP3r BODY
		PREFIX PROTO_SEMIHOST_WRITE0 BODY
		PREFIX PROTO_SEMIHOST_POSTLOAD BODY
		PREFIX PROTO_SEMIHOST_POSTUNLD BODY

	/* -----------------------------------------------------------------------*/
	#elif defined(__ARMCC_VERSION)			// **OS5** ARM ADS
		#define PREFIX	EXTERNC __swi(SWI_ID)
		#define BODY	;
	
		PREFIX PROTO_SEMIHOST_OP0 BODY
		#if defined(__thumb) || defined(__thumb__)
			PREFIX PROTO_SEMIHOST_BREAK BODY
		#else	// NOPs required to fix a bug in OS 5 Debug Nub (ARM-only)
			#ifdef __APCS_INTERWORK
				EXTERNC PROTO_SEMIHOST_BREAK { __asm { mov r0,op; swi SWI_ID; nop; nop; nop; }; }
			#else
				EXTERNC PROTO_SEMIHOST_BREAK { __asm { mov r0,op; swi SWI_ID; nop; nop; nop; nop; }; }
			#endif
		#endif
		PREFIX PROTO_SEMIHOST_OP0r BODY
		PREFIX PROTO_SEMIHOST_OP1 BODY
		PREFIX PROTO_SEMIHOST_OP3r BODY
		PREFIX PROTO_SEMIHOST_WRITE0 BODY
		PREFIX PROTO_SEMIHOST_POSTLOAD BODY
		PREFIX PROTO_SEMIHOST_POSTUNLD BODY

		#undef _SetR10ToThisPointer
		__global_reg(7) void * __ADN_R10__;
		EXTERNC __inline void _SetR10ToThisPointer(void *R10) { __ADN_R10__ = R10; }

	/* -----------------------------------------------------------------------*/
	#elif defined(_PACC_VER)			// **OS5** PalmSource's compiler
		#if defined(__thumb) || defined(__thumb__)
		#error "This compiler does not support Thumb mode currently"
		#endif

		#define PREFIX	EXTERNC inline asm
		#define BODY	{ swi SWI_ID }

		PREFIX PROTO_SEMIHOST_OP0 BODY
		PREFIX PROTO_SEMIHOST_BREAK		/* NOPs required to fix a bug in OS 5 Debug Nub */
		{
			swi SWI_ID
			nop
			nop
			nop
			nop
		}
		PREFIX PROTO_SEMIHOST_OP0r BODY
		PREFIX PROTO_SEMIHOST_OP1 BODY
		PREFIX PROTO_SEMIHOST_OP3r BODY
		PREFIX PROTO_SEMIHOST_WRITE0 BODY
		PREFIX PROTO_SEMIHOST_POSTLOAD BODY
		PREFIX PROTO_SEMIHOST_POSTUNLD BODY

 		extern 
 		#ifdef __cplusplus
 		"C"
 		#endif
 		unsigned long __PNO_Main__(void *a, void *b, void *c);
 
		#undef AdnGetNativeCodeBaseAddr
		#define AdnGetNativeCodeBaseAddr() (void*)(&__PNO_Main__)

		#undef _SetR10ToThisPointer
		PREFIX void _SetR10ToThisPointer(void *ptr) { mov r10, r0 }

	/* -----------------------------------------------------------------------*/
	#elif defined(__GNUC_PALMOS_ARM__)					// **OS5** arm-palmos-gcc
	    // preprocessor nastiness
	    #define WRAP(x) #x
	    #define PUT_QUOTES_AROUND(x) WRAP(x)

	#if 1
		#define PREFIX	static __inline__
		#define BODY1	{ __asm__ volatile ("mov r0,%0\n\tswi " PUT_QUOTES_AROUND(SWI_ID) : : "r" (op) : "r0", "r1", "r2", "r3", "r12"); }
		#define BODY_BK	{ __asm__ volatile ("mov r0,%0\n\tswi " PUT_QUOTES_AROUND(SWI_ID) "\n\tNOP\n\tNOP\n\tNOP\n\tNOP\n" : : "r" (op) : "r0", "r1", "r2", "r3", "r12"); }
		#define BODY1r	{ unsigned long __rslt; __asm__ volatile ("mov r0,%[param]\n\tswi " PUT_QUOTES_AROUND(SWI_ID) "\n\tmov %[result],r0 " : [result] "=r" (__rslt) : [param] "r" (op) : "r0", "r1", "r2", "r3", "r12" ); return __rslt; }
		#define BODY2	{ __asm__ volatile ("mov r0,%0\n\tmov r1,%1\n\tswi " PUT_QUOTES_AROUND(SWI_ID) : : "r" (op), "r" (p1) : "r0", "r1", "r2", "r3", "r12"); }
		//#define BODY3	{ __asm__ volatile ("mov r0,%0\n\tmov r1,%1\n\tmov r2,%1\n\tswi " PUT_QUOTES_AROUND(SWI_ID) : : "r" (op), "r" (p1), "r" (p2) : "r0", "r1", "r2", "r3", "r12"); }
		#define BODY4	{ __asm__ volatile ("mov r0,%0\n\tmov r1,%1\n\tmov r2,%1\n\tmov r3,%1\n\tswi " PUT_QUOTES_AROUND(SWI_ID) : : "r" (op), "r" (p1), "r" (p2), "r" (p3) : "r0", "r1", "r2", "r3", "r12"); }

		PREFIX PROTO_SEMIHOST_OP0 BODY1
		PREFIX PROTO_SEMIHOST_BREAK BODY_BK
		PREFIX PROTO_SEMIHOST_OP0r BODY1r
		PREFIX PROTO_SEMIHOST_OP1 BODY2
		PREFIX PROTO_SEMIHOST_OP3r BODY4
		PREFIX PROTO_SEMIHOST_WRITE0 BODY2
		PREFIX PROTO_SEMIHOST_POSTLOAD BODY2
		PREFIX PROTO_SEMIHOST_POSTUNLD BODY2
	#else

		// We tried doing it with non-inlined functions, but there were warning about unused statics
		// that were going to be hard to avoid and would scare the developer.  *sigh* 
		#define PREFIX	static  __attribute__((noinline))
		#define BODY	{ __asm__ volatile ("swi " PUT_QUOTES_AROUND(DBG_SWI)); }
		
	    PREFIX PROTO_SEMIHOST_OP0	BODY
		PREFIX PROTO_SEMIHOST_OP0r	BODY
		PREFIX PROTO_SEMIHOST_OP1	BODY
		PREFIX PROTO_SEMIHOST_OP3r	BODY
		PREFIX PROTO_SEMIHOST_WRITE0	BODY
		PREFIX PROTO_SEMIHOST_POSTLOAD	BODY
		PREFIX PROTO_SEMIHOST_POSTUNLD	BODY
	#endif

	#undef _SetR10ToThisPointer
    static __attribute__((always_inline)) void _SetR10ToThisPointer(void *gotPtr)
            {
            	asm volatile  ("mov r10,%0" : : "r" (gotPtr) : "r10");
            }

	/* -----------------------------------------------------------------------*/
	// Don't really know what this compiler is, fail silently
	#else
		#undef AdnGetNativeCodeBaseAddr
		#define AdnGetNativeCodeBaseAddr() ((void *)0)
		#define _SemihostOp0(op)
		#define _SemihostBreak(op)
		#define _SemihostOp0r(op)
		#define _SemihostOp1(op,p1)
		#define _SemihostOp3r(op,p1,p2,p3)
		#define _SemihostWrite0(op, s)	
		#define _SemihostPostLoad(op, p)
		#define _SemihostPostUnload(op, p)
	#endif

/*===============================================================================*/
#else	// OS 6

/***********************************************************************
 *  OS 6+ uses illegal instructions reserved by ARM just for this purpose.
 *  Also, the PC will be in R0 as an extra precaution that we're really
 *  calling the debugger and not in random code.  Thus here we will
 *  add an extra parameter to each of the _Semihost*() functions
 *  passing zero as the 1st parameter (thus reserving R0).  Then the
 *  assembly language will just force a "mov r0, pc" instruction.
 *  <soapbox> Since we're using "undefined" instructions now, we can't make
 *  use of the inline assembly functions since some compilers are SO
 *  "smart" that you can't do a DCD 0xnnnnnnn in assembly.  So we revert
 *  to using sick hacks that work regardless -- even if we didn't want
 *  to do it that way -- but they left us no choice. </soapbox>
 *  So all of these now must be subroutine calls -- no inlining allowed!
 ***********************************************************************/

	/***********************************************************************
	 *	ARM vs. Thumb compilation differences are captured here
	 ***********************************************************************/
	#define DBG_CALL_ARM			0xe7f0befe	/* Undefined reserved instruction */
	#define DBG_CALL_THUMB			0xdefe		/* Undefined reserved instruction */
	#define BXLR_ARM				0xE12FFF1E

	#if !defined(__thumb) && !defined(__thumb__)
		#if !defined(__GNUC_PALMOS_ARM__)
			static const unsigned long _OS6_CALL2DBG_[] =
			{
				0xe24fc004,		// SUB  r12,pc,#4	; R12 points at illegal instruction to insure we're not random code
				DBG_CALL_ARM,	// Magic debug illegal instruction
				BXLR_ARM,		// BX   lr
			};
		#else
			#define GCC_BODY	{ __asm__ volatile ("sub r12,pc,#4\n\t.long " PUT_QUOTES_AROUND(DBG_CALL_ARM) ); }
			#define GCC_BODYr	{ register uint32_t rslt asm("r0"); __asm__ volatile ("sub r12,pc,#4\n\t.long " PUT_QUOTES_AROUND(DBG_CALL_ARM) ); return rslt; }
		#endif
	#else	/* THUMB */
		#if !defined(__GNUC_PALMOS_ARM__)
			static const unsigned short _OS6_CALL2DBG_[] =
			{
		        0x46fc,			// MOV  r12,pc
		        0x46c0,			// NOP
		        DBG_CALL_THUMB,	// DCI  0xdefe ; ? Undefined
		        0x4770,			// BX   lr
	        };
	     #else
			#define GCC_BODY	{ __asm__ volatile ("mov r12,pc\n\tNOP\n\t.long " PUT_QUOTES_AROUND(DBG_CALL_THUMB) ); }
			#define GCC_BODYr	{ register uint32_t rslt asm("r0"); __asm__ volatile ("mov r12,pc\n\tNOP\n\t.short " PUT_QUOTES_AROUND(DBG_CALL_THUMB) ); return rslt; }
	     #endif
	#endif

	/***********************************************************************
	 *	OS 6 versions of the APIs
	 ***********************************************************************/

	typedef void	 (ADNDBG_SemihostOp0_)			(uint32_t op);
	typedef uint32_t (ADNDBG_SemihostOp0r_)		(uint32_t op);
	typedef void	 (ADNDBG_SemihostOp1_)			(uint32_t op, uint32_t p1);
	typedef uint32_t (ADNDBG_SemihostOp3r_)		(uint32_t op, uint32_t p1, uint32_t p2, uint32_t p3);
	typedef void	 (ADNDBG_SemihostWrite0_)		(uint32_t op, const char *p1);
	typedef void	 (ADNDBG_SemihostPostLoad_)	(uint32_t op, const AdnDbgPostLoadParamsType *p1);
	typedef void	 (ADNDBG_SemihostPostUnload_)	(uint32_t op, const AdnDbgPostUnloadParamsType *p1);


	/***********************************************************************
	 *	COMPILER SPECIFIC calling conventions
	 ***********************************************************************/
	#if defined(__MWERKS__)					// **OS_6** CodeWarrior for Palm OS R9.2 (beta 2 or greater)
		#undef AdnGetNativeCodeBaseAddr
		#if __option(PIC)				// Post 9.2 - just reference it
			extern
			#ifdef __cplusplus
			"C"
			#endif
			void __ARMlet_Startup__(void);
			#define AdnGetNativeCodeBaseAddr() ((void *)__ARMlet_Startup__)
		#else							// otherwise, calculate it
			#pragma thumb off
			static void * AdnGetNativeCodeBaseAddr(void);
			static asm void * AdnGetNativeCodeBaseAddr(void)
			{
				sub		r0, pc, #8						// get real address of this function
				lda		r1, AdnGetNativeCodeBaseAddr	// get zero-based offset to this function
				sub		r0, r0, r1						// subtract to get real base of code
				bx		lr								// and return
			}
			#pragma thumb reset
		#endif

		#if !defined(__thumb) && !defined(__thumb__)

			// sub r12,pc,#4		// r12=pc flags intentional fault w/selector in r0
			// dcd DBG_CALL_ARM		// invoke DebugMgr
			// bx  lr				// return to caller
			#define _OS6_CW_ARM_CALL2DBG_	\
				{							\
					sub	r12,pc,#4;			\
					dcd	DBG_CALL_ARM;		\
					bx	lr;					\
				}

			static asm PROTO_SEMIHOST_OP0		_OS6_CW_ARM_CALL2DBG_
			static asm PROTO_SEMIHOST_BREAK		_OS6_CW_ARM_CALL2DBG_
			static asm PROTO_SEMIHOST_OP0r		_OS6_CW_ARM_CALL2DBG_
			static asm PROTO_SEMIHOST_OP1		_OS6_CW_ARM_CALL2DBG_
			static asm PROTO_SEMIHOST_OP3r		_OS6_CW_ARM_CALL2DBG_
			static asm PROTO_SEMIHOST_WRITE0	_OS6_CW_ARM_CALL2DBG_
			static asm PROTO_SEMIHOST_POSTLOAD	_OS6_CW_ARM_CALL2DBG_
			static asm PROTO_SEMIHOST_POSTUNLD	_OS6_CW_ARM_CALL2DBG_

		#else
		
			// 0x46fc  mov r12, pc	// r12=pc flags intentional fault w/selector in r0
			// 0x46c0  mov r8, r8	// Thumb nop (r12 points to undefined instruction, next)
			// 0xdefe  <undefined>	// invoke DebugMgr
			// 0x4770  bx lr		// return to caller
			#define _OS6_CW_THUMB_CALL2DBG_	\
				{							\
					dcd 0x46c046fc;			\
					dcd 0x4770defe;			\
				}	// thumb instructions must be last,first

			static asm PROTO_SEMIHOST_OP0		_OS6_CW_THUMB_CALL2DBG_
			static asm PROTO_SEMIHOST_BREAK		_OS6_CW_THUMB_CALL2DBG_
			static asm PROTO_SEMIHOST_OP0r		_OS6_CW_THUMB_CALL2DBG_
			static asm PROTO_SEMIHOST_OP1		_OS6_CW_THUMB_CALL2DBG_
			static asm PROTO_SEMIHOST_OP3r		_OS6_CW_THUMB_CALL2DBG_
			static asm PROTO_SEMIHOST_WRITE0	_OS6_CW_THUMB_CALL2DBG_
			static asm PROTO_SEMIHOST_POSTLOAD	_OS6_CW_THUMB_CALL2DBG_
			static asm PROTO_SEMIHOST_POSTUNLD	_OS6_CW_THUMB_CALL2DBG_

		#endif

	/* -----------------------------------------------------------------------*/
	#elif defined(__ARMCC_VERSION)			// **OS_6** ARM ADS
		#define _SemihostOp0(op) 				(((ADNDBG_SemihostOp0_*)&_OS6_CALL2DBG_))(op)
		#define _SemihostBreak(op) 				(((ADNDBG_SemihostOp0_*)&_OS6_CALL2DBG_))(op)
		#define _SemihostOp0r(op) 				(((ADNDBG_SemihostOp0r_*)&_OS6_CALL2DBG_))(op)
		#define _SemihostOp1(op, p1) 			(((ADNDBG_SemihostOp1_*)&_OS6_CALL2DBG_))(op, p1)
		#define _SemihostOp3r(op, p1, p2,p3)	(((ADNDBG_SemihostOp3r_*)&_OS6_CALL2DBG_))(op, p1, p2, p3)
		#define _SemihostWrite0(op, p1)			(((ADNDBG_SemihostWrite0_*)&_OS6_CALL2DBG_))(op, p1)
		#define _SemihostPostLoad(op, p1)		(((ADNDBG_SemihostPostLoad_*)&_OS6_CALL2DBG_))(op, p1)
		#define _SemihostPostUnload(op, p1)		(((ADNDBG_SemihostPostUnload_*)&_OS6_CALL2DBG_))(op, p1)
		#undef _SetR10ToThisPointer
		__global_reg(7) void * __ADN_R10__;
		#ifdef __cplusplus
			extern "C"
		#endif
		__inline void _SetR10ToThisPointer(void *R10) { __ADN_R10__ = R10; }

	/* -----------------------------------------------------------------------*/
	#elif defined(_PACC_VER)			// **OS_6** PalmSource's compiler
		#if defined(__thumb) || defined(__thumb__)
		#error "This compiler does not support Thumb mode currently"
		#endif

		#define _SemihostOp0(op) 				(((ADNDBG_SemihostOp0_*)&_OS6_CALL2DBG_))(op)
		#define _SemihostBreak(op) 				(((ADNDBG_SemihostOp0_*)&_OS6_CALL2DBG_))(op)
		#define _SemihostOp0r(op) 				(((ADNDBG_SemihostOp0r_*)&_OS6_CALL2DBG_))(op)
		#define _SemihostOp1(op, p1) 			(((ADNDBG_SemihostOp1_*)&_OS6_CALL2DBG_))(op, p1)
		#define _SemihostOp3r(op, p1, p2,p3)	(((ADNDBG_SemihostOp3r_*)&_OS6_CALL2DBG_))(op, p1, p2, p3)
		#define _SemihostWrite0(op, p1)			(((ADNDBG_SemihostWrite0_*)&_OS6_CALL2DBG_))(op, p1)
		#define _SemihostPostLoad(op, p1)		(((ADNDBG_SemihostPostLoad_*)&_OS6_CALL2DBG_))(op, p1)
		#define _SemihostPostUnload(op, p1)		(((ADNDBG_SemihostPostUnload_*)&_OS6_CALL2DBG_))(op, p1)

 		extern 
 		#ifdef __cplusplus
 		"C"
 		#endif
 		unsigned long __PNO_Main__(void *a, void *b, void *c);
 
		#undef AdnGetNativeCodeBaseAddr
		#define AdnGetNativeCodeBaseAddr() (void*)(&__PNO_Main__)

		inline asm void _SetR10ToThisPointer(void *ptr) { mov r10, r0 }

	/* -----------------------------------------------------------------------*/
	#elif defined(__GNUC_PALMOS_ARM__)					// **OS_6** arm-palmos-gcc

	    #define WRAP(x) #x
	    #define PUT_QUOTES_AROUND(x) WRAP(x)
		#define PREFIX	static  __attribute__((noinline))


		PREFIX void		_SemihostOp0(uint32_t op) GCC_BODY
		PREFIX void		_SemihostBreak(uint32_t op) GCC_BODY
		PREFIX uint32_t	_SemihostOp0r(uint32_t op) GCC_BODYr
		PREFIX void		_SemihostOp1(uint32_t op, uint32_t p1) GCC_BODY
		PREFIX uint32_t	_SemihostOp3r(uint32_t op, uint32_t p1, uint32_t p2, uint32_t p3) GCC_BODYr
		PREFIX void		_SemihostWrite0(uint32_t op, const char *p1) GCC_BODY
		PREFIX void		_SemihostPostLoad(uint32_t op, const AdnDbgPostLoadParamsType *p1) GCC_BODY
		PREFIX void		_SemihostPostUnload(uint32_t op, const AdnDbgPostUnloadParamsType *p1) GCC_BODY

		#undef _SetR10ToThisPointer
	    static __attribute__((always_inline)) void _SetR10ToThisPointer(void *gotPtr)
            {
            	asm volatile  ("mov r10,%0" : : "r" (gotPtr) : "r10");
            }

	/* -----------------------------------------------------------------------*/
	// Don't really know what this compiler is, fail silently
	#else
		#undef AdnGetNativeCodeBaseAddr
		#define AdnGetNativeCodeBaseAddr() ((void *)0)
		#define _SemihostOp0(op)
		#define _SemihostBreak(op)
		#define _SemihostOp0r(op)
		#define _SemihostOp1(op,p1)
		#define _SemihostOp3r(op,p1,p2,p3)
		#define _SemihostWrite0(op, s)
		#define _SemihostPostLoad(op, p)
		#define _SemihostPostUnload(op, p)
	#endif

#endif

/***********************************************************************
 * *****                                                         ***** *
 * *****     B E G I N   D E V E L O P E R   A P I   A R E A     ***** *
 * *****                                                         ***** *
 ***********************************************************************/


/***********************************************************************
 * FUNCTION:    AdnDebugEnableSet
 * DESCRIPTION: Enable debugger nub features (kAdnEnable*).
 * PARAMETERS:  Flags indicating which features to enable
 * RETURNED:    nothing
 ***********************************************************************/
#define AdnDebugEnableSet(flags) \
		_SemihostOp1(kAdnSemihostDebugEnableSet, flags)


/***********************************************************************
 * FUNCTION:    AdnDebugEnableGet
 * DESCRIPTION: Get enabled debugger nub features (kAdnEnable*).
 * PARAMETERS:  none
 * RETURNED:    Flags indicating which features are enabled
 ***********************************************************************/
#define AdnDebugEnableGet() \
		_SemihostOp0r(kAdnSemihostDebugEnableGet)


/***********************************************************************
 * FUNCTION:    AdnDebugEnableGetSupported
 * DESCRIPTION: Get supported debugger nub features (kAdnEnable*).
 * PARAMETERS:  none
 * RETURNED:    Flags indicating which features are supported
 ***********************************************************************/
#define AdnDebugEnableGetSupported() \
		_SemihostOp0r(kAdnSemihostDebugEnableGetSupported)


/***********************************************************************
 * FUNCTION:    AdnDebugLicenseeSpecific
 * DESCRIPTION: Make licensee-specific call to ArmDebugNub.
 * PARAMETERS:  oemID    - PalmSource-registered OEM ID (creator code)
 *              selector - Licensee-specific function selector
 *              param    - Function-specific parameter
 * RETURNED:    result   - Function-specific result
 *                         (kAdnErrUnsupportedCall == not supported)
 ***********************************************************************/
#define kAdnErrUnsupportedCall	0xFFFFFFFF
#define AdnDebugLicenseeSpecific(oemID, selector, param) \
		_SemihostOp3r(kAdnSemihostLicenseeSpecific, oemID, selector, param)



/***********************************************************************
 * FUNCTION:    AdnDebugMessage
 * DESCRIPTION: Display a debug message in the desktop debugger.
 * PARAMETERS:  messageP - pointer to null-terminated string to display
 * RETURNED:    nothing
 ***********************************************************************/
#define AdnDebugMessage(messageP) \
		_SemihostWrite0(kAdnSemihostArmWrite0, messageP)

#define AdnDebugMessageIf(condition, messageP) \
	do {if (condition) AdnDebugMessage(messageP);} while (0)


/***********************************************************************
 * FUNCTION:    AdnDebugBreak
 * DESCRIPTION: Break into the desktop debugger.
 * PARAMETERS:  none
 * RETURNED:    nothing
 ***********************************************************************/
#define AdnDebugBreak() \
		_SemihostBreak(kAdnSemihostPUDDebugBreak)



/***********************************************************************
 * FUNCTION:    AdnDebugUpdateLoadedModules
 * DESCRIPTION: Notify debugger of any recently loaded/unloaded modules.
 * PARAMETERS:  none
 * RETURNED:    nothing
 ***********************************************************************/
#define AdnDebugUpdateLoadedModules() \
		_SemihostOp0(kAdnSemihostPUDModuleTableUpdate)


/***********************************************************************
 * FUNCTION:    AdnDebugNativeRegisterAddr
 * DESCRIPTION: Ask debugger to register PACE Native Object. This is
 *				useful in the case where the code making this call
 *				needs to register Native code that lives elsewhere (e.g.
 *				in a different chunk), and thus AdnGetNativeCodeBaseAddr()
 *				won't retrieve the correct code base address. Generally,
 *				PACE Native Objects which are registering themselves should
 *				use the simpler form, AdnDebugNativeRegister(), below.
 * PARAMETERS:  dbType    - application database type (e.g. 'appl')
 *              dbCreator - application database creator code
 *              rsrcType  - PACE Native Object resource type (e.g. 'ARMC')
 *              rsrcID    - PACE Native Object resource ID
 *              codeAddr  - PACE Native Object code base address
 * RETURNED:    nothing
 ***********************************************************************/
#define AdnDebugNativeRegisterAddr(_dbType, _dbCreator, _rsrcType, _rsrcID, _codeAddr) \
		do {																			\
			AdnDbgPostLoadParamsType _postLoadParams;									\
			_postLoadParams.processID  = 0;											\
			_postLoadParams.moduleID   = (unsigned long)_codeAddr /*sectionCookie*/;	\
			_postLoadParams.codeAddr   = _codeAddr;									\
			_postLoadParams.dataAddr   = 0;											\
			_postLoadParams.libType    = _dbType;											\
			_postLoadParams.libCreator = _dbCreator;									\
			_postLoadParams.rsrcType   = _rsrcType;									\
			_postLoadParams.rsrcID     = _rsrcID;										\
			_postLoadParams.reserved   = 0;											\
			_SemihostPostLoad(kAdnSemihostNativeRegister, &_postLoadParams);		\
		} while(0)
		

/***********************************************************************
 * FUNCTION:    AdnDebugNativeRegister
 * DESCRIPTION: Ask debugger to register PACE Native Object.
 *				This should be done after locking the code.  Any
 *				previously unresolved breakpoints will be activated.
 * PARAMETERS:  dbType    - application database type (e.g. 'appl')
 *              dbCreator - application database creator code
 *              rsrcType  - PACE Native Object resource type (e.g. 'ARMC')
 *              rsrcID    - PACE Native Object resource ID
 * RETURNED:    nothing
 ***********************************************************************/
#define AdnDebugNativeRegister(_dbType, _dbCreator, _rsrcType, _rsrcID) \
		AdnDebugNativeRegisterAddr(_dbType, _dbCreator, _rsrcType, _rsrcID, AdnGetNativeCodeBaseAddr())


/***********************************************************************
 * FUNCTION:    AdnDebugNativeUnregisterAddr
 * DESCRIPTION: Ask debugger to unregister PACE Native Object. This is
 *				useful in the case where the code making this call
 *				needs to unregister Native code that lives elsewhere (e.g.
 *				in a different chunk), and thus AdnGetNativeCodeBaseAddr()
 *				won't retrieve the correct code base address. Generally,
 *				PACE Native Objects which are unregistering themselves should
 *				use the simpler form, AdnDebugNativeUnregister(), below.
 * PARAMETERS:  codeAddr  - PACE Native Object code base address
 * RETURNED:    nothing
 ***********************************************************************/
#define AdnDebugNativeUnregisterAddr(_codeAddr)											\
		{																				\
			AdnDbgPostUnloadParamsType _postUnloadParams;								\
			_postUnloadParams.processID = 0;											\
			_postUnloadParams.moduleID = (unsigned long)_codeAddr /*sectionCookie*/;	\
			_SemihostPostUnload(kAdnSemihostNativeUnregister, &_postUnloadParams);		\
		}

/***********************************************************************
 * FUNCTION:    AdnDebugNativeUnregister
 * DESCRIPTION: Ask debugger to unregister PACE Native Object.
 *				This should be done prior to unlocking the code since
 *				breakpoints in the code may need to be removed.
 * PARAMETERS:  none
 * RETURNED:    nothing
 ***********************************************************************/
#define AdnDebugNativeUnregister() \
		AdnDebugNativeUnregisterAddr(AdnGetNativeCodeBaseAddr())

/***********************************************************************
 * FUNCTION:    SetR10ToDataPointer
 * DESCRIPTION: Helper routine -- set register r10 to point to your
 *				data section (e.g., the GOT from gcc)
 *				(You did compile your code to be r10-relative, didn't you?).
 * PARAMETERS:  none
 * RETURNED:    nothing
 ***********************************************************************/
#define SetR10ToDataPointer(/*(const char *)*/ptr) \
		_SetR10ToThisPointer(ptr)

#endif	// Debugging or not

#endif // __ADNDEBUGMGR_H__
