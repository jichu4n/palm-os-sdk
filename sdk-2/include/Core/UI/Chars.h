/***********************************************************************
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     Chars.h
 * AUTHOR:   Roger Flores: November 3, 1994
 *
 * DECLARER: UI
 *
 * DESCRIPTION:
 *        This file defines the characters in fonts.
 *
 **********************************************************************/
#ifndef __CHARS_H__
#define __CHARS_H__


// Special characters
#define downArrowChr			0x1F
#define upArrowChr			0x1E
#define leftArrowChr			0x1C
#define rightArrowChr		0x1D


// The hardware arrow buttons generate the following Chr codes.
#define 	pageUpChr			0x0B
#define 	pageDownChr			0x0C


//  The following are key codes used for virtual events, like
//   low battery warnings, etc. These keyboard events MUST
//   have the commandKeyMask bit set in the modifiers in order
//   to be recognized.
#define	lowBatteryChr			0x0101		// Display low battery dialog
#define	enterDebuggerChr		0x0102		// Enter Debugger
#define 	nextFieldChr			0x0103		// Go to next field in form
#define	startConsoleChr		0x0104		// Startup console task
#define  menuChr					0x0105		// Ctl-A
#define  commandChr				0x0106		// Ctl-C
#define  confirmChr				0x0107		// Ctl-D
#define  launchChr				0x0108		// Ctl-E
#define  keyboardChr				0x0109		// Ctl-F popup the keyboard in appropriate mode
#define  findChr					0x010A
#define  calcChr					0x010B
#define	prevFieldChr			0x010C
#define	alarmChr					0x010D		// sent before displaying an alarm
#define	ronamaticChr			0x010E		// stroke from graffiti area to top half of screen
#define	graffitiReferenceChr	0x010F		// popup the Graffiti reference
#define	keyboardAlphaChr		0x0110		// popup the keyboard in alpha mode
#define	keyboardNumericChr	0x0111		// popup the keyboard in number mode
#define  lockChr					0x0112		// switch to the Security app and lock the device
#define	backlightChr			0x0113		// toggle state of backlight
#define	autoOffChr				0x0114		// power off due to inactivity timer

//  The Application launching buttons generate the following
// key codes and will also set the commandKeyMask bit in the 
// modifiers field
#define  hardKeyMin			0x0200
#define  hardKeyMax			0x02FF
#define	hard1Chr				0x0204
#define	hard2Chr				0x0205
#define	hard3Chr				0x0206
#define	hard4Chr				0x0207
#define	hardPowerChr		0x0208
#define	hardCradleChr		0x0209			// Button on cradle pressed
#define	hardCradle2Chr		0x020A			// Button on cradle pressed and hwrDockInGeneric1
														//  input on dock asserted (low).

#define ChrIsHardKey(c)		(((c >= hardKeyMin) && (c <= hardKeyMax)) || c == calcChr)

// Characters from all text fonts.  Resource ID's 9000, 9001, 9002
#define nullChr				0x00
#define backspaceChr			0x08
#define tabChr					0x09
#define linefeedChr			0x0A
#define crChr					0x0D
#define colonChr				':'
#define periodChr				'.'
#define commaChr				','
#define quoteChr				'"'

#define returnChr				0x0D
#define escapeChr				0x1B
#define spaceChr				0x20
#define lastAsciiChr			0xFF

#define numericSpaceChr		0x80


#define cutChr					0x12D     // Ctl-X
#define copyChr				0x12E     // Ctl-C
#define pasteChr				0x12F     // Ctl-V


// Extended characters for Windows Code Page 1252
#define lowSingleCommaQuoteChr				0x82
#define scriptFChr								0x83
#define lowDblCommaQuoteChr					0x84
#define horizEllipsisChr						0x85
#define daggerChr									0x86
#define dblDaggerChr								0x87
#define circumflexChr							0x88
#define perMilleChr								0x89
#define upSHacekChr								0x8A
#define leftSingleGuillemetChr				0x8B
#define upOEChr									0x8C
#define diamondChr								0x8D		//	These 4 are not actually in the
																	//	windows code page, but we like 'em
																	//	anyway.
#define clubChr									0x8E
#define heartChr									0x8F
#define spadeChr									0x90
#define singleOpenCommaQuoteChr				0x91
#define singleCloseCommaQuoteChr				0x92
#define dblOpenCommaQuoteChr					0x93
#define dblCloseCommaQuoteChr					0x94
#define bulletChr									0x95
#define enDashChr									0x96
#define emDashChr									0x97
#define spacingTildeChr							0x98
#define trademarkChr								0x99
#define lowSHacekChr								0x9A
#define rightSingleGuillemetChr				0x9B
#define lowOEChr									0x9C
#define commandStrokeChr						0x9D
#define shortcutStrokeChr						0x9E
#define upYDiaeresisChr							0x9F
#define nonBreakSpaceChr						0xA0
#define invertedExclamationChr				0xA1
#define centChr									0xA2
#define poundChr									0xA3
#define currencyChr								0xA4
#define yenChr										0xA5
#define brokenVertBarChr						0xA6
#define sectionChr								0xA7
#define spacingDiaeresisChr					0xA8
#define copyrightChr								0xA9
#define feminineOrdinalChr						0xAA
#define leftGuillemetChr						0xAB
#define notChr										0xAC
#define softHyphenChr							0xAD
#define registeredChr							0xAE
#define spacingMacronChr						0xAF
#define degreeChr									0xB0
#define plusMinusChr								0xB1
#define superscript2Chr							0xB2
#define superscript3Chr							0xB3
#define spacingAcuteChr							0xB4
#define microChr									0xB5
#define paragraphChr								0xB6
#define middleDotChr								0xB7
#define spacingCedillaChr						0xB8
#define superscript1Chr							0xB9
#define masculineOrdinalChr					0xBA
#define rightGuillemetChr						0xBB
#define fractOneQuarterChr						0xBC
#define fractOneHalfChr							0xBD
#define fractThreeQuartersChr					0xBE
#define invertedQuestionChr					0xBF
#define upAGraveChr								0xC0
#define upAAcuteChr								0xC1
#define upACircumflexChr						0xC2
#define upATildeChr								0xC3
#define upADiaeresisChr							0xC4
#define upARingChr								0xC5
#define upAEChr									0xC6
#define upCCedillaChr							0xC7
#define upEGraveChr								0xC8
#define upEAcuteChr								0xC9
#define upECircumflexChr						0xCA
#define upEDiaeresisChr							0xCB
#define upIGraveChr								0xCC
#define upIAcuteChr								0xCD
#define upICircumflexChr						0xCE
#define upIDiaeresisChr							0xCF
#define upEthChr									0xD0
#define upNTildeChr								0xD1
#define upOGraveChr								0xD2
#define upOAcuteChr								0xD3
#define upOCircumflexChr						0xD4
#define upOTildeChr								0xD5
#define upODiaeresisChr							0xD6
#define multiplyChr								0xD7
#define upOSlashChr								0xD8
#define upUGraveChr								0xD9
#define upUAcuteChr								0xDA
#define upUCircumflexChr						0xDB
#define upUDiaeresisChr							0xDC
#define upYAcuteChr								0xDD
#define upThorn									0xDE
#define lowSharpSChr								0xDF							
#define lowAGraveChr								0xE0
#define lowAAcuteChr								0xE1
#define lowACircumflexChr						0xE2
#define lowATildeChr								0xE3
#define lowADiaeresisChr						0xE4
#define lowARingChr								0xE5
#define lowAEChr									0xE6
#define lowCCedillaChr							0xE7
#define lowEGraveChr								0xE8
#define lowEAcuteChr								0xE9
#define lowECircumflexChr						0xEA
#define lowEDiaeresisChr						0xEB
#define lowIGraveChr								0xEC
#define lowIAcuteChr								0xED
#define lowICircumflexChr						0xEE
#define lowIDiaeresisChr						0xEF
#define lowEthChr									0xF0
#define lowNTildeChr								0xF1
#define lowOGraveChr								0xF2
#define lowOAcuteChr								0xF3
#define lowOCircumflexChr						0xF4
#define lowOTildeChr								0xF5
#define lowODiaeresisChr						0xF6
#define divideChr									0xF7
#define lowOSlashChr								0xF8
#define lowUGraveChr								0xF9
#define lowUAcuteChr								0xFA
#define lowUCircumflexChr						0xFB
#define lowUDiaeresisChr						0xFC
#define lowYAcuteChr								0xFD
#define lowThorn									0xFE
#define lowYDiaeresisChr						0xFF


// Characters in the 9 point symbol font.  Resource ID 9003
enum symbolChars {
	symbolLeftArrow = 3,
	symbolRightArrow,
	symbolUpArrow,
	symbolDownArrow,
	symbolSmallDownArrow,
	symbolSmallUpArrow,
	symbolMemo = 9,
	symbolHelp,
	symbolNote,
	symbolNoteSelected,
	symbolCapsLock,
	symbolNumLock,
	symbolShiftUpper,
	symbolShiftPunc,
	symbolShiftExt,
	symbolShiftNone,
	symbolNoTime,
	symbolAlarm,
	symbolRepeat,
	symbolCheckMark
	};

// Character in the 7 point symbol font.  Resource ID 9005
enum symbol7Chars {
	symbol7ScrollUp = 1,
	symbol7ScrollDown,
	symbol7ScrollUpDisabled,
	symbol7ScrollDownDisabled
	};

//	Characters in the 11 point symbol font.  Resource ID 9004
enum symbol11Chars {
	symbolCheckboxOff = 0,
	symbolCheckboxOn,
	symbol11LeftArrow,
	symbol11RightArrow
	};


#endif // __CHARS_H__
