/*
 * MiniImage.c
 *
 * This wizard-generated code is based on code adapted from the
 * stationery files distributed as part of the Palm OS SDK 4.0.
 *
 * Copyright (c) 2006 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 */
 
/** @ingroup MiniImage
 *
**/

/** @file Main.c
 * This file is the main source file for MiniImage
**/

/** @name
 *
**/
/*@{*/

#include <PalmOS.h>
#include <VFSMgr.h>
#include <PalmOneCodecPluginMgr.h>
#include <PalmOneCodecFormat.h>

#include "Common.h"
#include "MiniImage.h"
#include "MiniImageRsc.h"
#include "FileBrowserForm.h"
#include "FileBrowserFormRsrc.h"

#include "HsNav.h"

#define ourMinVersion    sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define kPalmOS20Version sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)

#define appErr 0x8001
#define gifErr 0x8002

#define BUFFER_SIZE (4096)
#define ALL_FORMATS	0x0001
#define TRACK_TIME	(sysTicksPerSecond / 10)

/** Holds basic encoder/decoder info. */
typedef struct {
	Char	inputFileName[256];
	Char	outputFileName[256];
	
	FileRef inputFileRef;
	FileRef outputFileRef;
	
	UInt32 	width;
	UInt32 	height;
	UInt32  rowBytes;
	UInt32	colorFormat;
	UInt32	fileFormat;
	
	Boolean encoding;
	Boolean fitToScreen;
	
	PalmCodecSession session;

} ImageInfo;

#if 0
#pragma mark Globals
#endif

/** Internal Globals. */	
static ImageInfo 	gImageInfo;
static UInt32		gScreenDepth = 16;
static UInt32		gPreviousScreenDepth = 0;
static UInt16		gCodecMgrLibRefNum = sysInvalidRefNum;
static Char			*gRunButtonLabel[] = { "Decode", "Encode" };
static Char* 		gColorTableP;

/** Offscreen global. */
static WinHandle	gWinH = NULL;
static Int16		gWinWidth = 0;
static Int16		gWinHeight = 0;
static Int16		gImageX = 0;
static Int16		gImageY = 0;
static Boolean		gPanning = false;

/** Current image formats. */
static UInt32		*gCurrentColorFormatList = NULL; 
static Char			**gCurrentColorFormatTextList = NULL;
static UInt32		gCurrentColorFormatCount = 0;
static UInt32		gCurrentColorFormatSelection = 0;

/** Current file formats. */
static UInt32		gCurrentFileFormatSelection = 0;

/** List of supported format. */
static UInt32		gJPEGDecodeFormatCount = 0;
static UInt32		gJPEGDecodeFormat[20];	// I really don't expect more than 20
static UInt32		gJPEGEncodeFormatCount = 0;
static UInt32		gJPEGEncodeFormat[20];
static UInt32		gWBMPDecodeFormatCount = 0;
static UInt32		gWBMPDecodeFormat[20];
static UInt32		gWBMPEncodeFormatCount = 0;
static UInt32		gWBMPEncodeFormat[20];
static UInt32		gGIF87DecodeFormatCount = 0;
static UInt32		gGIF87DecodeFormat[20];
static UInt32		gGIF89DecodeFormatCount = 0;
static UInt32		gGIF89DecodeFormat[20];
static UInt32		gTIFFDecodeFormatCount = 0;
static UInt32		gTIFFDecodeFormat[20];

/** The list of all supported formats. */
static UInt32		gAllColorFormatSupportedCount = 0;
static UInt32		gAllColorFormatSupported[40];

/**
 * List of known supported encoding format.
 */
static Char *gFileFormatTextList[] = 
{
	"JPEG File",
	"WBMP File"
};

static UInt32 gFileFormatList[] = 
{
	palmCodecImageJPEGFile,
	palmCodecImageWBMPFile
};

/**
 * List of Image format
 */
static Char *gColorFormatList[] = 
{
	"ImageARGB32",
	"ImageRGBA32",
	"ImageRGB32",
	"ImageRGB888",
	"ImageRGB888Planar",
	"ImageRGB565",
	"ImageRGB555",
	"ImageBGRA32",
	"ImageBGR32",
	"ImageBGR888",
	"ImageBGR565",
	"ImageBGR555",
	"ImageYUV444",
	"ImageYUV422",
	"ImageYUV420",
	"ImageYUV411",
	"ImageYUV211",
	"ImageYUV444Planar",
	"ImageYUV422Planar",
	"ImageYUV420Planar",
	"ImageYUV411Planar",
	"ImageYUV211Planar",
	"ImageYCbCr444",
	"ImageYCbCr422",
	"ImageYCbCr420",
	"ImageYCbCr411",
	"ImageYCbCr211",
	"ImageYCbCr444Planar",
	"ImageYCbCr422Planar",
	"ImageYCbCr420Planar",
	"ImageYCbCr411Planar",
	"ImageYCbCr211Planar",
	"ImageRGBIndex16",
	"ImageRGBIndex8",
	"ImageRGBIndex4",
	"ImageRGBIndex2",
	"ImageRGBIndex1",
	"ImageRGBIndex",
	"ImageGRAY8",
	"ImageGRAY4",
	"ImageGRAY2",
	"ImageGRAY1",
	"ImageGIFFrame",
	"ImageCMY",
	"ImageCMYK",
	"ImageHSL",
	"ImageHSI",
	"ImageHSV",
	"ImageHCI"				
};

static Err PrvFileOpenInput();

/**
 * Mass byte-swapping - all elments must be 32 bits
 */
 /*
static void PrvSwapStructure(UInt32 *structP, UInt32 numElement)
{	
	while(numElement--) {
		*structP = Swap32(*structP);
		structP++;
	}
}
*/

#if 0
#pragma mark -
#endif

/**
 * Returns the file format from the file name.
 */
static UInt32 PrvGetFileFormat(const Char *fileName, FileRef fileRef)
{
	Err		err = errNone;
	UInt8	buffer[64];
	UInt32	bytesRead = 0;
	UInt32 	fileFormat = 0;
	
	// Read a few bytes from the file
	err = VFSFileRead(fileRef, 64, buffer, &bytesRead);
	if(err || bytesRead < 16) goto Done;
	
	// Check for JPEG
	if( StrCaselessCompare(fileName + StrLen(fileName) - 4, ".jpg") == 0
		||  StrCaselessCompare(fileName + StrLen(fileName) - 5, ".jpeg") == 0 
		||  StrCaselessCompare(fileName + StrLen(fileName) - 4, ".jpe") == 0 )	
	{
		// Just make sure it's a JPEG - Check the SOI
		if(buffer[0] == 0xFF && buffer[1] == 0xD8)
			fileFormat = palmCodecImageJPEGFile;
	}
	else if( StrCaselessCompare(fileName + StrLen(fileName) - 4, ".tif") == 0 
		   ||StrCaselessCompare(fileName + StrLen(fileName) - 5, ".tiff") == 0 )
	{
		// Just make sure it's a TIFF - Check for II or MM for the first 2 bytes
		if((buffer[0] == 'I' && buffer[1] == 'I') || (buffer[0] == 'M' && buffer[1] == 'M'))
			fileFormat = palmCodecImageTIFFFile;
	}
	else if ( StrCaselessCompare(fileName + StrLen(fileName) - 4, ".bmp") == 0 )
	{
		// Just make sure it's a BMP - Check for BM
		if(buffer[0] == 'B' && buffer[1] == 'M')
			fileFormat = palmCodecImageWBMPFile;
	}
	else if ( StrCaselessCompare(fileName + StrLen(fileName) - 4, ".gif") == 0 )
	{
		// Just make sure it's a GIF - Check for GIF
		if(buffer[0] == 'G' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == '8')
		{
			if(buffer[4] == '7' && buffer[5] == 'a')
				fileFormat = palmCodecImageGIF87aFile;
			else if(buffer[4] == '9' && buffer[5] == 'a')
				fileFormat = palmCodecImageGIF89aFile;
		}
	}	

Done:		
	return fileFormat;
}

/**
 * Add a color format to the big list of supported color formats.
 */
static void PrvAddColorFormat(UInt32 colorFormat)
{
	UInt32 count = gAllColorFormatSupportedCount;

	while(count)	
		if(gAllColorFormatSupported[--count] == colorFormat)
			break;
			
	if(!count)
		gAllColorFormatSupported[gAllColorFormatSupportedCount++] = colorFormat;
}

/**
 * Load all supported image formats and their respective
 * supported color formats. This list is only for the known
 * formats.
 */
static Err PrvLoadFormats()
{
	Err err = errNone; 
	UInt32 iterator = palmCodecIteratorStart;
	PalmCodecFormat inputFormat, outputFormat;
	UInt32 creatorID, codecID;
	
	// Reset current state
	gJPEGDecodeFormatCount 	= 0;
	gJPEGEncodeFormatCount 	= 0;
	gWBMPDecodeFormatCount	= 0;
	gWBMPEncodeFormatCount 	= 0;
	gGIF87DecodeFormatCount	= 0;
	gGIF89DecodeFormatCount	= 0;
	gTIFFDecodeFormatCount	= 0;
	gAllColorFormatSupportedCount = 0;
	
	// Enumerate all formats
	while(iterator != palmCodecIteratorStop)
	{
		err = CodecMgrEnumerateFormats(gCodecMgrLibRefNum, &iterator, &inputFormat, &outputFormat, &creatorID, &codecID);
		if(err)	goto Done;
		
		switch(inputFormat)
		{
			case palmCodecImageJPEGFile:
				gJPEGDecodeFormat[gJPEGDecodeFormatCount] = outputFormat;
				gJPEGDecodeFormatCount++;
				break;
				
			case palmCodecImageWBMPFile:
				gWBMPDecodeFormat[gWBMPDecodeFormatCount] = outputFormat;
				gWBMPDecodeFormatCount++;
				break;
				
			case palmCodecImageGIF87aFile:
				gGIF87DecodeFormat[gGIF87DecodeFormatCount] = outputFormat;
				gGIF87DecodeFormatCount++;
				break;
				
			case palmCodecImageGIF89aFile:
				gGIF89DecodeFormat[gGIF89DecodeFormatCount] = outputFormat;
				gGIF89DecodeFormatCount++;
				break;

			case palmCodecImageTIFFFile:
				gTIFFDecodeFormat[gTIFFDecodeFormatCount] = outputFormat;
				gTIFFDecodeFormatCount++;
				break;
		}
		
		switch(outputFormat)
		{
			case palmCodecImageJPEGFile:
				PrvAddColorFormat(inputFormat);
				gJPEGEncodeFormat[gJPEGEncodeFormatCount] = inputFormat;
				gJPEGEncodeFormatCount++;
				break;
				
			case palmCodecImageWBMPFile:
				PrvAddColorFormat(inputFormat);
				gWBMPEncodeFormat[gWBMPEncodeFormatCount] = inputFormat;
				gWBMPEncodeFormatCount++;
				break;				
		}
	}

Done:
	return errNone;	// Ignore any errors
}

/**
 * Set the text array for the Image formats
 */ 
static void PrvSetColorFormatList(UInt32 format)
{
	UInt32 oldColor = 0;
	UInt32 formatCount = 0;
	UInt32 *formatList = NULL;
	
	if(gCurrentColorFormatList)
		oldColor = gCurrentColorFormatList[gCurrentColorFormatSelection];
	
	switch(format)
	{
		case palmCodecImageJPEGFile:
			if(gImageInfo.encoding) {
				formatList = gJPEGEncodeFormat;
				formatCount = gJPEGEncodeFormatCount;
			} else {
				formatList = gJPEGDecodeFormat;
				formatCount = gJPEGDecodeFormatCount;
			}
			break;
			
		case palmCodecImageWBMPFile:
			if(gImageInfo.encoding) {
				formatList = gWBMPEncodeFormat;
				formatCount = gWBMPEncodeFormatCount;
			} else {
				formatList = gWBMPDecodeFormat;
				formatCount = gWBMPDecodeFormatCount;
			}
			break;
			
		case palmCodecImageGIF87aFile:
			formatList = gGIF87DecodeFormat;
			formatCount = gGIF87DecodeFormatCount;
			break;
			
		case palmCodecImageGIF89aFile:
			formatList = gGIF89DecodeFormat;
			formatCount = gGIF89DecodeFormatCount;
			break;

		case palmCodecImageTIFFFile:
			formatList = gTIFFDecodeFormat;
			formatCount = gTIFFDecodeFormatCount;
			break;
		
		case ALL_FORMATS:
			formatList = gAllColorFormatSupported;
			formatCount = gAllColorFormatSupportedCount;
			break;
				
		default:
			formatList = NULL;
			formatCount = 0;
			break;
	}
	
	// Quick clean-up
	if(gCurrentColorFormatTextList) {
		MemPtrFree(gCurrentColorFormatTextList);
		gCurrentColorFormatTextList = NULL;
		gCurrentColorFormatList = 0;
		gCurrentColorFormatCount = 0;
		gCurrentColorFormatSelection = 0;
	}
		
	// Populate our list
	gCurrentColorFormatList = formatList;
	gCurrentColorFormatCount = formatCount;

	if(formatList==NULL) return;
	
	if(gCurrentColorFormatCount)
	{
		gCurrentColorFormatTextList = MemPtrNew(formatCount * sizeof(Char*));
		MemSet(gCurrentColorFormatTextList, sizeof(UInt32) * formatCount, 0);
		while(formatCount--) {
			gCurrentColorFormatTextList[formatCount] = gColorFormatList[formatList[formatCount] - palmCodecImageBase];
			if(oldColor == formatList[formatCount])
				gCurrentColorFormatSelection = formatCount;
		}
	}
}

#if 0
#pragma mark -
#endif

/**
 *
 */
static void PrvSetOutputExtension(Char *fileName, UInt32 fileFormat)
{
	UInt32 appendPosition = StrLen(fileName);
	
	// Check for old extension
	if(*(fileName + appendPosition - 4) == '.')
		appendPosition -= 3;
	else
		fileName[appendPosition++] = '.';
		
	switch(fileFormat)
	{
		case palmCodecImageJPEGFile:
			StrCopy(fileName + appendPosition, "jpg");
			break;
			
		case palmCodecImageWBMPFile:
			StrCopy(fileName + appendPosition, "bmp");
			break;
	}
}

/**
 * Remember current UI settings
 */
static Err PrvSaveCurrentValues(FormType *frmP)
{
	//Err err = errNone;
	FieldType *fldP = NULL;
	Char* s;
	
	// Read the file names
	fldP = FrmGetPtr(frmP, MainInputField);
	StrCopy(gImageInfo.inputFileName, FldGetTextPtr(fldP));
	fldP = FrmGetPtr(frmP, MainOutputField);
	StrCopy(gImageInfo.outputFileName, FldGetTextPtr(fldP));
	
	// Read the width and height
	fldP = FrmGetPtr(frmP, MainWidthField);
	s = FldGetTextPtr(fldP) ;
	if( s )
		gImageInfo.width = StrAToI(s);	
	fldP = FrmGetPtr(frmP, MainHeightField);
	s = FldGetTextPtr(fldP) ;
	if( s )
	gImageInfo.height = StrAToI(s);
	
	return errNone;
}

/**
 * Set a field Editable
 */
/*
static inline void PrvSetFieldEditable(FormType *frmP, UInt16 ObjID)
{
	FieldType *fldP = NULL;
	FieldAttrType fldAttr;

	fldP = FrmGetPtr(frmP, ObjID);
	FldGetAttributes(fldP, &fldAttr);
	fldAttr.editable = 1;
	FldSetAttributes(fldP, &fldAttr);
}
*/

/*
 *
 * Set a field Not Editable
 */

/*
static inline void PrvSetFieldNotEditable(FormType *frmP, UInt16 ObjID)
{
	FieldType *fldP = NULL;
	FieldAttrType fldAttr;

	fldP = FrmGetPtr(frmP, ObjID);
	FldReleaseFocus(fldP);
	FldGetAttributes(fldP, &fldAttr);
	fldAttr.editable = 0;
	FldSetAttributes(fldP, &fldAttr);
}
*/

/**
 * Show/Hide Encode specific controls.
 */
static void PrvEncodeUI(FormType *frmP, Boolean encoding)
{	
	if(encoding) {
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFileFormatLabel));
		FrmShowObject(frmP, FrmGetObjectIndex(frmP, MainFileFormatPopTrigger));
	} else {
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainFileFormatLabel));
		FrmHideObject(frmP, FrmGetObjectIndex(frmP, MainFileFormatPopTrigger));
	}
}

/**
 * Refreshes the color format list
 */
static void PrvRefreshColorFormatList(FormType *frmP)
{
	ListType *lstP = FrmGetPtr(frmP, MainColorFormatList);
	ControlType *ctlP = FrmGetPtr(frmP, MainColorFormatPopTrigger);
	
	if(gCurrentColorFormatTextList && gCurrentColorFormatCount) {
		LstSetListChoices(lstP, gCurrentColorFormatTextList, gCurrentColorFormatCount);
		LstSetHeight(lstP, gCurrentColorFormatCount);
		LstSetSelection(lstP, gCurrentColorFormatSelection);
		CtlSetLabel(ctlP, gCurrentColorFormatTextList[gCurrentColorFormatSelection]);
	} else {
		LstSetListChoices(lstP, NULL, 0);
		LstSetHeight(lstP, 0);
		CtlSetLabel(ctlP, "");
	}
}

/**
 * Refreshes the file format list
 */
static void PrvRefreshFileFormatList(FormType *frmP)
{
	ListType *lstP = NULL;
	ControlType *ctlP = NULL;
	
	lstP = FrmGetPtr(frmP, MainFileFormatList);
	LstSetListChoices(lstP, gFileFormatTextList, 2);
	LstSetHeight(lstP, 2);
	LstSetSelection(lstP, gCurrentFileFormatSelection);
	ctlP = FrmGetPtr(frmP, MainFileFormatPopTrigger);
	CtlSetLabel(ctlP, gFileFormatTextList[gCurrentFileFormatSelection]);
}

/**
 * Switch settings to encoder.
 */
static void PrvSwitchToEncode(FormType *frmP)
{
	FieldType *fldP = NULL;
	ControlType *ctlP = NULL;
	
	gImageInfo.encoding = true;
		
	if(gImageInfo.fileFormat != palmCodecImageJPEGFile && gImageInfo.fileFormat != palmCodecImageWBMPFile)
		gImageInfo.fileFormat = palmCodecImageJPEGFile;
		
	gCurrentFileFormatSelection = (gImageInfo.fileFormat == palmCodecImageJPEGFile) ? 0 : 1;
	
	
	// Show Encode UI
	PrvEncodeUI(frmP, true);
	ctlP = FrmGetPtr(frmP, MainRunButton);
	CtlSetLabel(ctlP, gRunButtonLabel[1]);
	
	// Set the Color formats
	PrvSetColorFormatList(gImageInfo.fileFormat);
	PrvRefreshColorFormatList(frmP);
	PrvRefreshFileFormatList(frmP);
			
	// Set the old output file name as input
	StrCopy(gImageInfo.inputFileName, gImageInfo.outputFileName);
	StrCopy(gImageInfo.outputFileName, "/default");
	PrvSetOutputExtension(gImageInfo.outputFileName, gImageInfo.fileFormat);
	fldP = FrmGetPtr(frmP, MainInputField);
	SetFieldTextFromStr(fldP, gImageInfo.inputFileName, true);
	fldP = FrmGetPtr(frmP, MainOutputField);
	SetFieldTextFromStr(fldP, gImageInfo.outputFileName, true);
	
	
	
}

/**
 * Switch settings to decoder.
 */
static void PrvSwitchToDecode(FormType *frmP)
{
	FieldType *fldP = NULL;
	ControlType *ctlP = NULL;
	Err err=errNone;
	
	gImageInfo.encoding = false;
	gCurrentFileFormatSelection = 0;
	gCurrentColorFormatSelection = 0;
	
	// Show Encode UI
	PrvEncodeUI(frmP, false);
	ctlP = FrmGetPtr(frmP, MainRunButton);
	CtlSetLabel(ctlP, gRunButtonLabel[0]);
	
	// Set the old output file name as input
	StrCopy(gImageInfo.inputFileName, gImageInfo.outputFileName);
	StrCopy(gImageInfo.outputFileName, "/default.raw");	
	
	err = PrvFileOpenInput();
	if(err!=errNone) {
		// Oops!
		FrmCustomAlert(ErrorAlert, "Error opening Input file", NULL ,NULL);
	}
	
	fldP = FrmGetPtr(frmP, MainInputField);
	SetFieldTextFromStr(fldP, gImageInfo.inputFileName, true);
	fldP = FrmGetPtr(frmP, MainOutputField);
	SetFieldTextFromStr(fldP, gImageInfo.outputFileName, true);

	
	// Set the Color formats
	if(*gImageInfo.inputFileName)
		gImageInfo.fileFormat = PrvGetFileFormat(gImageInfo.inputFileName, gImageInfo.inputFileRef);
		
	PrvSetColorFormatList(gImageInfo.fileFormat);
	PrvRefreshColorFormatList(frmP);
	
}

#if 0
#pragma mark -
#endif

/**
 * Open input file
 */
static Err PrvFileOpenInput()
{
	Err err = errNone;
	UInt32 volumeIterator = 0;
	UInt16 firstVolume = 0;
	
	// Get first card
	volumeIterator = vfsIteratorStart;
	err = VFSVolumeEnumerate(&firstVolume, &volumeIterator);
	if( err == expErrEnumerationEmpty ) {
		goto Done;
	}
	
	// If we have a file path, open the file
	if(*gImageInfo.inputFileName)
	{
		// Make sure old references are closed
		if(gImageInfo.inputFileRef)
			VFSFileClose(gImageInfo.inputFileRef);
			
		err = VFSFileOpen(firstVolume, gImageInfo.inputFileName, vfsModeRead, &gImageInfo.inputFileRef);
		if( err ) goto Done;
	} else
		err = -1;

Done:	
	return err;
}

/**
 * Open output file
 */
static Err PrvFileOpenOutput()
{
	Err err = errNone;
	UInt32 volumeIterator = 0;
	UInt16 firstVolume = 0;
	FieldType *fldP = NULL;
	FormType *frmP = NULL;
	
	// Get first card
	volumeIterator = vfsIteratorStart;
	err = VFSVolumeEnumerate(&firstVolume, &volumeIterator);
	if( err == expErrEnumerationEmpty ) {
		goto Done;
	}
	
	// Read the file name
	frmP = FrmGetActiveForm();
	fldP = FrmGetPtr(frmP, MainOutputField);
	StrCopy( gImageInfo.outputFileName, FldGetTextPtr(fldP)); 
	
	// If we have a file path, open the file
	if(*gImageInfo.outputFileName)
	{
		// Make sure old references are closed
		if(gImageInfo.outputFileRef)
			VFSFileClose(gImageInfo.outputFileRef);
			
		err = VFSFileOpen(firstVolume, gImageInfo.outputFileName, vfsModeWrite|vfsModeCreate|vfsModeTruncate, &gImageInfo.outputFileRef);
		if( err ) goto Done;
	} else
		err = -1;

Done:	
	return err;
}

/**
 * File info callback.
 * This function is called by the file browser
 */
static void PrvGetInputFileInfoCallback(UInt16 volume, const Char* path, const Char* file)
{
	Err err = errNone;
	volume; //to supress CW warning
	
	// Create the full path name
	StrCopy(gImageInfo.inputFileName, path);

	// Internally, we expect full path
	StrCat(gImageInfo.inputFileName, file);
	
	// Open the selected file
	err = PrvFileOpenInput();
	if(err) {
		// Oops!
		FrmCustomAlert(ErrorAlert, "Error opening Input file", NULL ,NULL);
	} else {
		// Parse input file
		gImageInfo.fileFormat = PrvGetFileFormat(gImageInfo.inputFileName, gImageInfo.inputFileRef);
		PrvSetColorFormatList(gImageInfo.fileFormat);
	}
}

#if 0
#pragma mark -
#endif

/**
 * Start the decoder.
 * 1. Uses the file specified in the In File field.
 * 2. Decodes using the specified Color Format.
 * 3. Writes the RAW image to Out File.
 * Notes: If Out File is empty, just decodes with no output file.
 */
static Err PrvRunDecoder()
{
	Char text[32];
	Err err = errNone;
	UInt8 *bufferP = NULL;
	UInt32 inSize = 0;
	UInt32 outSize = 0;
	UInt32 written = 0;
	UInt32 decodeFormat = 0;
	UInt32 colorBits = 8, colorTablesize = 0;
	Boolean useOutFile = true;
	FormType *frmP = FrmGetActiveForm();
	PalmImageParamType *imageParamP = NULL;
	PalmFileFormatParamType *fileParamP = NULL;
	PalmImageINDEXParamType* indexImageParamP = NULL;
	
	// Open the input file
	err = PrvFileOpenInput();
	if(err) {
		FrmCustomAlert(ErrorAlert, "Cannot Open Input File !", NULL, NULL);
		goto Done;
	}
	
	// We don't do GIF decoding here theres a seperate sample for that
	if(gImageInfo.fileFormat == palmCodecImageGIF87aFile || gImageInfo.fileFormat == palmCodecImageGIF89aFile)
	{
		FrmCustomAlert(ErrorAlert, "This sample code doesn't handle GIF decoding !\nCheck MiniGIF for a better GIF sample.", NULL, NULL);
		goto Done;
	}
	
	// Open output file
	err = PrvFileOpenOutput();
	if(err) useOutFile = false;
		
	// Always use MemPtrNew, so that we are sure the pointers are 4 byte aligned
	fileParamP = MemPtrNew(sizeof(PalmFileFormatParamType)) ;
	imageParamP = MemPtrNew(sizeof(PalmImageParamType));
	
	// Output Buffer needs to be allocated depending on the output format type
	bufferP = MemPtrNew(BUFFER_SIZE);
	
	if(!fileParamP || !imageParamP || !bufferP) {
		FrmCustomAlert(ErrorAlert, "Not Enough Memory !", NULL, NULL);
		goto Done;
	}
	
	// Set File parameters
	fileParamP->fileLocationType = Swap32(palmCodecVFSFile);
	fileParamP->vfsFileRef = Swap32(gImageInfo.inputFileRef);
	
	// Get curent width and height
	// Note: width=0 and height=0 means that you want the correct
	//		 image size to be returned from the session	creation.
	gImageInfo.width = StrAToI(FldGetTextPtr(FrmGetPtr(frmP, MainWidthField)));	
	gImageInfo.height = StrAToI(FldGetTextPtr(FrmGetPtr(frmP, MainHeightField)));
	
	// Set Image parameters - For the offscreen window we use BGR since it's native.
	imageParamP->width = Swap32(gImageInfo.width);
	imageParamP->height = Swap32(gImageInfo.width);
	imageParamP->rowByte = Swap32(0);
	imageParamP->endianess = Swap32(palmCodecLITTLE_ENDIAN);
	decodeFormat = gCurrentColorFormatList[gCurrentColorFormatSelection];
		
	// Create Session
	err = CodecMgrCreateSession(gCodecMgrLibRefNum, 
			gImageInfo.fileFormat, fileParamP, 
			decodeFormat, imageParamP,
			&gImageInfo.session);
			
	if(err) {
		FrmCustomAlert(ErrorAlert, "Unable to create codec session !", NULL, NULL);
		goto Done;
	}
	
	// Get the width and height of the image sent back.
	gImageInfo.width = Swap32(imageParamP->width);
	gImageInfo.height = Swap32(imageParamP->height);
	gImageInfo.rowBytes = Swap32(imageParamP->rowByte); 
			
	// Update the UI
	StrIToA(text, gImageInfo.width);
	SetFieldTextFromStr(FrmGetPtr(frmP, MainWidthField), text, true);
	StrIToA(text, gImageInfo.height);
	SetFieldTextFromStr(FrmGetPtr(frmP, MainHeightField), text, true);
	
	// Check if we are decoding to a Indexed Image
	if( decodeFormat == palmCodecImageRGBIndex1 
     	   ||decodeFormat == palmCodecImageRGBIndex4
     	   ||decodeFormat == palmCodecImageRGBIndex8  )
    {
    
    	indexImageParamP = MemPtrNew( sizeof( PalmImageINDEXParamType ));
    	if( !indexImageParamP )
			goto Done;
    		
    	if( decodeFormat == palmCodecImageRGBIndex1)
    	 	colorBits = 1;
    	 else if ( decodeFormat == palmCodecImageRGBIndex4 )
    	 	colorBits = 4;
    		
		indexImageParamP->colorBits = Swap32(colorBits);
		colorTablesize = 4 * (1<< colorBits);		
		indexImageParamP->colorTable = (UInt8*)(MemPtrNew(colorTablesize ));
			
		if( !indexImageParamP->colorTable )
		{
			err = memErrNotEnoughSpace;
			goto Done;
		}
 		indexImageParamP->colorTable = (UInt8*)Swap32 ( (UInt32)indexImageParamP->colorTable);        	
    	indexImageParamP->imageDataP = (UInt8*)Swap32( (UInt32)bufferP );
			
    }
	
	
	// Decode the image
	// Note: Windows Bitmap are decoded from bottom to top.
	while(true)
	{
		// Set the size for the input and output
		inSize = sizeof(PalmFileFormatParamType);

   		outSize = BUFFER_SIZE;	
	
		if( decodeFormat == palmCodecImageRGBIndex1 
     	   ||decodeFormat == palmCodecImageRGBIndex4
     	   ||decodeFormat == palmCodecImageRGBIndex8  )
    	{
		
    		err = CodecMgrEncodeDecode(gCodecMgrLibRefNum,
				gImageInfo.session, fileParamP, &inSize, indexImageParamP, &outSize);
				
    	}
    	else
    	{    	
			err = CodecMgrEncodeDecode(gCodecMgrLibRefNum,
				gImageInfo.session, fileParamP, &inSize, bufferP, &outSize);							
		}
						
		// Write it to the card
		if(!err && useOutFile)
			err = VFSFileWrite(gImageInfo.outputFileRef, outSize, bufferP, &written);
			
		// If no more to read
		if(!inSize || err)
			break;
	}
	
	
	// Color table needs to be saved if the raw file which was decoded needs to be
	// used for testing encoding, else you can pass in your own color table while 
	// encoding the raw data, this sample just takes the decoded color table. 
	if( decodeFormat == palmCodecImageRGBIndex1 
     	   ||decodeFormat == palmCodecImageRGBIndex4
     	   ||decodeFormat == palmCodecImageRGBIndex8  )
    {
		// If no error save color table for encoding.
		gColorTableP = MemPtrNew( colorTablesize );
		if( gColorTableP )
			MemMove( gColorTableP, (void*)Swap32(indexImageParamP->colorTable) , colorTablesize );
		else
		 err = 	memErrNotEnoughSpace;
	}
	
Done:
	
	// Always close a session when you are done. 	
	if(gImageInfo.session)
	{
		CodecMgrDeleteSession(gCodecMgrLibRefNum, &gImageInfo.session);
		
		if( !err )
			 FrmCustomAlert( InfoAlert, "Decoding done succesfully","","");
		else
			 FrmCustomAlert( ErrorAlert, "Some errors encountered Decoding","","");
	}


	if(gImageInfo.inputFileRef) {
		VFSFileClose(gImageInfo.inputFileRef);
		gImageInfo.inputFileRef = NULL;
	}
	if(gImageInfo.outputFileRef) {
		VFSFileClose(gImageInfo.outputFileRef);
		gImageInfo.outputFileRef = NULL;
	}


	if(indexImageParamP)
	{
		indexImageParamP->imageDataP = NULL; // We are freeing bufferP
		if( indexImageParamP->colorTable )
			MemPtrFree( (void*)Swap32( indexImageParamP->colorTable ));		
		
		MemPtrFree( indexImageParamP);
	}

	if(fileParamP)
		MemPtrFree(fileParamP);
	if(imageParamP)
		MemPtrFree(imageParamP);
	if(bufferP)
		MemPtrFree(bufferP);
		
	return err;
}

/**
 * Decode the image for display.
 */
static Err PrvDecodeForDisplay()
{
	Err err = errNone;
	UInt32 inSize = 0;
	UInt32 outSize = 0;
	UInt8 *winBufferP = NULL;
	UInt8 *winBufferEndP = NULL;
	BitmapType *winBmpP = NULL;
	PalmImageParamType *imageParamP = NULL;
	PalmFileFormatParamType *fileParamP = NULL;
	Char msg[50];
	
	// Open the input file
	err = PrvFileOpenInput();
	if(err) {
		FrmCustomAlert(ErrorAlert, "Cannot Open Input File !", NULL, NULL);
		goto Done;
	}
	
	// We don't do GIF decoding here theres a seperate sample for that
	if(gImageInfo.fileFormat == palmCodecImageGIF87aFile || gImageInfo.fileFormat == palmCodecImageGIF89aFile)
	{
		FrmCustomAlert(ErrorAlert, "This sample code doesn't handle GIF decoding !\nCheck MiniGIF for a better GIF sample.", NULL, NULL);
		err = gifErr;
		goto Done; 
	}
	
	
	// Always use MemPtrNew, so that we are sure the pointers are 4 byte aligned
	fileParamP = MemPtrNew(sizeof(PalmFileFormatParamType)) ;
	imageParamP = MemPtrNew(sizeof(PalmImageParamType));
	
	
	if(!fileParamP || !imageParamP ) {
		FrmCustomAlert(ErrorAlert, "Not Enough Memory !", NULL, NULL);
		goto Done;
	}
	
	// Set File parameters
	fileParamP->fileLocationType = Swap32(palmCodecVFSFile);
	fileParamP->vfsFileRef = (FileRef)Swap32(gImageInfo.inputFileRef);
	
	// Set Image parameters - For the offscreen window we use BGR since it's native.
	imageParamP->width = Swap32(gWinWidth);
	imageParamP->height = Swap32(gWinHeight);
	imageParamP->rowByte = Swap32(0);
	imageParamP->endianess = Swap32(palmCodecLITTLE_ENDIAN);
		
	// Create Session
	err = CodecMgrCreateSession(gCodecMgrLibRefNum, 
			gImageInfo.fileFormat, fileParamP, 
			palmCodecImageBGR565, imageParamP,
			&gImageInfo.session);
			
	if(err) {
	    StrIToA(msg, err);
	    FrmCustomAlert(ErrorAlert, msg, NULL, NULL);
		FrmCustomAlert(ErrorAlert, "Unable to create codec session !", NULL, NULL);
		goto Done;
	}
	
	// Get the width and height of the image sent back
	gWinWidth = Swap32(imageParamP->width);
	gWinHeight = Swap32(imageParamP->height);

	// Create offscreen window (+1 to get ceil value)
	gWinH = WinCreateOffscreenWindow((gWinWidth+1) / 2, (gWinHeight+1) / 2, nativeFormat, &err);
	if(err) {
		FrmCustomAlert(ErrorAlert, "Not Enough Memory !", NULL, NULL);
		goto Done;
	}
		
	// Get bits - Assume rowByte is always width * 2
	winBmpP = WinGetBitmap(gWinH);
	winBufferP = BmpGetBits(winBmpP);
	winBufferEndP = winBufferP + (UInt32)(gWinWidth) * (UInt32)(gWinHeight) * 2;
	
	// Decode the image
	inSize = sizeof(PalmFileFormatParamType);		
	outSize = (UInt32)(gWinWidth) * (UInt32)(gWinHeight) * 2;
	
	err = CodecMgrEncodeDecode(gCodecMgrLibRefNum,
			gImageInfo.session, fileParamP, &inSize, winBufferP, &outSize);

	if(gImageInfo.session)
		CodecMgrDeleteSession(gCodecMgrLibRefNum, &gImageInfo.session);
	
Done:

	if(gImageInfo.inputFileRef) {
		VFSFileClose(gImageInfo.inputFileRef);
		gImageInfo.inputFileRef = NULL;
	}
	if(gImageInfo.outputFileRef) {
		VFSFileClose(gImageInfo.outputFileRef);
		gImageInfo.outputFileRef = NULL;
	}

	if(fileParamP)
		MemPtrFree(fileParamP);
	if(imageParamP)
		MemPtrFree(imageParamP);
		
	return err;
}

static UInt32 PrvGetBytesPerPixel(UInt32 format)
{
	switch(format)
	{
		case palmCodecImageARGB32:
		case palmCodecImageRGBA32:
		case palmCodecImageRGB32:
		case palmCodecImageBGRA32:		
		case palmCodecImageBGR32:
			return 4;
			
		case palmCodecImageRGB888:
		case palmCodecImageBGR888:
		case palmCodecImageYUV444:
		case palmCodecImageYCbCr444:
			return 3;		
		
		case palmCodecImageRGB565:
		case palmCodecImageRGB555:		
		case palmCodecImageBGR565:		
		case palmCodecImageBGR555:
		case palmCodecImageYUV422:
		case palmCodecImageYCbCr422:
			return 2;
					
		case palmCodecImageGRAY8:
			return 1;
	}
	
	return 0;
}

/**
 * Start the encoder.
 * 1. Uses the RAW image file specified in the In File field.
 * 2. Encodes using the specified Color Format and File Format.
 * 3. Writes the encoded image to Out File.
 * Notes:	If Out File is empty, just encodes with no output file.
 *			File Format and Color Format are inter-related. Changing
 *			one will affect the other.
 *			Make sure the width and height are correct.
 */
static void PrvRunEncoder()
{
	Err err = errNone;
	UInt8 *bufferP = NULL;
	UInt32 inSize = 0;
	UInt32 outSize = 0;
	UInt32 read = 0;
	UInt32 encodeFormat = 0;
	UInt32 bytesPerPixels = 0;
	UInt32 bufferSize = 0;
	UInt32 colorBits = 8, colorTablesize = 0;
	//Boolean useOutFile = true;
	FormType *frmP = FrmGetActiveForm();
	PalmImageParamType *imageParamP = NULL;
	PalmFileFormatParamType *fileParamP = NULL;
	PalmImageBMPParamType *bmpEncodeParamP = NULL;
	PalmImageJPEGEncodeParamType *jpgEncodeParamP = NULL;
	PalmImageINDEXParamType* indexImageParamP = NULL;
	
	// Open the input file
	err = PrvFileOpenInput();
	if(err) {
		FrmCustomAlert(ErrorAlert, "Cannot Open Input File !", NULL, NULL);
		goto Done;
	}
	
	// Get the actual File name and Open output file	 
	err = PrvFileOpenOutput();
			
	// Always use MemPtrNew, so that we are sure the pointers are 4 byte aligned
	fileParamP = MemPtrNew(sizeof(PalmFileFormatParamType)) ;
	imageParamP = MemPtrNew(sizeof(PalmImageParamType));
	jpgEncodeParamP = MemPtrNew(sizeof(PalmImageJPEGEncodeParamType));
	bmpEncodeParamP = MemPtrNew(sizeof(PalmImageBMPParamType));	
	
	if(!fileParamP || !imageParamP || !jpgEncodeParamP || !bmpEncodeParamP) {
		FrmCustomAlert(ErrorAlert, "Not Enough Memory !", NULL, NULL);
		goto Done;
	}
	
	// Set File parameters
	fileParamP->fileLocationType = Swap32(palmCodecVFSFile);
	fileParamP->vfsFileRef = Swap32(gImageInfo.outputFileRef);
	
	// Get curent width and height
	// Note: width=0 and height=0 means that you want the correct
	//		 image size to be returned from the session	creation.
	gImageInfo.width = StrAToI(FldGetTextPtr(FrmGetPtr(frmP, MainWidthField)));	
	gImageInfo.height = StrAToI(FldGetTextPtr(FrmGetPtr(frmP, MainHeightField)));
	
	// Get teh number of bytes per pixels
	encodeFormat = gCurrentColorFormatList[gCurrentColorFormatSelection];
	bytesPerPixels = PrvGetBytesPerPixel(encodeFormat);
	
	// Allocate the buffer
	bufferSize = gImageInfo.rowBytes * 2;
	bufferP = MemPtrNew(bufferSize);
	if(!bufferP) {
		FrmCustomAlert(ErrorAlert, "Not Enough Memory !", NULL, NULL);
		goto Done;
	}
	
	// Set Image parameters
	imageParamP->width = Swap32(gImageInfo.width);
	imageParamP->height = Swap32(gImageInfo.height);
	imageParamP->rowByte = Swap32(gImageInfo.rowBytes );
	imageParamP->endianess = Swap32(palmCodecLITTLE_ENDIAN);
	

	// Encode JPEG
	if(gImageInfo.fileFormat == palmCodecImageJPEGFile)
	{
		jpgEncodeParamP->quality = Swap32(100);
		jpgEncodeParamP->restartInterval = 0; /* ignored */
		jpgEncodeParamP->JPEGMode = Swap32(0); /* baseline only */
		jpgEncodeParamP->subSampling = 0; /* ignored */
		
		// Create Session
		err = CodecMgrCreateSession(gCodecMgrLibRefNum, 
				encodeFormat, imageParamP,
				gImageInfo.fileFormat, jpgEncodeParamP, 
				&gImageInfo.session);
	}
	// Encode WBMP
	else
	{
				
		if (encodeFormat == palmCodecImageRGBIndex8)
				bmpEncodeParamP->compression = palmCodecBMP_RLE8;
		else if (encodeFormat == palmCodecImageRGBIndex4)
				bmpEncodeParamP->compression = palmCodecBMP_RLE4;
		
		// Create Session
		err = CodecMgrCreateSession(gCodecMgrLibRefNum, 
				encodeFormat, imageParamP,
				gImageInfo.fileFormat, bmpEncodeParamP, 
				&gImageInfo.session);
	}
			
	if(err) {
		FrmCustomAlert(ErrorAlert, "Unable to create codec session !", NULL, NULL);
		goto Done;
	}
	
	// If its an IndexImage we are encoding ( currently only for BMP )
	// Check if we are decoding to a Indexed Image
	if( encodeFormat == palmCodecImageRGBIndex1 
     	   ||encodeFormat == palmCodecImageRGBIndex4
     	   ||encodeFormat == palmCodecImageRGBIndex8  )
    {
    
    	indexImageParamP = MemPtrNew( sizeof( PalmImageINDEXParamType ));
    	if( !indexImageParamP )
			goto Done;
    	
    	indexImageParamP->width = Swap32(gImageInfo.width);
		indexImageParamP->height = Swap32(gImageInfo.height);;
		indexImageParamP->rowByte = Swap32(gImageInfo.rowBytes); 
		indexImageParamP->endianess = 0;	
    	if( encodeFormat == palmCodecImageRGBIndex1)
    	 	colorBits = 1;
    	 else if ( encodeFormat == palmCodecImageRGBIndex4 )
    	 	colorBits = 4;
    		
		indexImageParamP->colorBits = Swap32(colorBits);
		colorTablesize = 4 * (1<< colorBits);
		indexImageParamP->colorTable = (UInt8*)(MemPtrNew(colorTablesize ));
			
		if( !indexImageParamP->colorTable )
			goto Done;
		
		// The color table was saved during decoding	
		MemMove( indexImageParamP->colorTable, gColorTableP, colorTablesize );
		
		indexImageParamP->colorTable = (UInt8*)Swap32 ( (UInt32)indexImageParamP->colorTable);        	
    	indexImageParamP->imageDataP = (UInt8*)Swap32( (UInt32)bufferP );
			
    }
	
	// Encode the image
	outSize = sizeof(PalmFileFormatParamType);
	
	while(true)
	{
		inSize = read;
		
		// Write it to the card
		err = VFSFileRead(gImageInfo.inputFileRef, bufferSize - inSize, bufferP + inSize, &read);
			
		// Set the size for the input and output
		inSize += read;
		read = inSize;
		
		if( encodeFormat == palmCodecImageRGBIndex1 
     	   ||encodeFormat == palmCodecImageRGBIndex4
     	   ||encodeFormat == palmCodecImageRGBIndex8  )
     	{		
			err = CodecMgrEncodeDecode(gCodecMgrLibRefNum,
				gImageInfo.session, indexImageParamP, &inSize, fileParamP, &outSize);
		}
		else
		{
			err = CodecMgrEncodeDecode(gCodecMgrLibRefNum,
				gImageInfo.session, bufferP, &inSize, fileParamP, &outSize);
		}
		
		// If no more to read
		if(!outSize || err)
			break;
			
		// Move leftover
		read -= inSize;
		MemMove(bufferP, bufferP + inSize, read);
	}
	
	
Done:
	
	// Do this first before you close the file, DeleteSession writes some
	// into the file esp bmp headers.
	if(gImageInfo.session)
	{
		CodecMgrDeleteSession(gCodecMgrLibRefNum, &gImageInfo.session);
		
		if( !err )
		 FrmCustomAlert( InfoAlert, "Encoding done succesfully","","");
		else
		 FrmCustomAlert( ErrorAlert, "Some errors encountered Encoding","","");
	}


	if(gImageInfo.inputFileRef) {
		VFSFileClose(gImageInfo.inputFileRef);
		gImageInfo.inputFileRef = NULL;
	}
	if(gImageInfo.outputFileRef) {
		VFSFileClose(gImageInfo.outputFileRef);
		gImageInfo.outputFileRef = NULL;
	}
	
	if(indexImageParamP)
	{
		indexImageParamP->imageDataP = NULL; // We are freeing bufferP
	
		if( indexImageParamP->colorTable )
			MemPtrFree( (void*)Swap32( indexImageParamP->colorTable ));		
		
		MemPtrFree( indexImageParamP);		
		
	}
	
	if(fileParamP)
		MemPtrFree(fileParamP);
	if(imageParamP)
		MemPtrFree(imageParamP);
	if(bufferP)
		MemPtrFree(bufferP);
	if(bmpEncodeParamP)
		MemPtrFree(bmpEncodeParamP);
	if(jpgEncodeParamP)
		MemPtrFree(jpgEncodeParamP);
	if( gColorTableP )
	{
		MemPtrFree( gColorTableP );
		gColorTableP = NULL;	
 	}
}

/**
 * Display an image
 */
static void PrvDisplayImage()
{
	Err err = errNone;
	
	// Temp - Don't do anything in this case
	if(gImageInfo.encoding)
		return;
	
	if(gImageInfo.fitToScreen)
		WinGetDisplayExtent(&gWinWidth, &gWinHeight);
		
	gWinWidth *= 2;
	gWinHeight *= 2;
	
	// Decode the image
	err = PrvDecodeForDisplay();
	if(err) {
		// if(gWinH) WinDeleteWindow(gWinH, false);
		// gWinH = NULL;
		return;
	}
	
	// Display the offscreen window
	FrmGotoForm(DisplayForm);
}
#if 0
#pragma mark -
#endif

/**
 * Initialize the display form
 */
static void DisplayFormInit()
{
	Coord x, y;
	RectangleType rect = { { 0, 0 }, { 0, 0 } };
	
	gImageX = 0;
	gImageY = 0;
	
	// Check if we can pan in the image
	if(gWinWidth > 320 || gWinHeight > 320)
		gPanning = true;
	else
		gPanning = false;
		
	// Image size
	rect.extent.x = gWinWidth / 2;
	if(rect.extent.x > 160)	rect.extent.x = 160;
	rect.extent.y = gWinHeight / 2;
	if(rect.extent.y > 160)	rect.extent.y = 160;
	
	// Center the image
	x = 80 - rect.extent.x / 2;
	y = 80 - rect.extent.y / 2;
		
	// Display the capture image
	WinCopyRectangle(gWinH, WinGetDisplayWindow(), &rect, x, y, winPaint);
}

/**
 * Track teh pen and move the image
 */
static void PrvTrackPenDown(EventType *eventP)
{
	UInt32 startTick = TimGetTicks();
	Int16 oldX, oldY, newX, newY;
	Boolean penIsDown = eventP->penDown;
	oldX = eventP->screenX;
	oldY = eventP->screenY;
	
	while(penIsDown)
	{
		EvtGetPen(&newX, &newY, &penIsDown);
		
		if(newX == oldX && newY == oldY)
			continue;
			
		gImageX += (oldX - newX);
		gImageY += (oldY - newY);
		
		if(gImageX < 0)	gImageX = 0;
		if(gImageX > (gWinWidth / 2 - 160))	gImageX = gWinWidth / 2 - 160;
		if(gImageY < 0)	gImageY = 0;
		if(gImageY > (gWinHeight / 2 - 160)) gImageY = gWinHeight / 2 - 160;
		
		// If panning is enabled
		if(gPanning) {
			RectangleType rect = { { 0, 0 }, { 160, 160 } };
			
			// Image Coord
			rect.topLeft.x = gImageX;
			rect.topLeft.y = gImageY;
			
			// Display the capture image
			WinCopyRectangle(gWinH, WinGetDisplayWindow(), &rect, 0, 0, winPaint);
		}
		
		oldX = newX;
		oldY = newY;
	}
	
	if(TimGetTicks() - startTick < TRACK_TIME) {
		FrmGotoForm(MainForm);
	}
}

/***********************************************************************
 *
 * FUNCTION:    		PrvHandleMenu
 *
 * DESCRIPTION: 		Deals with menu commands issued
 *
 * PARAMETERS:  		itemID -> ID of the Menu which has been selected
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvHandleMenu(UInt16 itemID)
{
	//Err err = errNone;
	FormType *frmAboutP = NULL;
    const RGBColorType black = { 0, 0, 0 ,0 };	
    const RGBColorType white = { 0, 255, 255 ,255 };	
    const RectangleType	currentColorRect = { {27, 142}, { 19, 17} };

	
	switch(itemID)
	{		
		case HelpAboutMiniImage:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmDeleteForm(frmAboutP);
			
			WinSetForeColorRGB(&white, NULL);
			WinSetTextColorRGB(&black, NULL);
			WinDrawRectangle(&currentColorRect, 0);
			
			break;
			
	}
}

/**
 * DisplayFormHandleEvent
 */
static Boolean DisplayFormHandleEvent(EventType *eventP)
{
	//Err err = errNone;
	Boolean handled = false;
	FormType *frmP = FrmGetActiveForm();

	switch(eventP->eType)
	{
		case frmOpenEvent:
			FrmDrawForm(frmP);
			DisplayFormInit();
			handled = true;
			break;
			
		case frmCloseEvent:
			if(gWinH) WinDeleteWindow(gWinH, false);
			break;
			
		case penDownEvent:
			PrvTrackPenDown(eventP);
			handled = true;
			break;
			
		default:
			break;
	}
	
	return handled;	
}

#if 0
#pragma mark -
#endif

/**
 * Initialize the form
 */
static void MainFormInit(FormType *frmP)
{
	Char text[32];
	FieldType *fldP = NULL;
	ControlType *ctlP = NULL;
	
	// Set files
	fldP = FrmGetPtr(frmP, MainInputField);
	SetFieldTextFromStr(fldP, gImageInfo.inputFileName, false);
	fldP = FrmGetPtr(frmP, MainOutputField);
	SetFieldTextFromStr(fldP, gImageInfo.outputFileName, false);
		
	// Set the current mode
	if(gImageInfo.encoding)	{
		ctlP = FrmGetPtr(frmP, MainEncodePushButton);
		CtlSetValue(ctlP, true);
		ctlP = FrmGetPtr(frmP, MainRunButton);
		CtlSetLabel(ctlP, gRunButtonLabel[1]);
		PrvEncodeUI(frmP, true);
	} else {
		ctlP = FrmGetPtr(frmP, MainDecodePushButton);
		CtlSetValue(ctlP, true);
		ctlP = FrmGetPtr(frmP, MainRunButton);
		CtlSetLabel(ctlP, gRunButtonLabel[0]);
		
		if(gImageInfo.width)
			StrIToA(text, gImageInfo.width);
		else
			StrCopy(text, "MAX");
		SetFieldTextFromStr(FrmGetPtr(frmP, MainWidthField), text, true);
		if(gImageInfo.height)
			StrIToA(text, gImageInfo.height);
		else
			StrCopy(text, "MAX");
		SetFieldTextFromStr(FrmGetPtr(frmP, MainHeightField), text, true);
	}
	
	// Fit to Screen
	ctlP = FrmGetPtr(frmP, MainFitToScreenCheckbox);
	CtlSetValue(ctlP, gImageInfo.fitToScreen);
	
	// Populate the Color formats
	PrvRefreshColorFormatList(frmP);
	PrvRefreshFileFormatList(frmP);
}

/**
 * MainFormHandleEvent
 */
static Boolean MainFormHandleEvent(EventType * eventP)
{
	Boolean handled = false;
	//ControlType *ctlP = NULL;
	FormType *frmP = FrmGetActiveForm();
    
	switch(eventP->eType)
	{
		case frmOpenEvent:
			MainFormInit(frmP);
			
		    // Make sure empty text field is added to navigation order and set focus to encode button
			FrmSetNavEntry(frmP, MainInputField, MainEncodePushButton, MainEncodePushButton, MainDisplayButton, 0);
			//Setting focus on MainDisplayButton
			FrmNavObjectTakeFocus(frmP, MainDisplayButton);
			FrmDrawForm(frmP);
			handled = true;
			break;
			
		case frmUpdateEvent:
			break;
			
		case frmCloseEvent:
			PrvSaveCurrentValues(frmP);
			break;
			
		case popSelectEvent:
			switch(eventP->data.popSelect.controlID)
			{
				case MainColorFormatPopTrigger:
					gCurrentColorFormatSelection = eventP->data.popSelect.selection;
					gImageInfo.colorFormat = gCurrentColorFormatList[gCurrentColorFormatSelection];
					break;
					
				case MainFileFormatPopTrigger:
					gCurrentFileFormatSelection = eventP->data.popSelect.selection;
					gImageInfo.fileFormat = gFileFormatList[gCurrentFileFormatSelection];
					PrvSetColorFormatList(gImageInfo.fileFormat);
					PrvRefreshColorFormatList(frmP);
					break;
			}
			break;
			
		case ctlSelectEvent:
			switch(eventP->data.ctlSelect.controlID)
			{
				case MainInputGraphicButton:
					// Sets the callback function and return form
					FileBrowserSetCallback(&PrvGetInputFileInfoCallback, MainForm);
					FrmGotoForm(FileBrowserForm);
					handled = true;
					break;
					
				case MainDisplayButton:
					PrvDisplayImage();
					handled = true;
					break;
					
				case MainRunButton:
					if(gImageInfo.encoding)
						PrvRunEncoder();
					else
						PrvRunDecoder();
					handled = true;
					break;
					
				case MainDecodePushButton:
					if(gImageInfo.encoding)
						PrvSwitchToDecode(frmP);
					break;
					
				case MainEncodePushButton:
					if(!gImageInfo.encoding)
						PrvSwitchToEncode(frmP);
					break;
					
				case MainFitToScreenCheckbox:
					gImageInfo.fitToScreen = eventP->data.ctlSelect.on;
					break;
			}
			break;
			
		case menuEvent:
			PrvHandleMenu(eventP->data.menu.itemID);
			FrmDrawForm(frmP);
			handled = true;
			break;
		
		default:
			break;
	}
	
	return handled;
}

#if 0
#pragma mark -
#endif

/**
 * AppHandleEvent
 */
static Boolean AppHandleEvent(EventType * eventP)
{
    UInt16 formId;
    FormType * frmP;
    
    if (eventP->eType == frmLoadEvent)
    {
        formId = eventP->data.frmLoad.formID;
        frmP = FrmInitForm(formId);
        FrmSetActiveForm(frmP);

        switch (formId)
        {
        case MainForm:
            FrmNavObjectTakeFocus(frmP, MainDecodePushButton); 
            FrmSetEventHandler(frmP, MainFormHandleEvent);
            break;
            
        case DisplayForm:
            FrmNavObjectTakeFocus(frmP, MainDecodePushButton); 
            FrmSetEventHandler(frmP, DisplayFormHandleEvent);
            break;
            
        case FileBrowserForm:
            FrmSetEventHandler(frmP, FileBrowserFormHandleEvent);
            break;

        default:
            break;

        }
        return true;
    }

    return false;
}

/**
 * AppEventLoop
 */
static void AppEventLoop(void)
{
	UInt16 error = errNone;
	EventType event;

    do
    {
    	EvtGetEvent(&event, evtWaitForever);
    	
    	if (! SysHandleEvent(&event)) {
    		if (! MenuHandleEvent(0, &event, &error)) {
    			if (! AppHandleEvent(&event))
    				FrmDispatchEvent(&event);
    		}
        }
        
    } while (event.eType != appStopEvent);
}

/**
 * AppStart
 * Called when the application starts.
 */
static Err AppStart(void)
{
	Err err = errNone;
	
	// Clear the global
	MemSet(&gImageInfo, sizeof(ImageInfo), 0);

	// Try to find the hardware utils library
	err = SysLibFind(kCodecMgrLibName, &gCodecMgrLibRefNum);	
	if(err != errNone)
		err = SysLibLoad(kCodecMgrLibType, kCodecMgrLibCreator, &gCodecMgrLibRefNum);
	
	if(err == errNone)
		err = CodecMgrOpen(gCodecMgrLibRefNum);
	
	if(err) {
		FrmAlert(NoCPMLibAlert);
		goto Done;
	}
	
	// Load known formats
	PrvLoadFormats();
	
	// Set default values
	gImageInfo.inputFileRef = NULL;
	gImageInfo.outputFileRef = NULL;
	gImageInfo.session = NULL;
	
	gImageInfo.width = 0;
	gImageInfo.height = 0;
	gImageInfo.fitToScreen = true;
	gImageInfo.encoding = false;
		
	StrCopy(gImageInfo.outputFileName, "/default.raw");
	
	// Switch to 16 bit color
	WinScreenMode(winScreenModeGet, NULL, NULL, &gPreviousScreenDepth, NULL);
	WinScreenMode(winScreenModeSet, NULL, NULL, &gScreenDepth, NULL);
		
Done:			
	return err;
}

/**
 * AppStop
 * Called when the application stops.
 */
static void AppStop(void)
{
	Err err = errNone;
	
    FrmCloseAllForms();
    
    // Close files
	if(gImageInfo.inputFileRef)
		VFSFileClose(gImageInfo.inputFileRef);
	if(gImageInfo.outputFileRef)
		VFSFileClose(gImageInfo.outputFileRef);
	
	// Close the library
	if(gCodecMgrLibRefNum != sysInvalidRefNum)
	{
		err = CodecMgrClose(gCodecMgrLibRefNum);
		if (err == errNone)
			err = SysLibRemove(gCodecMgrLibRefNum);
	}

	// Restore previous color mode
	WinScreenMode(winScreenModeSet, NULL, NULL, &gPreviousScreenDepth, NULL);
	
	// Clean memory
	if( gColorTableP )
		MemPtrFree( gColorTableP );
		
	if(gCurrentColorFormatTextList) 
		MemPtrFree(gCurrentColorFormatTextList);
		
	
}

#pragma warn_a5_access on

//
// RomVersionCompatible
//
static Err RomVersionCompatible(UInt32 requiredVersion, UInt16 launchFlags)
{
    UInt32 romVersion;

    FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
    if (romVersion < requiredVersion)
    {
        if ((launchFlags & 
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
            (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
        {
            FrmAlert (RomIncompatibleAlert);

            if (romVersion < kPalmOS20Version)
            {
                AppLaunchWithCommand(
                    sysFileCDefaultApp, 
                    sysAppLaunchCmdNormalLaunch, NULL);
            }
        }

        return sysErrRomIncompatible;
    }

    return errNone;
}

//
// PilotMain
//
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
    Err error;
    //SysNotifyParamType *notifyParamP = NULL;
    cmdPBP; //to supress CW warning

    error = RomVersionCompatible (ourMinVersion, launchFlags);
    if (error) return (error);

    switch (cmd)
    {
    case sysAppLaunchCmdNormalLaunch:
        error = AppStart();
        if (error) 
            return error;

        FrmGotoForm(MainForm);
        AppEventLoop();
        
        AppStop();
        break;
        
    default:
        break;
    }

    return errNone;
}

#pragma warn_a5_access reset


/*@}*/
