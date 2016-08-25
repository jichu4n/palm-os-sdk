/***********************************************************************
 * Project	International
 * File		TextMgr.h
 * Author	Ken Krugler
 * Purpose	Header file for Text Manager.
 *
 * Written by TransPac Software, Inc.
 * Copyright © 1998-1999 Palm Computing, Inc.
 * All Rights Reserved.
 *
 * Agenda
 *
 * History
 *	03/05/98	kwk	New today.
 *	02/16/99	kwk	Added charEncodingPalmLatin & charEncodingPalmSJIS,
 *					since we've extended the CP1252 & CP932 encodings.
 *				kwk	Added TxtUpperStr & TxtLowerStr macros.
 *				kwk	Changed TxtTruncate to TxtGetTruncationOffset.
 *
 ***********************************************************************/

#ifndef __TEXTMGR_H__
#define __TEXTMGR_H__


#include <IntlMgr.h>


/***********************************************************************
 * Public constants
 ***********************************************************************/

// Various character encodings supported by the PalmOS. Actually these
// are a mixture of character sets (repetoires or coded character sets
// in Internet lingo) and character encodids (CES - character encoding
// standard). Many, however, are some of both (e.g. CP932 is the Shift-JIS
// encoding of the JIS character set + Microsoft's extensions).

typedef enum {
	charEncodingUnknown = 0,	// Unknown to this version of PalmOS.
	
	charEncodingAscii,			// ISO 646-1991
	charEncodingISO8859_1,		// ISO 8859 Part 1
	charEncodingPalmLatin,		// PalmOS version of CP1252
	charEncodingShiftJIS,		// Encoding for 0208-1990 + 1-byte katakana
	charEncodingPalmSJIS,		// PalmOS version of CP932
	charEncodingUTF8,			// Encoding for Unicode
	charEncodingCP1252,			// Windows variant of 8859-1
	charEncodingCP932			// Windows variant of ShiftJIS
} CharEncodingType;

// Transliteration operations for the TxtTransliterate call. We don't use
// an enum, since each character encoding contains its own set of special
// transliteration operations (which begin at translitOpCustomBase).

typedef Word TranslitOpType;

#define	translitOpUpperCase		0
#define	translitOpLowerCase		1

#define	translitOpCustomBase	1000	// Beginning of char-encoding specific ops.

#define	translitOpPreprocess	0x8000	// Mask for pre-process option, where
										// no transliteration actually is done.

// Names of the known encodings.

#define encodingNameAscii			"us-ascii"
#define	encodingNameISO8859_1		"ISO-8859-1"
#define	encodingNameCP1252			"ISO-8859-1-Windows-3.1-Latin-1"
#define encodingNameShiftJIS		"Shift_JIS"
#define	encodingNameCP932			"Windows-31J"
#define	encodingNameUTF8			"UTF-8"

// Maximum length of any encoding name.

#define	kMaxEncodingName			40

// Various byte attribute flags. Note that multiple flags can be
// set, thus a byte could be both a single-byte character, or the first
// byte of a multi-byte character.

#define	byteAttrFirst			0x80	// First byte of multi-byte char.
#define	byteAttrLast			0x40	// Last byte of multi-byte char.
#define	byteAttrMiddle			0x20	// Middle byte of muli-byte char.
#define	byteAttrSingle			0x01	// Single byte.

// Various sets of character attribute flags.

#define charAttrPrint			(_DI|_LO|_PU|_SP|_UP|_XA)
#define charAttrSpace			(_CN|_SP|_XS)
#define charAttrAlNum			(_DI|_LO|_UP|_XA)
#define	charAttrAlpha			(_LO|_UP|_XA)
#define	charAttrCntrl			(_BB|_CN)
#define	charAttrGraph			(_DI|_LO|_PU|_UP|_XA)
#define	charAttrDelim			(_SP|_PU)

// Maximum size a single WChar character will occupy in a text string.

#define	kMaxCharSize			3

// Text manager error codes.

#define	txtErrUknownTranslitOp				(txtErrorClass | 1)
#define	txtErrTranslitOverrun				(txtErrorClass | 2)
#define	txtErrTranslitOverflow				(txtErrorClass | 3)

/***********************************************************************
 * Public macros
 ***********************************************************************/

#define TxtCharIsSpace(ch)		((TxtCharAttr(ch) & charAttrSpace) != 0)
#define	TxtCharIsPrint(ch)		((TxtCharAttr(ch) & charAttrPrint) != 0)
#define	TxtCharIsDigit(ch)		((TxtCharAttr(ch) & _DI) != 0)
#define TxtCharIsAlNum(ch)		((TxtCharAttr(ch) & charAttrAlNum) != 0)
#define TxtCharIsAlpha(ch)		((TxtCharAttr(ch) & charAttrAlpha) != 0)
#define TxtCharIsCntrl(ch)		((TxtCharAttr(ch) & charAttrCntrl) != 0)
#define TxtCharIsGraph(ch)		((TxtCharAttr(ch) & charAttrGraph) != 0)
#define TxtCharIsLower(ch)		((TxtCharAttr(ch) & _LO) != 0)
#define TxtCharIsPunct(ch)		((TxtCharAttr(ch) & _PU) != 0)
#define TxtCharIsUpper(ch)		((TxtCharAttr(ch) & _UP) != 0)
#define TxtCharIsHex(ch)		((TxtCharAttr(ch) & _XD) != 0)
#define TxtCharIsDelim(ch)		((TxtCharAttr(ch) & charAttrDelim) != 0)

// <c> is a hard key if the event modifier <m> has the command bit set
// and <c> is either in the proper range or is the calculator character.
#define	TxtCharIsHardKey(m, c)	((((m) & commandKeyMask) != 0) && \
								((((c) >= hardKeyMin) && ((c) <= hardKeyMax)) || ((c) == calcChr)))

#define	TxtPreviousCharSize(inText, inOffset)	TxtGetPreviousChar((inText), (inOffset), NULL)
#define	TxtNextCharSize(inText, inOffset)		TxtGetNextChar((inText), (inOffset), NULL)


/***********************************************************************
 * Public routines
 ***********************************************************************/

#ifdef __cplusplus
	extern "C" {
#endif

// Return back byte attribute (first, last, single, middle) for <inByte>.

Byte TxtByteAttr(Byte inByte)
		INTL_TRAP(intlTxtByteAttr);
		
// Return back the standard attribute bits for <inChar>.

Word TxtCharAttr(WChar inChar)
		INTL_TRAP(intlTxtCharAttr);

// Return back the extended attribute bits for <inChar>.

Word TxtCharXAttr(WChar inChar)
		INTL_TRAP(intlTxtCharXAttr);

// Return the size (in bytes) of the character <inChar>. This represents
// how many bytes would be required to store the character in a string.

Word TxtCharSize(WChar inChar)
		INTL_TRAP(intlTxtCharSize);

// Return the width (in pixels) of the character <inChar>.

SWord TxtCharWidth(WChar inChar)
		INTL_TRAP(intlTxtCharWidth);

// Load the character before offset <inOffset> in the <inText> text. Return
// back the size of the character.

Word TxtGetPreviousChar(ConstCharPtr inText, ULong inOffset, WCharPtr outChar)
		INTL_TRAP(intlTxtGetPreviousChar);

// Load the character at offset <inOffset> in the <inText> text. Return
// back the size of the character.

Word TxtGetNextChar(ConstCharPtr inText, ULong inOffset, WCharPtr outChar)
		INTL_TRAP(intlTxtGetNextChar);

// Return the character at offset <inOffset> in the <inText> text.

WChar TxtGetChar(ConstCharPtr inText, ULong inOffset)
		INTL_TRAP(intlTxtGetChar);

// Set the character at offset <inOffset> in the <inText> text, and
// return back the size of the character.

Word TxtSetNextChar(CharPtr ioText, ULong inOffset, WChar inChar)
		INTL_TRAP(intlTxtSetNextChar);

// Replace the substring "^X" (where X is 0..9, as specified by <inParamNum>)
// with the string <inParamStr>. If <inParamStr> is nil then don't modify <ioStr>.
// Make sure the resulting string doesn't contain more than <inMaxLen> bytes,
// excluding the terminating null. Return back the number of occurances of
// the substring found in <ioStr>.

Word TxtReplaceStr(CharPtr ioStr, Word inMaxLen, ConstCharPtr inParamStr, Word inParamNum)
		INTL_TRAP(intlTxtReplaceStr);

// Return the bounds of the character at <inOffset> in the <inText>
// text, via the <outStart> & <outEnd> offsets, and also return the
// actual value of character at or following <inOffset>.

WChar TxtCharBounds(ConstCharPtr inText, ULong inOffset, ULongPtr outStart, ULongPtr outEnd)
		INTL_TRAP(intlTxtCharBounds);

// Return the appropriate byte position for truncating <inText> such that it is
// at most <inOffset> bytes long.

ULong TxtGetTruncationOffset(ConstCharPtr inText, ULong inOffset)
		INTL_TRAP(intlTxtGetTruncationOffset);

// Convert the characters in <inSource> into an appropriate form for searching,
// and copy up to <inDstSize> bytes of converted characters into <outDest>. The
// resulting string will be null-terminated. We assume that <inDstSize> includes
// the space required for the null.

void TxtPrepFindString(ConstCharPtr inSource, CharPtr outDest, Word inDstSize)
		INTL_TRAP(intlTxtPrepFindString);

// Search for <inTargetStr> in <inSourceStr>. If found return true and pass back
// the found position (byte offset) in <outPos>, and the length of the matched
// text in <outLength>.

Boolean TxtFindString(ConstCharPtr inSourceStr, ConstCharPtr inTargetStr,
			ULongPtr outPos, WordPtr outLength)
		INTL_TRAP(intlTxtFindString);

// Find the bounds of the word that contains the character at <inOffset>.
// Return the offsets in <*outStart> and <*outEnd>. Return true if the
// word we found was not empty & not a delimiter (attribute of first char
// in word not equal to space or punct).

Boolean TxtWordBounds(ConstCharPtr inText, ULong inLength, ULong inOffset,
			ULongPtr outStart, ULongPtr outEnd)
		INTL_TRAP(intlTxtWordBounds);

// Return the minimum (lowest) encoding required for <inChar>. If we
// don't know about the character, return encoding_Unknown.

CharEncodingType TxtCharEncoding(WChar inChar)
		INTL_TRAP(intlTxtCharEncoding);

// Return the minimum (lowest) encoding required to represent <inStr>.
// This is the maximum encoding of any character in the string, where
// highest is unknown, and lowest is ascii.

CharEncodingType TxtStrEncoding(ConstCharPtr inStr)
		INTL_TRAP(intlTxtStrEncoding);

// Return the higher (max) encoding of <a> and <b>.

CharEncodingType TxtMaxEncoding(CharEncodingType a, CharEncodingType b)
		INTL_TRAP(intlTxtMaxEncoding);

// Return a const pointer to the 'standard' name for <inEncoding>. If the
// encoding is unknown, return a pointer to an empty string.

ConstCharPtr TxtEncodingName(CharEncodingType inEncoding)
		INTL_TRAP(intlTxtEncodingName);

// Transliterate <inSrcLength> bytes of text found in <inSrcText>, based
// on the requested <inOp> operation. Place the results in <outDstText>,
// and set the resulting length in <ioDstLength>. On entry <ioDstLength>
// must contain the maximum size of the <outDstText> buffer. If the
// buffer isn't large enough, return an error (note that outDestText
// might have been modified during the operation). Note that if <inOp>
// has the preprocess bit set, then <outDstText> is not modified, and
// <ioDstLength> will contain the total space required in the destination
// buffer in order to perform the operation. 

Err TxtTransliterate(ConstCharPtr inSrcText, Word inSrcLength, CharPtr outDstText,
			WordPtr ioDstLength, TranslitOpType inOp)
		INTL_TRAP(intlTxtTransliterate);

// Return true if <inChar> is a valid (drawable) character. Note that we'll
// return false if it is a virtual character code.

Boolean TxtIsValidChar(WChar inChar)
		INTL_TRAP(intlTxtIsValidChar);

// Compare the first <s1Len> bytes of <s1> with the first <s2Len> bytes
// of <s2>. Return the results of the comparison: < 0 if <s1> sorts before
// <s2>, > 0 if <s1> sorts after <s2>, and 0 if they are equal. Also return
// the number of bytes that matched in <s1MatchLen> and <s2MatchLen>
// (either one of which can be NULL if the match length is not needed).
// This comparison is "caseless", in the same manner as a find operation,
// thus case, character size, etc. don't matter.

Int TxtCaselessCompare(ConstCharPtr s1, Word s1Len, WordPtr s1MatchLen,
			ConstCharPtr s2, Word s2Len, WordPtr s2MatchLen)
		INTL_TRAP(intlTxtCaselessCompare);

// Compare the first <s1Len> bytes of <s1> with the first <s2Len> bytes
// of <s2>. Return the results of the comparison: < 0 if <s1> sorts before
// <s2>, > 0 if <s1> sorts after <s2>, and 0 if they are equal. Also return
// the number of bytes that matched in <s1MatchLen> and <s2MatchLen>
// (either one of which can be NULL if the match length is not needed).

Int TxtCompare(ConstCharPtr s1, Word s1Len, WordPtr s1MatchLen,
			ConstCharPtr s2, Word s2Len, WordPtr s2MatchLen)
		INTL_TRAP(intlTxtCompare);

#ifdef __cplusplus
	}
#endif

#endif // __TEXTMGR_H__
