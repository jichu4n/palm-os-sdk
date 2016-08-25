/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *      ImcUtils.c
 *
 * Description:
 *      Routines to handle Internet Mail Consortium specs
 *
 * History:
 *      8/6/97  roger - Created
 *
 *******************************************************************/


#ifndef _IMC_UTILS_H
#define _IMC_UTILS_H

#ifndef EOF
#define EOF	0xffff
#endif


// Constants for some common IMC spec values.
#define parameterDelimeterChr			';'
#define valueDelimeterChr				':'
#define groupDelimeterChr				'.'
#define paramaterNameDelimiterChr	'='
#define endOfLineChr 0x0D

#define imcFilenameLength				32
#define imcUnlimitedChars				0xFFFE		// 64K, minus 1 character for null

// These are for functions called to handle input and output.  These are currently used
// to allow disk based or obx based transfers
typedef Word GetCharF (const void *);
typedef void PutStringF(const void *, const Char * const stringP);


// maxChars does NOT include trailing null, buffer may be 1 larger.
// use imcUnlimitedChars if you don't want a max.
extern CharPtr ImcReadFieldNoSemicolon(const VoidPtr inputStream, 
	const GetCharF inputFunc, WordPtr c, const Word maxChars)
							SYS_TRAP(sysTrapImcReadFieldNoSemicolon);

// maxChars does NOT include trailing null, buffer may be 1 larger.
// use imcUnlimitedChars if you don't want a max.
extern CharPtr ImcReadFieldQuotablePrintable(const VoidPtr inputStream, const GetCharF inputFunc, WordPtr c, 
	const Char stopAt, const Boolean quotedPrintable, const Word maxChars)
							SYS_TRAP(sysTrapImcReadFieldQuotablePrintable);
	
extern void ImcReadPropertyParameter(const VoidPtr inputStream, const GetCharF inputFunc,
										WordPtr cP, const CharPtr nameP, const CharPtr valueP)
							SYS_TRAP(sysTrapImcReadPropertyParameter);
	
extern void ImcSkipAllPropertyParameters(const VoidPtr inputStream, const GetCharF inputFunc, 
	WordPtr cP, const CharPtr identifierP, Boolean *quotedPrintableP)
							SYS_TRAP(sysTrapImcSkipAllPropertyParameters);
	
extern void ImcReadWhiteSpace(const VoidPtr inputStream, const GetCharF inputFunc, 
	const Word * const charAttrP, WordPtr c)
							SYS_TRAP(sysTrapImcReadWhiteSpace);
	
extern void ImcWriteQuotedPrintable(const VoidPtr outputStream, const PutStringF outputFunc, 
	const CharPtr stringP, const Boolean noSemicolons)
							SYS_TRAP(sysTrapImcWriteQuotedPrintable);
	
extern void ImcWriteNoSemicolon(const VoidPtr outputStream, const PutStringF outputFunc, 
	const Char * const stringP)
							SYS_TRAP(sysTrapImcWriteNoSemicolon);
	
extern Boolean ImcStringIsAscii(const Char * const stringP)
							SYS_TRAP(sysTrapImcStringIsAscii);


#endif	// _IMC_UTILS_H
