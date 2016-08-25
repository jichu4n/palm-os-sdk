/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: CharShiftJIS.h
 *
 * Description:
 *         	Header file for Shift-JIS (code page 932) Japanese character
 *			encoding. These are based on Windows-J implementation of the
 *			Shift-JIS standard.
 *
 * Written by TransPac Software, Inc.
 *
 * History:
 *			Created by Ken Krugler
 *	12 Mar 98	kwk	New today.
 *	06 Apr 98	kwk	Reverted back to #define instead of const values.
 *	07 Apr 98	kwk	Added TxtCharIsHiragana & TxtCharIsKatakana macros.
 *	09 Apr 98	kwk	Made this entire file conditional on NON_INTERNATIONAL.
 *	15 Apr 98	kwk	Filled out full set of character names (from Unicode
 *					2.0 standard).
 *	17 Apr 98	kwk	Resolved duplicated names (thanks Microsoft).
 *	28 May 98	kwk	Put in horizEllipsisChr & numericSpaceChr.
 *	29 Jun 98	kwk	Changed name from CharCP932.h to CharShiftJIS.h.
 *	15 Aug 98	CSS	Reworked the extended character attribute #defines so
 *					we've now got a MicroSoft bit and a bunch of field
 *					values specifying kana, romaji, kanji, greek, etc.
 *					that don't overlap with the standard attributes.
 *				CSS	Recoded the TxtCharIsHiragana/Katakana macros to
 *					AND with class mask and compare with class.
 *	23 Aug 98	kwk	Added charXClassKanaSound.
 *
 *****************************************************************************/

#ifdef NON_INTERNATIONAL

#ifndef __CHARSHIFTJIS_H__
#define __CHARSHIFTJIS_H__

/***********************************************************************
 * Public macros
 ***********************************************************************/

#define	TxtCharIsHiragana(ch)	(	(TxtCharXAttr(ch) & charXClassMask)	\
								==	charXClassHiragana)
#define	TxtCharIsKatakana(ch)	(	(TxtCharXAttr(ch) & charXClassMask)	\
								==	charXClassKatakana)

/***********************************************************************
 * Public constants
 ***********************************************************************/

// Transliteration operations that are not universal, but can be applied
// to Japanese text.

#define	translitOpFullToHalfKatakana	(translitOpCustomBase+0)
#define	translitOpHalfToFullKatakana	(translitOpCustomBase+1)
#define	translitOpFullToHalfRomaji		(translitOpCustomBase+2)
#define	translitOpHalfToFullRomaji		(translitOpCustomBase+3)
#define	translitOpKatakanaToHiragana	(translitOpCustomBase+4)
#define	translitOpHiraganaToKatakana	(translitOpCustomBase+5)
#define	translitOpCombineSoundMark		(translitOpCustomBase+6)
#define	translitOpDivideSoundMark		(translitOpCustomBase+7)
#define	translitOpRomajiToHiragana		(translitOpCustomBase+8)
#define	translitOpHiraganaToRomaji		(translitOpCustomBase+9)

// Extended character attributes for the Shift-JIS (CP932) code page.
// Note that these attributes have to be on an encoding basis, since
// they're shared across all languages which use this encoding. For
// Japanese there's only one language, so we're OK to encode wrapping
// info here, which is often language-dependent.

#define	charXAttrMask			0x00ff
#define	charXAttrFollowing	0x0001
#define	charXAttrLeading		0x0002
#define	charXAttrBreak			0x0004
#define	charXAttrMicroSoft	0x0008

#define	charXClassMask			0x0f00
#define	charXClassRomaji		0x0100
#define	charXClassHiragana	0x0200
#define	charXClassKatakana	0x0300
#define	charXClassKanaSound	0x0400
#define	charXClassGreek		0x0500
#define	charXClassCyrillic	0x0600
#define	charXClassKanjiL1		0x0700
#define	charXClassKanjiL2		0x0800
#define	charXClassKanjiOther	0x0900
#define	charXClassOther		0x0a00
#define	charXClassUndefined	0x0b00

// Some ShiftJIS-specific combinations. Every byte in a stream of
// ShiftJIS data must be either a single byte, a single/low byte,
// or a high/low byte.

#define byteAttrSingleLow		(byteAttrSingle | byteAttrLast)
#define byteAttrHighLow			(byteAttrFirst | byteAttrLast)

#define kFirstHighByte			0x81
#define kFirstLowByte			0x40

// Character codes that are specific to Shift JIS. These names
// are generated from the Unicode 2.0 data files.

#define	chrYenSign								0x005c

#define	chrFirstSJISDoubleByte				0x8140

#define	chrHalfwidthIdeographicFullStop 	0x00A1
#define	chrHalfwidthLeftCornerBracket 	0x00A2
#define	chrHalfwidthRightCornerBracket 	0x00A3
#define	chrHalfwidthIdeographicComma 		0x00A4
#define	chrHalfwidthKatakanaMiddleDot 	0x00A5
#define	chrHalfwidthKatakana_WO				0x00A6
#define	chrHalfwidthKatakanaSmall_A 		0x00A7
#define	chrHalfwidthKatakanaSmall_I 		0x00A8
#define	chrHalfwidthKatakanaSmall_U 		0x00A9
#define	chrHalfwidthKatakanaSmall_E 		0x00AA
#define	chrHalfwidthKatakanaSmall_O 		0x00AB
#define	chrHalfwidthKatakanaSmall_YA 		0x00AC
#define	chrHalfwidthKatakanaSmall_YU 		0x00AD
#define	chrHalfwidthKatakanaSmall_YO 		0x00AE
#define	chrHalfwidthKatakanaSmall_TU 		0x00AF
#define	chrHalfwidthKatakanaHiraganaProlongedSoundMark	0x00B0
#define	chrHalfwidthKatakana_A				0x00B1
#define	chrHalfwidthKatakana_I				0x00B2
#define	chrHalfwidthKatakana_U				0x00B3
#define	chrHalfwidthKatakana_E				0x00B4
#define	chrHalfwidthKatakana_O				0x00B5
#define	chrHalfwidthKatakana_KA				0x00B6
#define	chrHalfwidthKatakana_KI				0x00B7
#define	chrHalfwidthKatakana_KU				0x00B8
#define	chrHalfwidthKatakana_KE				0x00B9
#define	chrHalfwidthKatakana_KO				0x00BA
#define	chrHalfwidthKatakana_SA				0x00BB
#define	chrHalfwidthKatakana_SI				0x00BC
#define	chrHalfwidthKatakana_SU				0x00BD
#define	chrHalfwidthKatakana_SE				0x00BE
#define	chrHalfwidthKatakana_SO				0x00BF
#define	chrHalfwidthKatakana_TA				0x00C0
#define	chrHalfwidthKatakana_TI				0x00C1
#define	chrHalfwidthKatakana_TU				0x00C2
#define	chrHalfwidthKatakana_TE				0x00C3
#define	chrHalfwidthKatakana_TO				0x00C4
#define	chrHalfwidthKatakana_NA				0x00C5
#define	chrHalfwidthKatakana_NI				0x00C6
#define	chrHalfwidthKatakana_NU				0x00C7
#define	chrHalfwidthKatakana_NE				0x00C8
#define	chrHalfwidthKatakana_NO				0x00C9
#define	chrHalfwidthKatakana_HA				0x00CA
#define	chrHalfwidthKatakana_HI				0x00CB
#define	chrHalfwidthKatakana_HU				0x00CC
#define	chrHalfwidthKatakana_HE				0x00CD
#define	chrHalfwidthKatakana_HO				0x00CE
#define	chrHalfwidthKatakana_MA				0x00CF
#define	chrHalfwidthKatakana_MI				0x00D0
#define	chrHalfwidthKatakana_MU				0x00D1
#define	chrHalfwidthKatakana_ME				0x00D2
#define	chrHalfwidthKatakana_MO				0x00D3
#define	chrHalfwidthKatakana_YA				0x00D4
#define	chrHalfwidthKatakana_YU				0x00D5
#define	chrHalfwidthKatakana_YO				0x00D6
#define	chrHalfwidthKatakana_RA				0x00D7
#define	chrHalfwidthKatakana_RI				0x00D8
#define	chrHalfwidthKatakana_RU				0x00D9
#define	chrHalfwidthKatakana_RE				0x00DA
#define	chrHalfwidthKatakana_RO				0x00DB
#define	chrHalfwidthKatakana_WA				0x00DC
#define	chrHalfwidthKatakana_N				0x00DD
#define	chrHalfwidthKatakanaVoicedSoundMark 	0x00DE
#define	chrHalfwidthKatakanaSemiVoicedSoundMark 0x00DF

#define	chrIdeographicSpace					0x8140
#define	chrIdeographicComma					0x8141
#define	chrIdeographicFullStop				0x8142
#define	chrFullwidthComma						0x8143
#define	chrFullwidthFullStop					0x8144
#define	chrKatakanaMiddleDot					0x8145
#define	chrFullwidthColon						0x8146
#define	chrFullwidthSemicolon				0x8147
#define	chrFullwidthQuestionMark 			0x8148
#define	chrFullwidthExclamationMark 		0x8149
#define	chrKatakanaHiraganaVoicedSoundMark	0x814A
#define	chrKatakanaHiraganaSemiVoicedSoundMark	0x814B
#define	chrAcuteAccent							0x814C
#define	chrFullwidthGraveAccent				0x814D
#define	chrDiaeresis							0x814E
#define	chrFullwidthCircumflexAccent 		0x814F
#define	chrFullwidthMacron					0x8150
#define	chrFullwidthLowLine					0x8151
#define	chrKatakanaIterationMark 			0x8152
#define	chrKatakanaVoicedIterationMark 	0x8153
#define	chrHiraganaIterationMark 			0x8154
#define	chrHiraganaVoicedIterationMark 	0x8155
#define	chrDittoMark							0x8156
#define	chrIdeographicIterationMark 		0x8158
#define	chrIdeographicClosingMark 			0x8159
#define	chrIdeographicNumberZero 			0x815A
#define	chrKatakanaHiraganaProlongedSoundMark 0x815B
#define	chrHorizontalBar						0x815C
#define	chrHyphen								0x815D
#define	chrFullwidthSolidus					0x815E
#define	chrFullwidthReverseSolidus 		0x815F
#define	chrFullwidthTilde						0x8160
#define	chrParallelTo							0x8161
#define	chrFullwidthVerticalLine 			0x8162
#define	chrFullwidthHorizontalEllipsis	0x8163
#define	chrTwoDotLeader						0x8164
#define	chrLeftSingleQuotationMark 		0x8165
#define	chrRightSingleQuotationMark 		0x8166
#define	chrLeftDoubleQuotationMark 		0x8167
#define	chrRightDoubleQuotationMark 		0x8168
#define	chrFullwidthLeftParenthesis 		0x8169
#define	chrFullwidthRightParenthesis 		0x816A
#define	chrLeftTortoiseShellBracket 		0x816B
#define	chrRightTortoiseShellBracket 		0x816C
#define	chrFullwidthLeftSquareBracket 	0x816D
#define	chrFullwidthRightSquareBracket 	0x816E
#define	chrFullwidthLeftCurlyBracket 		0x816F
#define	chrFullwidthRightCurlyBracket 	0x8170
#define	chrLeftAngleBracket					0x8171
#define	chrRightAngleBracket					0x8172
#define	chrLeftDoubleAngleBracket 			0x8173
#define	chrRightDoubleAngleBracket 		0x8174
#define	chrLeftCornerBracket					0x8175
#define	chrRightCornerBracket				0x8176
#define	chrLeftWhiteCornerBracket 			0x8177
#define	chrRightWhiteCornerBracket 		0x8178
#define	chrLeftBlackLenticularBracket 	0x8179
#define	chrRightBlackLenticularBracket 	0x817A
#define	chrFullwidthPlusSign					0x817B
#define	chrFullwidthHyphenMinus				0x817C
#define	chrPlusMinusSign						0x817D
#define	chrMultiplicationSign				0x817E
#define	chrDivisionSign						0x8180
#define	chrFullwidthEqualsSign				0x8181
#define	chrNotEqualTo							0x8182
#define	chrFullwidthLessThanSign 			0x8183
#define	chrFullwidthGreaterThanSign 		0x8184
#define	chrLessThanOverEqualTo				0x8185
#define	chrGreaterThanOverEqualTo 			0x8186
#define	chrInfinity								0x8187
#define	chrTherefore							0x8188
#define	chrMaleSign								0x8189
#define	chrFemaleSign							0x818A
#define	chrDegreeSign							0x818B
#define	chrPrime									0x818C
#define	chrDoublePrime							0x818D
#define	chrDegreeCelsius						0x818E
#define	chrFullwidthYenSign					0x818F
#define	chrFullwidthDollarSign				0x8190
#define	chrFullwidthCentSign					0x8191
#define	chrFullwidthPoundSign				0x8192
#define	chrFullwidthPercentSign				0x8193
#define	chrFullwidthNumberSign				0x8194
#define	chrFullwidthAmpersand				0x8195
#define	chrFullwidthAsterisk					0x8196
#define	chrFullwidthCommercialAt 			0x8197
#define	chrSectionSign							0x8198
#define	chrWhiteStar							0x8199
#define	chrBlackStar							0x819A
#define	chrWhiteCircle							0x819B
#define	chrBlackCircle							0x819C
#define	chrBullseye								0x819D
#define	chrWhiteDiamond						0x819E
#define	chrBlackDiamond						0x819F
#define	chrWhiteSquare							0x81A0
#define	chrBlackSquare							0x81A1
#define	chrWhiteUpPointingTriangle 		0x81A2
#define	chrBlackUpPointingTriangle 		0x81A3
#define	chrWhiteDownPointingTriangle 		0x81A4
#define	chrBlackDownPointingTriangle 		0x81A5
#define	chrReferenceMark						0x81A6
#define	chrPostalMark							0x81A7
#define	chrRightwardsArrow					0x81A8
#define	chrLeftwardsArrow						0x81A9
#define	chrUpwardsArrow						0x81AA
#define	chrDownwardsArrow						0x81AB
#define	chrGetaMark								0x81AC
#define	chrElementOf							0x81B8
#define	chrContainsAsMember					0x81B9
#define	chrSubsetOfOrEqualTo					0x81BA
#define	chrSupersetOfOrEqualTo				0x81BB
#define	chrSubsetOf								0x81BC
#define	chrSupersetOf							0x81BD
#define	chrUnion									0x81BE
#define	chrIntersection						0x81BF
#define	chrLogicalAnd							0x81C8
#define	chrLogicalOr							0x81C9
#define	chrFullwidthNotSign					0x81CA
#define	chrRightwardsDoubleArrow 			0x81CB
#define	chrLeftRightDoubleArrow				0x81CC
#define	chrForAll								0x81CD
#define	chrThereExists							0x81CE
#define	chrAngle									0x81DA
#define	chrUpTack								0x81DB
#define	chrArc									0x81DC
#define	chrPartialDifferential				0x81DD
#define	chrNabla									0x81DE
#define	chrIdenticalTo							0x81DF
#define	chrApproximatelyEqualToOrTheImageOf 0x81E0
#define	chrMuchLessThan						0x81E1
#define	chrMuchGreaterThan					0x81E2
#define	chrSquareRoot							0x81E3
#define	chrReversedTilde						0x81E4
#define	chrProportionalTo						0x81E5
#define	chrBecause								0x81E6
#define	chrIntegral								0x81E7
#define	chrDoubleIntegral						0x81E8
#define	chrAngstromSign						0x81F0
#define	chrPerMilleSign						0x81F1
#define	chrMusicSharpSign						0x81F2
#define	chrMusicFlatSign						0x81F3
#define	chrEighthNote							0x81F4
#define	chrDagger								0x81F5
#define	chrDoubleDagger						0x81F6
#define	chrPilcrowSign							0x81F7
#define	chrLargeCircle							0x81FC

#define	chrFullwidthDigitZero				0x824F
#define	chrFullwidthDigitOne					0x8250
#define	chrFullwidthDigitTwo					0x8251
#define	chrFullwidthDigitThree				0x8252
#define	chrFullwidthDigitFour				0x8253
#define	chrFullwidthDigitFive				0x8254
#define	chrFullwidthDigitSix					0x8255
#define	chrFullwidthDigitSeven				0x8256
#define	chrFullwidthDigitEight				0x8257
#define	chrFullwidthDigitNine				0x8258
#define	chrFullwidthCapital_A				0x8260
#define	chrFullwidthCapital_B				0x8261
#define	chrFullwidthCapital_C				0x8262
#define	chrFullwidthCapital_D				0x8263
#define	chrFullwidthCapital_E				0x8264
#define	chrFullwidthCapital_F				0x8265
#define	chrFullwidthCapital_G				0x8266
#define	chrFullwidthCapital_H				0x8267
#define	chrFullwidthCapital_I				0x8268
#define	chrFullwidthCapital_J				0x8269
#define	chrFullwidthCapital_K				0x826A
#define	chrFullwidthCapital_L				0x826B
#define	chrFullwidthCapital_M				0x826C
#define	chrFullwidthCapital_N				0x826D
#define	chrFullwidthCapital_O				0x826E
#define	chrFullwidthCapital_P				0x826F
#define	chrFullwidthCapital_Q				0x8270
#define	chrFullwidthCapital_R				0x8271
#define	chrFullwidthCapital_S				0x8272
#define	chrFullwidthCapital_T				0x8273
#define	chrFullwidthCapital_U				0x8274
#define	chrFullwidthCapital_V				0x8275
#define	chrFullwidthCapital_W				0x8276
#define	chrFullwidthCapital_X				0x8277
#define	chrFullwidthCapital_Y				0x8278
#define	chrFullwidthCapital_Z				0x8279
#define	chrFullwidthSmall_A					0x8281
#define	chrFullwidthSmall_B					0x8282
#define	chrFullwidthSmall_C					0x8283
#define	chrFullwidthSmall_D					0x8284
#define	chrFullwidthSmall_E					0x8285
#define	chrFullwidthSmall_F					0x8286
#define	chrFullwidthSmall_G					0x8287
#define	chrFullwidthSmall_H					0x8288
#define	chrFullwidthSmall_I					0x8289
#define	chrFullwidthSmall_J					0x828A
#define	chrFullwidthSmall_K					0x828B
#define	chrFullwidthSmall_L					0x828C
#define	chrFullwidthSmall_M					0x828D
#define	chrFullwidthSmall_N					0x828E
#define	chrFullwidthSmall_O					0x828F
#define	chrFullwidthSmall_P					0x8290
#define	chrFullwidthSmall_Q					0x8291
#define	chrFullwidthSmall_R					0x8292
#define	chrFullwidthSmall_S					0x8293
#define	chrFullwidthSmall_T					0x8294
#define	chrFullwidthSmall_U					0x8295
#define	chrFullwidthSmall_V					0x8296
#define	chrFullwidthSmall_W					0x8297
#define	chrFullwidthSmall_X					0x8298
#define	chrFullwidthSmall_Y					0x8299
#define	chrFullwidthSmall_Z					0x829A

#define	chrHiraganaSmall_A					0x829F
#define	chrHiragana_A							0x82A0
#define	chrHiraganaSmall_I					0x82A1
#define	chrHiragana_I							0x82A2
#define	chrHiraganaSmall_U					0x82A3
#define	chrHiragana_U							0x82A4
#define	chrHiraganaSmall_E					0x82A5
#define	chrHiragana_E							0x82A6
#define	chrHiraganaSmall_O					0x82A7
#define	chrHiragana_O							0x82A8
#define	chrHiragana_KA							0x82A9
#define	chrHiragana_GA							0x82AA
#define	chrHiragana_KI							0x82AB
#define	chrHiragana_GI							0x82AC
#define	chrHiragana_KU							0x82AD
#define	chrHiragana_GU							0x82AE
#define	chrHiragana_KE							0x82AF
#define	chrHiragana_GE							0x82B0
#define	chrHiragana_KO							0x82B1
#define	chrHiragana_GO							0x82B2
#define	chrHiragana_SA							0x82B3
#define	chrHiragana_ZA							0x82B4
#define	chrHiragana_SI							0x82B5
#define	chrHiragana_ZI							0x82B6
#define	chrHiragana_SU							0x82B7
#define	chrHiragana_ZU							0x82B8
#define	chrHiragana_SE							0x82B9
#define	chrHiragana_ZE							0x82BA
#define	chrHiragana_SO							0x82BB
#define	chrHiragana_ZO							0x82BC
#define	chrHiragana_TA							0x82BD
#define	chrHiragana_DA							0x82BE
#define	chrHiragana_TI							0x82BF
#define	chrHiragana_DI							0x82C0
#define	chrHiraganaSmall_TU					0x82C1
#define	chrHiragana_TU							0x82C2
#define	chrHiragana_DU							0x82C3
#define	chrHiragana_TE							0x82C4
#define	chrHiragana_DE							0x82C5
#define	chrHiragana_TO							0x82C6
#define	chrHiragana_DO							0x82C7
#define	chrHiragana_NA							0x82C8
#define	chrHiragana_NI							0x82C9
#define	chrHiragana_NU							0x82CA
#define	chrHiragana_NE							0x82CB
#define	chrHiragana_NO							0x82CC
#define	chrHiragana_HA							0x82CD
#define	chrHiragana_BA							0x82CE
#define	chrHiragana_PA							0x82CF
#define	chrHiragana_HI							0x82D0
#define	chrHiragana_BI							0x82D1
#define	chrHiragana_PI							0x82D2
#define	chrHiragana_HU							0x82D3
#define	chrHiragana_BU							0x82D4
#define	chrHiragana_PU							0x82D5
#define	chrHiragana_HE							0x82D6
#define	chrHiragana_BE							0x82D7
#define	chrHiragana_PE							0x82D8
#define	chrHiragana_HO							0x82D9
#define	chrHiragana_BO							0x82DA
#define	chrHiragana_PO							0x82DB
#define	chrHiragana_MA							0x82DC
#define	chrHiragana_MI							0x82DD
#define	chrHiragana_MU							0x82DE
#define	chrHiragana_ME							0x82DF
#define	chrHiragana_MO							0x82E0
#define	chrHiraganaSmall_YA					0x82E1
#define	chrHiragana_YA							0x82E2
#define	chrHiraganaSmall_YU					0x82E3
#define	chrHiragana_YU							0x82E4
#define	chrHiraganaSmall_YO					0x82E5
#define	chrHiragana_YO							0x82E6
#define	chrHiragana_RA							0x82E7
#define	chrHiragana_RI							0x82E8
#define	chrHiragana_RU							0x82E9
#define	chrHiragana_RE							0x82EA
#define	chrHiragana_RO							0x82EB
#define	chrHiraganaSmall_WA					0x82EC
#define	chrHiragana_WA							0x82ED
#define	chrHiragana_WI							0x82EE
#define	chrHiragana_WE							0x82EF
#define	chrHiragana_WO							0x82F0
#define	chrHiragana_N							0x82F1

#define	chrKatakanaSmall_A					0x8340
#define	chrKatakana_A							0x8341
#define	chrKatakanaSmall_I					0x8342
#define	chrKatakana_I							0x8343
#define	chrKatakanaSmall_U					0x8344
#define	chrKatakana_U							0x8345
#define	chrKatakanaSmall_E					0x8346
#define	chrKatakana_E							0x8347
#define	chrKatakanaSmall_O					0x8348
#define	chrKatakana_O							0x8349
#define	chrKatakana_KA							0x834A
#define	chrKatakana_GA							0x834B
#define	chrKatakana_KI							0x834C
#define	chrKatakana_GI							0x834D
#define	chrKatakana_KU							0x834E
#define	chrKatakana_GU							0x834F
#define	chrKatakana_KE							0x8350
#define	chrKatakana_GE							0x8351
#define	chrKatakana_KO							0x8352
#define	chrKatakana_GO							0x8353
#define	chrKatakana_SA							0x8354
#define	chrKatakana_ZA							0x8355
#define	chrKatakana_SI							0x8356
#define	chrKatakana_ZI							0x8357
#define	chrKatakana_SU							0x8358
#define	chrKatakana_ZU							0x8359
#define	chrKatakana_SE							0x835A
#define	chrKatakana_ZE							0x835B
#define	chrKatakana_SO							0x835C
#define	chrKatakana_ZO							0x835D
#define	chrKatakana_TA							0x835E
#define	chrKatakana_DA							0x835F
#define	chrKatakana_TI							0x8360
#define	chrKatakana_DI							0x8361
#define	chrKatakanaSmall_TU					0x8362
#define	chrKatakana_TU							0x8363
#define	chrKatakana_DU							0x8364
#define	chrKatakana_TE							0x8365
#define	chrKatakana_DE							0x8366
#define	chrKatakana_TO							0x8367
#define	chrKatakana_DO							0x8368
#define	chrKatakana_NA							0x8369
#define	chrKatakana_NI							0x836A
#define	chrKatakana_NU							0x836B
#define	chrKatakana_NE							0x836C
#define	chrKatakana_NO							0x836D
#define	chrKatakana_HA							0x836E
#define	chrKatakana_BA							0x836F
#define	chrKatakana_PA							0x8370
#define	chrKatakana_HI							0x8371
#define	chrKatakana_BI							0x8372
#define	chrKatakana_PI							0x8373
#define	chrKatakana_HU							0x8374
#define	chrKatakana_BU							0x8375
#define	chrKatakana_PU							0x8376
#define	chrKatakana_HE							0x8377
#define	chrKatakana_BE							0x8378
#define	chrKatakana_PE							0x8379
#define	chrKatakana_HO							0x837A
#define	chrKatakana_BO							0x837B
#define	chrKatakana_PO							0x837C
#define	chrKatakana_MA							0x837D
#define	chrKatakana_MI							0x837E
#define	chrKatakana_MU							0x8380
#define	chrKatakana_ME							0x8381
#define	chrKatakana_MO							0x8382
#define	chrKatakanaSmall_YA					0x8383
#define	chrKatakana_YA							0x8384
#define	chrKatakanaSmall_YU					0x8385
#define	chrKatakana_YU							0x8386
#define	chrKatakanaSmall_YO					0x8387
#define	chrKatakana_YO							0x8388
#define	chrKatakana_RA							0x8389
#define	chrKatakana_RI							0x838A
#define	chrKatakana_RU							0x838B
#define	chrKatakana_RE							0x838C
#define	chrKatakana_RO							0x838D
#define	chrKatakanaSmall_WA					0x838E
#define	chrKatakana_WA							0x838F
#define	chrKatakana_WI							0x8390
#define	chrKatakana_WE							0x8391
#define	chrKatakana_WO							0x8392
#define	chrKatakana_N							0x8393
#define	chrKatakana_VU							0x8394
#define	chrKatakanaSmall_KA					0x8395
#define	chrKatakanaSmall_KE					0x8396

#define	chrGreekCapitalAlpha					0x839F
#define	chrGreekCapitalBeta					0x83A0
#define	chrGreekCapitalGamma					0x83A1
#define	chrGreekCapitalDelta					0x83A2
#define	chrGreekCapitalEpsilon				0x83A3
#define	chrGreekCapitalZeta					0x83A4
#define	chrGreekCapitalEta					0x83A5
#define	chrGreekCapitalTheta					0x83A6
#define	chrGreekCapitalIota					0x83A7
#define	chrGreekCapitalKappa					0x83A8
#define	chrGreekCapitalLamda					0x83A9
#define	chrGreekCapitalMu						0x83AA
#define	chrGreekCapitalNu						0x83AB
#define	chrGreekCapitalXi						0x83AC
#define	chrGreekCapitalOmicron				0x83AD
#define	chrGreekCapitalPi						0x83AE
#define	chrGreekCapitalRho					0x83AF
#define	chrGreekCapitalSigma					0x83B0
#define	chrGreekCapitalTau					0x83B1
#define	chrGreekCapitalUpsilon				0x83B2
#define	chrGreekCapitalPhi					0x83B3
#define	chrGreekCapitalChi					0x83B4
#define	chrGreekCapitalPsi					0x83B5
#define	chrGreekCapitalOmega					0x83B6
#define	chrGreekSmallAlpha					0x83BF
#define	chrGreekSmallBeta						0x83C0
#define	chrGreekSmallGamma					0x83C1
#define	chrGreekSmallDelta					0x83C2
#define	chrGreekSmallEpsilon					0x83C3
#define	chrGreekSmallZeta						0x83C4
#define	chrGreekSmallEta						0x83C5
#define	chrGreekSmallTheta					0x83C6
#define	chrGreekSmallIota						0x83C7
#define	chrGreekSmallKappa					0x83C8
#define	chrGreekSmallLamda					0x83C9
#define	chrGreekSmallMu						0x83CA
#define	chrGreekSmallNu						0x83CB
#define	chrGreekSmallXi						0x83CC
#define	chrGreekSmallOmicron					0x83CD
#define	chrGreekSmallPi						0x83CE
#define	chrGreekSmallRho						0x83CF
#define	chrGreekSmallSigma					0x83D0
#define	chrGreekSmallTau						0x83D1
#define	chrGreekSmallUpsilon					0x83D2
#define	chrGreekSmallPhi						0x83D3
#define	chrGreekSmallChi						0x83D4
#define	chrGreekSmallPsi						0x83D5
#define	chrGreekSmallOmega					0x83D6

#define	chrCyrillicCapital_A					0x8440
#define	chrCyrillicCapital_BE				0x8441
#define	chrCyrillicCapital_VE				0x8442
#define	chrCyrillicCapital_GHE				0x8443
#define	chrCyrillicCapital_DE				0x8444
#define	chrCyrillicCapital_IE				0x8445
#define	chrCyrillicCapital_IO				0x8446
#define	chrCyrillicCapital_ZHE				0x8447
#define	chrCyrillicCapital_ZE				0x8448
#define	chrCyrillicCapital_I					0x8449
#define	chrCyrillicCapitalShort_I 			0x844A
#define	chrCyrillicCapital_KA				0x844B
#define	chrCyrillicCapital_EL				0x844C
#define	chrCyrillicCapital_EM				0x844D
#define	chrCyrillicCapital_EN				0x844E
#define	chrCyrillicCapital_O					0x844F
#define	chrCyrillicCapital_PE				0x8450
#define	chrCyrillicCapital_ER				0x8451
#define	chrCyrillicCapital_ES				0x8452
#define	chrCyrillicCapital_TE				0x8453
#define	chrCyrillicCapital_U					0x8454
#define	chrCyrillicCapital_EF				0x8455
#define	chrCyrillicCapital_HA				0x8456
#define	chrCyrillicCapital_TSE				0x8457
#define	chrCyrillicCapital_CHE				0x8458
#define	chrCyrillicCapital_SHA				0x8459
#define	chrCyrillicCapital_SHCHA 			0x845A
#define	chrCyrillicCapitalHardSign 		0x845B
#define	chrCyrillicCapital_YERU				0x845C
#define	chrCyrillicCapitalSoftSign 		0x845D
#define	chrCyrillicCapital_E					0x845E
#define	chrCyrillicCapital_YU				0x845F
#define	chrCyrillicCapital_YA				0x8460
#define	chrCyrillicSmall_A					0x8470
#define	chrCyrillicSmall_BE					0x8471
#define	chrCyrillicSmall_VE					0x8472
#define	chrCyrillicSmall_GHE					0x8473
#define	chrCyrillicSmall_DE					0x8474
#define	chrCyrillicSmall_IE					0x8475
#define	chrCyrillicSmall_IO					0x8476
#define	chrCyrillicSmall_ZHE					0x8477
#define	chrCyrillicSmall_ZE					0x8478
#define	chrCyrillicSmall_I					0x8479
#define	chrCyrillicSmallShort_I				0x847A
#define	chrCyrillicSmall_KA					0x847B
#define	chrCyrillicSmall_EL					0x847C
#define	chrCyrillicSmall_EM					0x847D
#define	chrCyrillicSmall_EN					0x847E
#define	chrCyrillicSmall_O					0x8480
#define	chrCyrillicSmall_PE					0x8481
#define	chrCyrillicSmall_ER					0x8482
#define	chrCyrillicSmall_ES					0x8483
#define	chrCyrillicSmall_TE					0x8484
#define	chrCyrillicSmall_U					0x8485
#define	chrCyrillicSmall_EF					0x8486
#define	chrCyrillicSmall_HA					0x8487
#define	chrCyrillicSmall_TSE					0x8488
#define	chrCyrillicSmall_CHE					0x8489
#define	chrCyrillicSmall_SHA					0x848A
#define	chrCyrillicSmall_SHCHA				0x848B
#define	chrCyrillicSmallHardSign 			0x848C
#define	chrCyrillicSmall_YERU				0x848D
#define	chrCyrillicSmallSoftSign 			0x848E
#define	chrCyrillicSmall_E					0x848F
#define	chrCyrillicSmall_YU					0x8490
#define	chrCyrillicSmall_YA					0x8491

#define	chrBoxDrawingsLightHorizontal 					0x849F
#define	chrBoxDrawingsLightVertical 						0x84A0
#define	chrBoxDrawingsLightDownAndRight 					0x84A1
#define	chrBoxDrawingsLightDownAndLeft 					0x84A2
#define	chrBoxDrawingsLightUpAndLeft 						0x84A3
#define	chrBoxDrawingsLightUpAndRight 					0x84A4
#define	chrBoxDrawingsLightVerticalAndRight 			0x84A5
#define	chrBoxDrawingsLightDownAndHorizontal 			0x84A6
#define	chrBoxDrawingsLightVerticalAndLeft 				0x84A7
#define	chrBoxDrawingsLightUpAndHorizontal 				0x84A8
#define	chrBoxDrawingsLightVerticalAndHorizontal 		0x84A9
#define	chrBoxDrawingsHeavyHorizontal 					0x84AA
#define	chrBoxDrawingsHeavyVertical 						0x84AB
#define	chrBoxDrawingsHeavyDownAndRight 					0x84AC
#define	chrBoxDrawingsHeavyDownAndLeft 					0x84AD
#define	chrBoxDrawingsHeavyUpAndLeft 						0x84AE
#define	chrBoxDrawingsHeavyUpAndRight 					0x84AF
#define	chrBoxDrawingsHeavyVerticalAndRight 			0x84B0
#define	chrBoxDrawingsHeavyDownAndHorizontal 			0x84B1
#define	chrBoxDrawingsHeavyVerticalAndLeft 				0x84B2
#define	chrBoxDrawingsHeavyUpAndHorizontal 				0x84B3
#define	chrBoxDrawingsHeavyVerticalAndHorizontal 		0x84B4
#define	chrBoxDrawingsVerticalHeavyAndRightLight 		0x84B5
#define	chrBoxDrawingsDownLightAndHorizontalHeavy 	0x84B6
#define	chrBoxDrawingsVerticalHeavyAndLeftLight 		0x84B7
#define	chrBoxDrawingsUpLightAndHorizontalHeavy 		0x84B8
#define	chrBoxDrawingsVerticalLightAndHorizontalHeavy 	0x84B9
#define	chrBoxDrawingsVerticalLightAndRightHeavy 		0x84BA
#define	chrBoxDrawingsDownHeavyAndHorizontalLight 	0x84BB
#define	chrBoxDrawingsVerticalLightAndLeftHeavy 		0x84BC
#define	chrBoxDrawingsUpHeavyAndHorizontalLight 		0x84BD
#define	chrBoxDrawingsVerticalHeavyAndHorizontalLight 	0x84BE

#define	chrCircledDigitOne					0x8740
#define	chrCircledDigitTwo					0x8741
#define	chrCircledDigitThree					0x8742
#define	chrCircledDigitFour					0x8743
#define	chrCircledDigitFive					0x8744
#define	chrCircledDigitSix					0x8745
#define	chrCircledDigitSeven					0x8746
#define	chrCircledDigitEight					0x8747
#define	chrCircledDigitNine					0x8748
#define	chrCircledNumberTen					0x8749
#define	chrCircledNumberEleven				0x874A
#define	chrCircledNumberTwelve				0x874B
#define	chrCircledNumberThirteen 			0x874C
#define	chrCircledNumberFourteen 			0x874D
#define	chrCircledNumberFifteen				0x874E
#define	chrCircledNumberSixteen				0x874F
#define	chrCircledNumberSeventeen 			0x8750
#define	chrCircledNumberEighteen 			0x8751
#define	chrCircledNumberNineteen 			0x8752
#define	chrCircledNumberTwenty				0x8753
#define	chrRomanNumeralOne					0x8754
#define	chrRomanNumeralTwo					0x8755
#define	chrRomanNumeralThree					0x8756
#define	chrRomanNumeralFour					0x8757
#define	chrRomanNumeralFive					0x8758
#define	chrRomanNumeralSix					0x8759
#define	chrRomanNumeralSeven					0x875A
#define	chrRomanNumeralEight					0x875B
#define	chrRomanNumeralNine					0x875C
#define	chrRomanNumeralTen					0x875D
#define	chrSquareMiri							0x875F
#define	chrSquareKiro							0x8760
#define	chrSquareSenti							0x8761
#define	chrSquareMeetoru						0x8762
#define	chrSquareGuramu						0x8763
#define	chrSquareTon							0x8764
#define	chrSquareAaru							0x8765
#define	chrSquareHekutaaru					0x8766
#define	chrSquareRittoru						0x8767
#define	chrSquareWatto							0x8768
#define	chrSquareKarorii						0x8769
#define	chrSquareDoru							0x876A
#define	chrSquareSento							0x876B
#define	chrSquarePaasento						0x876C
#define	chrSquareMiribaaru					0x876D
#define	chrSquarePeezi							0x876E
#define	chrSquareMm								0x876F
#define	chrSquareCm								0x8770
#define	chrSquareKm								0x8771
#define	chrSquareMg								0x8772
#define	chrSquareKg								0x8773
#define	chrSquareCc								0x8774
#define	chrSquareMSquared						0x8775
#define	chrSquareEraNameHeisei				0x877E
#define	chrReversedDoublePrimeQuotationMark 0x8780
#define	chrLowDoublePrimeQuotationMark 		0x8781
#define	chrNumeroSign							0x8782
#define	chrSquareKk								0x8783
#define	chrTelephoneSign						0x8784
#define	chrCircledIdeographHigh				0x8785
#define	chrCircledIdeographCentre 			0x8786
#define	chrCircledIdeographLow				0x8787
#define	chrCircledIdeographLeft				0x8788
#define	chrCircledIdeographRight 			0x8789
#define	chrParenthesizedIdeographStock 	0x878A
#define	chrParenthesizedIdeographHave 	0x878B
#define	chrParenthesizedIdeographRepresent 	0x878C
#define	chrSquareEraNameMeizi				0x878D
#define	chrSquareEraNameTaisyou				0x878E
#define	chrSquareEraNameSyouwa				0x878F
#define	chrApproximatelyEqualToOrTheImageOfDup	0x8790		// Same as 0x81E0
#define	chrIdenticalToDup						0x8791				// Same as 0x81DF
#define	chrIntegralDup							0x8792				// Same as 0x81E7
#define	chrContourIntegral					0x8793
#define	chrNArySummation						0x8794
#define	chrSquareRootDup						0x8795				// Same as 0x81E3
#define	chrUpTackDup							0x8796				// Same as 0x81DB
#define	chrAngleDup								0x8797				// Same as 0x81DA
#define	chrRightAngle							0x8798
#define	chrRightTriangle						0x8799
#define	chrBecauseDup							0x879A				// Same as 0x81E6
#define	chrIntersectionDup					0x879B				// Same as 0x81BF
#define	chrUnionDup								0x879C				// Same as 0x81BE

#define	chrSmallRomanNumeralOne				0xEEEF
#define	chrSmallRomanNumeralTwo				0xEEF0
#define	chrSmallRomanNumeralThree 			0xEEF1
#define	chrSmallRomanNumeralFour 			0xEEF2
#define	chrSmallRomanNumeralFive 			0xEEF3
#define	chrSmallRomanNumeralSix				0xEEF4
#define	chrSmallRomanNumeralSeven 			0xEEF5
#define	chrSmallRomanNumeralEight 			0xEEF6
#define	chrSmallRomanNumeralNine 			0xEEF7
#define	chrSmallRomanNumeralTen				0xEEF8
#define	chrFullwidthNotSignDup				0xEEF9				// Same as 0x81CA
#define	chrFullwidthBrokenBar				0xEEFA
#define	chrFullwidthApostrophe				0xEEFB
#define	chrFullwidthQuotationMark 			0xEEFC

#define	chrSmallRomanNumeralOneDup 		0xFA40				// Same as 0xEEEF
#define	chrSmallRomanNumeralTwoDup 		0xFA41				// Same as 0xEEF0
#define	chrSmallRomanNumeralThreeDup 		0xFA42				// Same as 0xEEF1
#define	chrSmallRomanNumeralFourDup 		0xFA43				// Same as 0xEEF2
#define	chrSmallRomanNumeralFiveDup 		0xFA44				// Same as 0xEEF3
#define	chrSmallRomanNumeralSixDup 		0xFA45				// Same as 0xEEF4
#define	chrSmallRomanNumeralSevenDup 		0xFA46				// Same as 0xEEF5
#define	chrSmallRomanNumeralEightDup 		0xFA47				// Same as 0xEEF6
#define	chrSmallRomanNumeralNineDup 		0xFA48				// Same as 0xEEF7
#define	chrSmallRomanNumeralTenDup 		0xFA49				// Same as 0xEEF8
#define	chrRomanNumeralOneDup				0xFA4A				// Same as 0x8754
#define	chrRomanNumeralTwoDup				0xFA4B				// Same as 0x8755
#define	chrRomanNumeralThreeDup				0xFA4C				// Same as 0x8756
#define	chrRomanNumeralFourDup				0xFA4D				// Same as 0x8757
#define	chrRomanNumeralFiveDup				0xFA4E				// Same as 0x8758
#define	chrRomanNumeralSixDup				0xFA4F				// Same as 0x8759
#define	chrRomanNumeralSevenDup				0xFA50				// Same as 0x875A
#define	chrRomanNumeralEightDup				0xFA51				// Same as 0x875B
#define	chrRomanNumeralNineDup				0xFA52				// Same as 0x875C
#define	chrRomanNumeralTenDup				0xFA53				// Same as 0x875D
#define	chrFullwidthNotSignDup2				0xFA54				// Same as 0xEEF9 & 0x81CA
#define	chrFullwidthBrokenBarDup 			0xFA55				// Same as 0xEEFA
#define	chrFullwidthApostropheDup 			0xFA56				// Same as 0xEEFB
#define	chrFullwidthQuotationMarkDup 		0xFA57				// Same as 0xEEFC
#define	chrParenthesizedIdeographStockDup 	0xFA58			// Same as 0x878A
#define	chrNumeroSignDup						0xFA59				// Same as 0x8782
#define	chrTelephoneSignDup					0xFA5A				// Same as 0x8784
#define	chrBecauseDup2							0xFA5B				// Same as 0x81E6 & 0x879A

#define chrLastSJISDoubleByte					0xFCFC

// Alternative character names.

#define	chrChouon	chrKatakanaHiraganaProlongedSoundMark

// Old character names.

#define yenChr								chrYenSign

#endif // __CHARSHIFTJIS_H__

#endif // NON_INTERNATIONAL

