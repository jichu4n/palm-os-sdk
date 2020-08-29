/***********************************************************************
 *
 * Copyright (c) 2003 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/
/**
 *  @ingroup FotoFinish
 **/

/**
 * @file MainForm.c
 * This file contains the event-handling code for the MainForm
 */

/** @name
 *
**/
/*@{*/


/***********************************************************************
 *
 * File:
 *	MainForm.c
 *
 * Description:
 *	Contains all event-handling code associated with MainForm
 *	Sample code for the Photo API
 *
 *	Version 1.0 	- Initial Revision (08/18/03)
 *
 ***********************************************************************/

#include <PalmOS.h>
#include "MainForm.h"
#include "utils.h"
//#include "PalmOnePhotoCommon.h"
#include "FotoFinishRsc.h"
#include "AddTextForm.h"
#include "SavePhotoForm.h"
#include "FotoFinish.h"
#include "DetailsForm.h"
#include "AlbumsOrganiseForm.h"

// Dimensions of the Draw-Area
#define	DRAW_AREA_WIDTH			131
#define	DRAW_AREA_HEIGHT		122
#define	DRAW_AREA_TOPLEFT_X 	27
#define	DRAW_AREA_TOPLEFT_Y		18

// Dimensions of the color-palette
#define	COLOR_PALETTE_WIDTH			110
#define	COLOR_PALETTE_HEIGHT		18
#define	COLOR_PALETTE_TOPLEFT_X 	48
#define	COLOR_PALETTE_TOPLEFT_Y		142


// Dimensions of each colour palette element
#define SQ_WIDTH	6
#define SQ_HEIGHT	7
#define	GAP_WIDTH	1
#define GAP_HEIGHT	2

// Maximum number of operations in the Operations-Queue
#define	MAX_OPS	10

// Maximum number of points which need to be tracked in a free-line operation
#define MAX_POINTS 1000

// Reference number of the library. Initialized in FotoFinish.c
extern	UInt16 	gLibRefNum;

// Pointer to the text which can be drawn on the picture
extern	Char	*gTextToAdd;

// Pointer to the list which contains a list of albums
extern	ListType *gLstP;

// Number of albums on the device
extern UInt16	gAlbumCount;

// Contains the array of album-names
extern Char 	**gAlbumList;

// Variable which holds the number of images in an album (Which is used by DetailsForm)
UInt32 gNumImagesInAlbum = 0;

// Index of a photo in an album (Used by DetailsForm)
UInt16 gPhotoIndex = 0;

// Global variable of type PalmPhotoSelectionParam -> Used to keep
// track of the photo selected by the user (in DetailsForm)
extern PalmPhotoSelectionParam	gDetailsSelectionParam;

// Handle to the picture which is currently stored in the back buffer. 
// (i.e. the one selected by the user)
static PalmPhotoHandle	gSelectedPhotoH;

// Global variable of type PalmPhotoSelectionParam -> Used to keep
// track of the photo selected by the user
static PalmPhotoSelectionParam gSelectionParam;

// Queue of operations which are performed by the user
static Operation gOpQueue[MAX_OPS];

// Back-buffer which contains the entire picture
WinHandle	gBackBufferWinH = NULL;
// Back-Buffer which contains the view-port. This will display a clipping of gBackBufferWinH
// with all the draw operations performed on it
WinHandle	gViewPortBackBufferWinH = NULL;

// Rectangle which is used to clip various portions of gBackBufferWinH, to displayo on the screen
RectangleType	gDrawAreaRect = { {0,0}, {DRAW_AREA_WIDTH + 2, DRAW_AREA_HEIGHT + 2}};
// Rectangle which displays the current color selected
RectangleType	gCurrentColorRect = { {27, 142}, { 19, 17} };
// Rectangle which is used as the clipping area while drawing...if the image is smaller than
// the drawing area, the co-ordinates of gScreenDrawAreaRect are modified so that the 
// the image is centred in the drawing area
RectangleType	gScreenDrawAreaRect = { {DRAW_AREA_TOPLEFT_X, DRAW_AREA_TOPLEFT_Y}, 
										{DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT}};

// Current color which is selected
RGBColorType	gCurrentColor = { 0, 0, 0, 0 };

// Co-ordinates used to keep track of the pen, while it is moving
static Int16	gOldX = 0;
static Int16	gNewX = 0;
static Int16	gOldY = 0;
static Int16	gNewY = 0;
static Int16	gDiffX = 0;
static Int16	gDiffY = 0;

// Variable which holds the current state -> For eg. Text/Timestamp/Straight Line/Free line etc
static	UInt8	gSelection = 0;

// This parameter denotes the index at which a new operation has to be added to the queue
static Int16	gOpLastIndex = -1;

// This denotes the current operation which the user has reverted to (It can be different
// from gOpLastIndex in the situation in which multiple undo/redo operations have been
// performed)
static Int16 	gOpIndex = -1;

// Global pointer to the x-coordinates needed to draw a free-line.
static Coord	*gXCoordP = NULL;

// Global pointer to the y-coordinates needed to draw a free-line.
static Coord	*gYCoordP = NULL;

// Contains the back-buffer's width in standard co-ordinates system
UInt32	gPhotoWidth = 0;
// Contains the back-buffer's height in standard co-ordinates system
UInt32 	gPhotoHeight = 0;


/***********************************************************************
 *
 * FUNCTION:    	DeInitOpQueue
 *
 * DESCRIPTION: 	Frees all the memory in the Operations Queue
 *
 * PARAMETERS:  	None
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void DeInitOpQueue()
{
	Int16 count = 0;
	
	// Frees all memory allocated for the operations queue
	for(count = 0; count <= gOpLastIndex; count++)
	{
		if(gOpQueue[count].OpCode == FREELINE_OP)
		{
		 	if(gOpQueue[count].Op.freeLineOp.x)
		 	{
		 		MemPtrFree(gOpQueue[count].Op.freeLineOp.x);
		 		gOpQueue[count].Op.freeLineOp.x = NULL;
		 	}
		 	if(gOpQueue[count].Op.freeLineOp.y)
		 	{
		 		MemPtrFree(gOpQueue[count].Op.freeLineOp.y);
				gOpQueue[count].Op.freeLineOp.y = NULL;
			}
		}
	}				 		
}


/***********************************************************************
 *
 * FUNCTION:    	CommitToBackBuffer
 *
 * DESCRIPTION: 	Draws all operations to the back-buffer which contains
 *					the Jpeg picture's contents	
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

void CommitToBackBuffer()
{
	Int16 count = 0;
	WinHandle mainWindow = NULL;
	
	mainWindow = WinSetDrawWindow(gBackBufferWinH);
	WinPushDrawState();
	
	// All the draw operations upto the last 'undo' are committed to the back-buffer
	for(count = 0; count <= gOpIndex; count++)
	{
		if(gOpQueue[count].OpCode == STLINE_OP)
		{
			WinSetForeColorRGB(&gOpQueue[count].color, NULL);
				 	
			WinDrawLine(gOpQueue[count].Op.stLineOp.startX, 
						gOpQueue[count].Op.stLineOp.startY,
						gOpQueue[count].Op.stLineOp.endX,
						gOpQueue[count].Op.stLineOp.endY);
			
			WinSetForeColorRGB(&gCurrentColor, NULL);

		}			
			
		if(gOpQueue[count].OpCode == TEXT_OP)
		{
			WinSetDrawMode(winOverlay);
			
			WinSetTextColorRGB(&gOpQueue[count].color, NULL);
		
			WinPaintChars(gOpQueue[count].Op.textOp.text, StrLen(gOpQueue[count].Op.textOp.text), 
							gOpQueue[count].Op.textOp.x ,
							gOpQueue[count].Op.textOp.y );
				
			WinSetTextColorRGB(&gCurrentColor, NULL);
			
		}

		if(gOpQueue[count].OpCode == FREELINE_OP)
		{
			UInt16 pointsCount = 0;
			
			WinSetForeColorRGB(&gOpQueue[count].color, NULL);
			
			for(pointsCount = 0; pointsCount < gOpQueue[count].Op.freeLineOp.numPoints; pointsCount+=2)
			{
				WinDrawLine(gOpQueue[count].Op.freeLineOp.x[pointsCount], 
							gOpQueue[count].Op.freeLineOp.y[pointsCount], 
							gOpQueue[count].Op.freeLineOp.x[pointsCount+1], 
							gOpQueue[count].Op.freeLineOp.y[pointsCount+1]);
			}
			
			WinSetForeColorRGB(&gCurrentColor, NULL);
		}

	}
	
	WinPopDrawState();
	WinSetDrawWindow(mainWindow);
}

/***********************************************************************
 *
 * FUNCTION:    	PrvDrawOps
 *
 * DESCRIPTION: 	Draws all the draw-operations on the view-port buffer. 
 *					It commits all operations in the Operations Queue until
 *					gOpIndex (gOpIndex is the index of the last operation which
 *					the user is at, after performing undo's etc.)	
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvDrawOps()
{
	Int16 count = 0;
	WinHandle mainWindow = NULL;
	
	// Sets the draw window as the view-port back buffer
	mainWindow = WinSetDrawWindow(gViewPortBackBufferWinH);
	
	WinPushDrawState();
	
	for(count = 0; count <= gOpIndex; count++)
	{
		// All draw operations to the view-port buffer are done with respect to the co-ordinates
		// of the view-port buffer. So you subtract gDrawAreaRect.topLeft.x / y from the x, y 
		// co-ordinates respectively
		
		// Straight Line
		if(gOpQueue[count].OpCode == STLINE_OP)
		{
			WinSetForeColorRGB(&gOpQueue[count].color, NULL);
				 	
			WinDrawLine(gOpQueue[count].Op.stLineOp.startX  - gDrawAreaRect.topLeft.x ,
						gOpQueue[count].Op.stLineOp.startY  - gDrawAreaRect.topLeft.y ,
						gOpQueue[count].Op.stLineOp.endX  - gDrawAreaRect.topLeft.x ,
						gOpQueue[count].Op.stLineOp.endY - gDrawAreaRect.topLeft.y);
			
			WinSetForeColorRGB(&gCurrentColor, NULL);

		}			
		
		// Text	
		if(gOpQueue[count].OpCode == TEXT_OP)
		{
			WinSetDrawMode(winOverlay);
			
			WinSetTextColorRGB(&gOpQueue[count].color, NULL);
		
			WinPaintChars(gOpQueue[count].Op.textOp.text, StrLen(gOpQueue[count].Op.textOp.text), 
							gOpQueue[count].Op.textOp.x  - gDrawAreaRect.topLeft.x ,
							gOpQueue[count].Op.textOp.y  - gDrawAreaRect.topLeft.y);
				
			WinSetTextColorRGB(&gCurrentColor, NULL);
			
		}

		// Free-line
		if(gOpQueue[count].OpCode == FREELINE_OP)
		{
			UInt16 pointsCount = 0;
			
			WinSetForeColorRGB(&gOpQueue[count].color, NULL);
			
			for(pointsCount = 0; pointsCount < gOpQueue[count].Op.freeLineOp.numPoints; pointsCount+=2)
			{
				WinDrawLine(gOpQueue[count].Op.freeLineOp.x[pointsCount] - gDrawAreaRect.topLeft.x , 
							gOpQueue[count].Op.freeLineOp.y[pointsCount]  - gDrawAreaRect.topLeft.y, 
							gOpQueue[count].Op.freeLineOp.x[pointsCount+1] - gDrawAreaRect.topLeft.x , 
							gOpQueue[count].Op.freeLineOp.y[pointsCount+1] - gDrawAreaRect.topLeft.y);
			}
			WinSetForeColorRGB(&gCurrentColor, NULL);
		}

	}
	
	WinPopDrawState();
	WinSetDrawWindow(mainWindow);
}

/***********************************************************************
 *
 * FUNCTION:    		PrvAddOpToQueue
 *
 * DESCRIPTION: 		Adds an operation which the user has performed
 *						to the Operations queue. If the queue if full, the first 
 *						element of the queue (element 0) is committed to the back-buffer
 *						and all elements are shifted to the left by 1, to make way
 *						for the new element at the end of the queue. The add-operation
 *						is always done at index gOpLastIndex (which always represents the 
 *						point at which the addition has to be made). If an undo has been done,
 *						gOpIndex which will contain the index of the operation, that the
 *						user has reverted to, and so gOpLastIndex will be set to
 *						gOpIndex
 *
 * PARAMETERS:  		startX -> x co-ordinate of point at which a straight-line/text
 *								  has to be drawn
 *						startY -> y co-ordinate of point at which a straight-line/text
 *								  has to be drawn
 *						endX ->	  x co-ordinate of point at which a straight-line to be drawn
 *								  (This parameter is ignored if text/free-line has to be drawn)
 *						endY ->   y co-ordinate of point at which a straight-line to be drawn
 *								  (This parameter is ignored if text/free-line has to be drawn)
 *						text -> Pointer to the text that has to be drawn (Ignored for straight
 *								line and free-line operations)
 *						xCoord -> Pointer to the array of x-coordinates which are used to draw  
 *								  free lines
 *						yCoord ->  Pointer to the array of y-coordinates which are used to draw  
 *								  free lines
 *						numPoints -> Number of points to be drawn for a free-line
 *						opCode -> Can be one of three values denoting straight line/ text / or
 *									free-line operations
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvAddOpToQueue(Coord startX, Coord startY, Coord endX, Coord endY, Char *text, 
							Coord *xCoord, Coord *yCoord, UInt16 numPoints, UInt8 opCode)
{
	Int16 count = 0;

	//Undo has taken place
	if(gOpIndex < gOpLastIndex)	
		gOpLastIndex = gOpIndex;

	// If the queue is full, commit the first element to the Back-Buffer and then proceed 
	// to add the new element to the end of the queue
	if(gOpLastIndex == MAX_OPS - 1)
	{
		// Commit the first element to the back-buffer
		WinHandle mainWindow = WinSetDrawWindow(gBackBufferWinH);
		WinPushDrawState();
		
		count = 0;

		if(gOpQueue[0].OpCode == STLINE_OP)
		{
			WinSetForeColorRGB(&gOpQueue[0].color, NULL);
			
			WinDrawLine(gOpQueue[0].Op.stLineOp.startX ,
						gOpQueue[0].Op.stLineOp.startY ,
						gOpQueue[0].Op.stLineOp.endX ,
						gOpQueue[0].Op.stLineOp.endY);
			
			WinSetForeColorRGB(&gCurrentColor, NULL);
		}
	
		if(gOpQueue[0].OpCode == TEXT_OP)
		{
			WinSetTextColorRGB(&gOpQueue[0].color, NULL);
			
			WinSetDrawMode(winOverlay);
			WinPaintChars(gOpQueue[0].Op.textOp.text, StrLen(gOpQueue[0].Op.textOp.text), 
					 	  gOpQueue[0].Op.textOp.x ,
						  gOpQueue[0].Op.textOp.y );
			
			WinSetTextColorRGB(&gCurrentColor, NULL);
		}
		
		if(gOpQueue[0].OpCode == FREELINE_OP)
		{
			UInt16 count = 0;
			
			WinSetForeColorRGB(&gOpQueue[0].color, NULL);
			
			for(count = 0; count < gOpQueue[0].Op.freeLineOp.numPoints; count+=2)
				WinDrawLine(gOpQueue[0].Op.freeLineOp.x[count], 
							gOpQueue[0].Op.freeLineOp.y[count], 
							gOpQueue[0].Op.freeLineOp.x[count+1], 
							gOpQueue[0].Op.freeLineOp.y[count+1]);

			if(gOpQueue[0].Op.freeLineOp.x)
			{
				MemPtrFree(gOpQueue[0].Op.freeLineOp.x);
				gOpQueue[0].Op.freeLineOp.x = NULL;
			}
			
			if(gOpQueue[0].Op.freeLineOp.y)
			{
				MemPtrFree(gOpQueue[0].Op.freeLineOp.y);
				gOpQueue[0].Op.freeLineOp.y = NULL;
			}		
			
			WinSetForeColorRGB(&gCurrentColor, NULL);
		}

		WinSetDrawWindow(mainWindow);
		WinPopDrawState();

		// Shift all elements in the queue by 1
		for(count = 1; count <= gOpLastIndex; count++)
			gOpQueue[count - 1] = gOpQueue[count];
	}
	

	// If the queue is still not full, go and increment the index by one
	if(gOpLastIndex < MAX_OPS - 1)
	{
		gOpLastIndex++;
		gOpIndex = gOpLastIndex;
	
		// If there is already an operation performed, free it!
		if(gOpQueue[gOpLastIndex].OpCode == FREELINE_OP)
		{
			if(gOpQueue[gOpLastIndex].Op.freeLineOp.x)
			{
				MemPtrFree(gOpQueue[gOpLastIndex].Op.freeLineOp.x);
				gOpQueue[gOpLastIndex].Op.freeLineOp.x = NULL;
			}
		
			if(gOpQueue[gOpLastIndex].Op.freeLineOp.y)
			{
				MemPtrFree(gOpQueue[gOpLastIndex].Op.freeLineOp.y);
				gOpQueue[gOpLastIndex].Op.freeLineOp.y = NULL;
			}
		}
	}
	gOpQueue[gOpLastIndex].OpCode = opCode;
	
	// Add new element to the queue
	if(opCode == STLINE_OP)
	{
		gOpQueue[gOpLastIndex].Op.stLineOp.startX = startX - gScreenDrawAreaRect.topLeft.x + gDrawAreaRect.topLeft.x ;
		gOpQueue[gOpLastIndex].Op.stLineOp.endX = endX  - gScreenDrawAreaRect.topLeft.x + gDrawAreaRect.topLeft.x;
		gOpQueue[gOpLastIndex].Op.stLineOp.startY = startY  - gScreenDrawAreaRect.topLeft.y + gDrawAreaRect.topLeft.y;
		gOpQueue[gOpLastIndex].Op.stLineOp.endY = endY  - gScreenDrawAreaRect.topLeft.y + gDrawAreaRect.topLeft.y;
	}
	
	if(opCode == TEXT_OP)
	{
		gOpQueue[gOpLastIndex].Op.textOp.x = startX - gScreenDrawAreaRect.topLeft.x + gDrawAreaRect.topLeft.x ;
		gOpQueue[gOpLastIndex].Op.textOp.y = startY - gScreenDrawAreaRect.topLeft.y + gDrawAreaRect.topLeft.y;

		if(text)
			StrCopy(gOpQueue[gOpLastIndex].Op.textOp.text, text);
	}
	
	if(opCode == FREELINE_OP)
	{
		
		gOpQueue[gOpLastIndex].Op.freeLineOp.x = MemPtrNew(numPoints * sizeof(Coord));
		gOpQueue[gOpLastIndex].Op.freeLineOp.y = MemPtrNew(numPoints * sizeof(Coord));
		
		MemMove(gOpQueue[gOpLastIndex].Op.freeLineOp.x, xCoord, numPoints * sizeof(Coord));
		MemMove(gOpQueue[gOpLastIndex].Op.freeLineOp.y, yCoord, numPoints * sizeof(Coord));
		
		gOpQueue[gOpLastIndex].Op.freeLineOp.numPoints = numPoints;
	}
	
	// Set color of the operation
	gOpQueue[gOpLastIndex].color.r = gCurrentColor.r;
	gOpQueue[gOpLastIndex].color.g = gCurrentColor.g;
	gOpQueue[gOpLastIndex].color.b = gCurrentColor.b;
}	


#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    	PrvIfSmallPic
 *
 * DESCRIPTION: 	Checks to see if an image is smaller than the drawing area
 *
 *
 * PARAMETERS:  	None	
 *
 * RETURNED:    	True, if the image is smaller than the drawing area,
 *					False, if not
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PrvIfSmallPic()
{
	Boolean	isSmall = false;
	
	if(gPhotoWidth <= DRAW_AREA_WIDTH && gPhotoHeight <= DRAW_AREA_HEIGHT)
		isSmall = true;
	
	return isSmall;
}

/***********************************************************************
 *
 * FUNCTION:    		PrvDoSingleSelection
 *
 * DESCRIPTION: 		Performs a single selection from the list of 
 *						photos on the device
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Err	PrvDoSingleSelection()
{
	
	Err err = errNone;

	// gSelectionParam is a global variable of type PalmPhotoSelectionParam
	
	MemSet(&gSelectionParam,sizeof(PalmPhotoSelectionParam),0);	
						
	gSelectionParam.albumID = PALM_PHOTO_ALBUM_ALL;	
	gSelectionParam.offset = 0;			
	gSelectionParam.selectionCount = PALM_PHOTO_SELECT_ALL;	
	gSelectionParam.filterCallback = NULL;	
	gSelectionParam.userDataP = NULL;		
	gSelectionParam.selectedImages.imageCount= 0;
				
	err = PalmPhotoSelectDlg(gLibRefNum, &gSelectionParam, palmPhotoDlgSingleSelection, true);

	return err;
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    		PrvChangeColor
 *
 * DESCRIPTION: 		Changes the current color if the user selects
 *						a color from the color-palette
 *
 * PARAMETERS:  		x,y-co-ordinates of the pixel whose color will be set
 *						to the current color
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvChangeColor(Coord x, Coord y)
{
	RectangleType	whiteSq = {{COLOR_PALETTE_TOPLEFT_X, COLOR_PALETTE_TOPLEFT_Y + SQ_HEIGHT + GAP_HEIGHT}, {SQ_WIDTH, SQ_HEIGHT}};
	
	WinGetPixelRGB(x,y, &gCurrentColor);
	
	// Hack! -> So that the white gaps between each colored square
	// in the palette are not chosen
	if(gCurrentColor.r == 0xff && gCurrentColor.g == 0xff && gCurrentColor.b == 0xff) 
		if(!RctPtInRectangle(x, y, &whiteSq))
			return;
	
	WinSetForeColorRGB(&gCurrentColor, NULL);
	WinSetTextColorRGB(&gCurrentColor, NULL);
	WinDrawRectangle(&gCurrentColorRect, 0);
}


/***********************************************************************
 *
 * FUNCTION:    		PrvInitBackBuffer
 *
 * DESCRIPTION: 		Initializes the two back-buffers used by this 
 *						tool -> gBackBufferWinH -> This back-buffer contains the 
 *								whole jpeg picture and all the operations will be 
 *								committed on this back-buffer once the user decides
 *								to save the picture
 *								
 *								gViewPortBackBufferWinH -> This back-buffer contains the
 *								current view of a certain portion of gBackBufferWinH. The
 *								size of this back-buffer will be the same as gDrawAreaRect
 *								and will be drawn at location of gScreenDrawAreaRect
 *
 * PARAMETERS:  		selectedImageP -> Pointer to the Photo handle which needs to be
 *										  transferred to gBackBufferWinH
 *						width -> Width of the photo
 *						height -> Height of the photo
 *
 * RETURNED:    		Err -> Returns an error if there has been an error while creating
 *								the back buffers or while reading from the photo
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Err PrvInitBackBuffer(PalmPhotoHandle *selectedImageP, UInt16 width, UInt16 height)
{
	PalmPhotoDisplayParam	displayParam;
	RectangleType			displayRect;
	UInt32					density = 0;	
	Err						err = errNone;
	
	// Get the screen density. If it is double-density, back-buffer is created as a nativeFormat window
	// else it is created as a genericFormat window.
	WinScreenGetAttribute(winScreenDensity, &density);
	
	if(gBackBufferWinH)
	{
		WinDeleteWindow(gBackBufferWinH, false);
		gBackBufferWinH = NULL;
	}
	if(gViewPortBackBufferWinH)
	{
		WinDeleteWindow(gViewPortBackBufferWinH, false);
		gViewPortBackBufferWinH = NULL;
	}
	
	gPhotoWidth = (width + 1) >> 1;
	gPhotoHeight = (height + 1) >> 1;
	
	if(PrvIfSmallPic())
	{
		gScreenDrawAreaRect.topLeft.x = DRAW_AREA_TOPLEFT_X + ((DRAW_AREA_WIDTH - gPhotoWidth) / 2);
		gScreenDrawAreaRect.topLeft.y = DRAW_AREA_TOPLEFT_Y + ((DRAW_AREA_HEIGHT - gPhotoHeight) / 2);
		gScreenDrawAreaRect.extent.x = gPhotoWidth;
		gScreenDrawAreaRect.extent.y = gPhotoHeight;
	
	}
		
	// Initializes a back-buffer which contains the entire picture, and a viewport-buffer which
	// contains the current 'view' of the picture	
	if(density == kDensityDouble)
		gBackBufferWinH = WinCreateOffscreenWindow(gPhotoWidth , gPhotoHeight , nativeFormat, &err);
	else
		gBackBufferWinH = WinCreateOffscreenWindow(gPhotoWidth , gPhotoHeight , genericFormat, &err);

	if(err)
	{
		FrmAlert(DrawErrorAlert);
		return err;
	}

	if(density == kDensityDouble)
		gViewPortBackBufferWinH = WinCreateOffscreenWindow(DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT, nativeFormat, &err);
	else
		gViewPortBackBufferWinH = WinCreateOffscreenWindow(DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT, genericFormat, &err);
		
	if(err)
	{
		FrmAlert(DrawErrorAlert);		
		return err;
	}
	
	if(!err)
	{
		if(selectedImageP)
		{
			displayRect.topLeft.x = 0;
			displayRect.topLeft.y = 0;

			if(density == kDensityDouble)
			{
				displayRect.extent.x = width;
				displayRect.extent.y = height;
			}
			else
			{
				displayRect.extent.x = gPhotoWidth;
				displayRect.extent.y = gPhotoHeight;
			}
			
			displayParam.winH = gBackBufferWinH;
			displayParam.rect = displayRect;
			displayParam.displayCallback = NULL;
			displayParam.userDataP = NULL;
			
			err = PalmPhotoDisplayImage(gLibRefNum, *selectedImageP, &displayParam);
		}
	}

	return err;
}

/***********************************************************************
 *
 * FUNCTION:    	PrvDisplayImageOnScreen
 *	
 * DESCRIPTION: 	Displays the portion of a picture on the screen. It first copies 
 *					gBackBufferWinH to gViewPortBackBufferWinH. All draw operations
 *					are then performed on gViewPortBackBufferWinH. Finally the 
 *					gViewPortBackBufferWinH is draw onto the screen
 *
 * PARAMETERS:  	None
 *
 * RETURNED:    	Nothing	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void	PrvDisplayImageOnScreen()
{
	RectangleType locDrawAreaRect = { {0,0}, {DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT}};
	RectangleType tempRect;
	
	WinGetClip(&tempRect);
	WinSetClip(&gScreenDrawAreaRect);
	
	if(gBackBufferWinH == NULL || gViewPortBackBufferWinH == NULL)
		goto Done;
		
	if(!PrvIfSmallPic())
		WinCopyRectangle(gBackBufferWinH,
						 gViewPortBackBufferWinH,
						 &gDrawAreaRect, 
						 0,
						 0,
						 winPaint);
	else
		WinCopyRectangle(gBackBufferWinH,
						 gViewPortBackBufferWinH,
						 &gDrawAreaRect, 
						 gScreenDrawAreaRect.topLeft.x - DRAW_AREA_TOPLEFT_X,
						 gScreenDrawAreaRect.topLeft.y - DRAW_AREA_TOPLEFT_Y,
						 winPaint);


	PrvDrawOps();
					 
	WinCopyRectangle(gViewPortBackBufferWinH,
					 NULL,
					 &locDrawAreaRect,	
					 DRAW_AREA_TOPLEFT_X , DRAW_AREA_TOPLEFT_Y , 
					 winPaint);

	WinSetClip(&tempRect);
	
Done:
	return;
}

/***********************************************************************
 *
 * FUNCTION:    		PrvDrawBorder
 *
 * DESCRIPTION: 		Draws the border for the drawing area canvas
 *
 * PARAMETERS:  		Nothing
 *
 * RETURNED:    		None
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvDrawBorder()
{
	RectangleType	screenDrawAreaBorderRect = { {DRAW_AREA_TOPLEFT_X , DRAW_AREA_TOPLEFT_Y }, 
											{DRAW_AREA_WIDTH, DRAW_AREA_HEIGHT}};
	RGBColorType blackCol = {0,0,0,0};

	WinSetForeColorRGB(&blackCol, NULL);
	WinDrawRectangleFrame(rectangleFrame, &screenDrawAreaBorderRect);
	WinSetForeColorRGB(&gCurrentColor, NULL);
}

/***********************************************************************
 *
 * FUNCTION:    		PrvClipImage
 *
 * DESCRIPTION: 		Clips a portion of the picture when the user pans
 *						the picture. It receives the difference in x and y
 *						co-ordinates (which is indicative of the pen move done
 *						by the user) and sets the start x and y co-ordinates of
 *						gDrawAreaRect and then that portion of the rectangle is clipped
 *						from gBackBufferWinH
 *
 * PARAMETERS:  		diffX, diffY -> Difference in x,y co-ordinates of the 
 *						pen movement by the user
 *
 * RETURNED:    		None
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void	PrvClipImage(Int16 diffX, Int16 diffY)
{
	// When you pull to the left
	if((gPhotoWidth > DRAW_AREA_WIDTH) || (gPhotoHeight > DRAW_AREA_HEIGHT))
	{
		gDrawAreaRect.topLeft.x -= diffX;
		gDrawAreaRect.topLeft.y -= diffY;

		if(gDrawAreaRect.topLeft.x >= ( (Int16)gPhotoWidth -  DRAW_AREA_WIDTH))		
			gDrawAreaRect.topLeft.x = ( (Int16)gPhotoWidth - DRAW_AREA_WIDTH);
			// When you pull to the right
		else if(gDrawAreaRect.topLeft.x <= 0)	
			gDrawAreaRect.topLeft.x = 0;
		
		
		// When you pull to the top
		if(gDrawAreaRect.topLeft.y > ( (Int16)gPhotoHeight - DRAW_AREA_HEIGHT))
			gDrawAreaRect.topLeft.y = ( (Int16)gPhotoHeight - DRAW_AREA_HEIGHT);
		// When you pull to the bottom
		else if(gDrawAreaRect.topLeft.y < 0)	
			gDrawAreaRect.topLeft.y = 0;
	}
}



/***********************************************************************
 *
 * FUNCTION:    		PrvOpenPhoto
 *
 * DESCRIPTION: 		Opens a photo for the user. Sets gSelectedPhotoH
 *						to the selected photo
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		Err-> depending on the error returned by InitBackBuffer
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Err PrvOpenPhoto()
{
	PalmPhotoImageInfo	info;
	Err err = errNone;

	err = PrvDoSingleSelection();
	
	WinDrawRectangle(&gCurrentColorRect, 0);
			
	// Get the selected image
	if(!err)
		gSelectedPhotoH = gSelectionParam.selectedImages.imageH[0];					
	else
		goto Done;				

	if(gSelectedPhotoH)
	{
		err = PalmPhotoGetImageInfo(gLibRefNum, gSelectedPhotoH,  &info);
		if(!err)
		{
			gOpIndex = -1;
			gOpLastIndex = -1;
			
			if(gXCoordP)
			{
				MemPtrFree(gXCoordP);
				gXCoordP = NULL;
			}
			if(gYCoordP)
			{
				MemPtrFree(gYCoordP);
				gYCoordP = NULL;
			}
			
			gXCoordP = MemPtrNew(MAX_POINTS * sizeof(Coord));
			gYCoordP = MemPtrNew(MAX_POINTS * sizeof(Coord));
			
			err = PrvInitBackBuffer(&gSelectedPhotoH, info.width, info.height);
			
			if(err)
			{
				if(err == sysErrNoFreeResource)
					FrmAlert(OutOfMemoryErrorAlert);
				goto Done;
			}
		}
		else
			goto Done;
	}

Done:
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    		PrvInitOpenPhoto
 *
 * DESCRIPTION: 		Performs all the initialization after a photo is opened
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvInitOpenPhoto()
{
	Err err = errNone;
	FormType  *frmMainP = NULL;
	ControlType *ctlP = NULL;
	
	err = PrvOpenPhoto();
			
	if(!err)
	{
		gDrawAreaRect.topLeft.x = 0;
		gDrawAreaRect.topLeft.y = 0;
		gOldX = 0;
		gOldY = 0;
		gNewX = 0;
		gNewY = 0;
		gDiffX = 0;
		gDiffY = 0;
				
		DeInitOpQueue();

		gOpLastIndex = -1;
		gOpIndex = -1;

		frmMainP = FrmGetActiveForm();
		FrmDrawForm(frmMainP);
							
		ctlP = FrmGetObjectPtr(frmMainP, FrmGetObjectIndex(frmMainP, MainStraightLinePushButton));
		CtlSetValue(ctlP, false);
		ctlP = FrmGetObjectPtr(frmMainP, FrmGetObjectIndex(frmMainP, MainTimestampPushButton));
		CtlSetValue(ctlP, false);
		ctlP = FrmGetObjectPtr(frmMainP, FrmGetObjectIndex(frmMainP, MainAddTextPushButton));
		CtlSetValue(ctlP, false);
		ctlP = FrmGetObjectPtr(frmMainP, FrmGetObjectIndex(frmMainP, MainFreeLinePushButton));
		CtlSetValue(ctlP, false);

		ctlP = FrmGetObjectPtr(frmMainP, FrmGetObjectIndex(frmMainP, MainPanningPushButton));
		CtlSetValue(ctlP, true);
						
		WinDrawRectangle(&gCurrentColorRect, 0);
		gSelection = PAN_SELECTION;
	}

	WinSetForeColorRGB(&gCurrentColor, NULL);
	WinSetTextColorRGB(&gCurrentColor, NULL);
			
	WinDrawRectangle(&gCurrentColorRect, 0);

}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    		PrvInitMainForm
 *
 * DESCRIPTION: 		Performs all the initialization when a frmOpenEvent
 *						is generated for MainForm
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		err -> Error returned by InitBackBuffer
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Err PrvInitMainForm()
{
	FormType  *frmP = NULL;
	ControlType *ctlP = NULL;
	Err err = errNone;
	UInt16 count = 0;

	// Initialization
	frmP = FrmGetActiveForm();
	FrmDrawForm(frmP);
			
	WinSetForeColorRGB(&gCurrentColor, NULL);
	WinDrawRectangle(&gCurrentColorRect, 0);
			
	gSelection = CURVY_LINE_SELECTION;
		
	gOpIndex = -1;
	gOpLastIndex = -1;
			
	if(gXCoordP)
	{
		MemPtrFree(gXCoordP);
		gXCoordP = NULL;
	}
	if(gYCoordP)
	{
		MemPtrFree(gYCoordP);
		gYCoordP = NULL;
	}
				
	gXCoordP = MemPtrNew(MAX_POINTS * sizeof(Coord));
	gYCoordP = MemPtrNew(MAX_POINTS * sizeof(Coord));
				
	err = PrvInitBackBuffer(NULL, 320, 320);
			
	PrvDisplayImageOnScreen();
	PrvDrawBorder();
			
	ctlP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, MainFreeLinePushButton));
	CtlSetValue(ctlP, true);

	for(count = 0; count < MAX_OPS; count++)
	{
		gOpQueue[count].Op.freeLineOp.x = NULL;
		gOpQueue[count].Op.freeLineOp.y = NULL;
	}
			
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    		PrvDeInitMainForm
 *
 * DESCRIPTION: 		Performs all the cleanup when a frmCloseEvent is 
 *						generated for MainForm
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvDeInitMainForm()
{
    FormType *mainFormP = FrmGetActiveForm();
    
	if(gSelectionParam.selectedImages.imageH)
	{
		PalmPhotoFreeSelections(gLibRefNum, &gSelectionParam);
		PalmPhotoCloseImage(gLibRefNum, gSelectionParam.selectedImages.imageH[0]);
		MemPtrFree(gSelectionParam.selectedImages.imageH);
	}
	
	// De-allocates all memory allocated on the dynamic heap, when MainForm is closed		
	DeInitOpQueue();
			
	if(gXCoordP)
	{	
		MemPtrFree(gXCoordP);
		gXCoordP = NULL;
	}
	if(gYCoordP)
	{
		MemPtrFree(gYCoordP);
		gYCoordP = NULL;
	}
	if(gBackBufferWinH)
	{	
		WinDeleteWindow(gBackBufferWinH, false);
		gBackBufferWinH = NULL;
	}
	if(gViewPortBackBufferWinH)
	{
		WinDeleteWindow(gViewPortBackBufferWinH, false);
		gViewPortBackBufferWinH = NULL;
	}
	if(gTextToAdd)
	{
		MemPtrFree(gTextToAdd);
		gTextToAdd = NULL;
	}
			
	FrmEraseForm(mainFormP);
	FrmDeleteForm(mainFormP);
}



#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    		PrvGotoOrganiseDialog
 *
 * DESCRIPTION: 		Performs all call to the Organise Dialog, performs
 *						all the necessary initialization and clean-up before \
 *						and after the dialog-call
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvGotoOrganiseDialog()
{
	FormType *frmOrganiseP = NULL;
	Err err = errNone;
	FormType *frmP = FrmGetActiveForm();
	
	frmOrganiseP = FrmInitForm(DetailsForm);
	FrmSetEventHandler(frmOrganiseP, DetailsFormHandleEvent);
			
	gLstP = (ListType *)FrmGetObjectPtr(frmOrganiseP, 
										FrmGetObjectIndex(frmOrganiseP, DetailsAlbumList));

	err = InitAlbumList();

	// Initialising of global variables
	gPhotoIndex = 0;
	gNumImagesInAlbum = 0;
		
	InitAlbumListUI(gLstP);
	LstSetSelection(gLstP, noListSelection);
	
	UpdateArrows();	
	ToggleLabelsDisplay(frmOrganiseP, false);
	FrmDoDialog(frmOrganiseP);

	PalmPhotoFreeSelections(gLibRefNum, &gDetailsSelectionParam);
        DeInitAlbumList(gAlbumList, gAlbumCount);
	FrmEraseForm(frmOrganiseP);
	FrmDeleteForm(frmOrganiseP);
	
	// Re-displaying what is on the screen
	// This is done, because some portions of the form will be lost
	FrmDrawForm(frmP);
	PrvDrawBorder();
	PrvDisplayImageOnScreen();
	WinDrawRectangle(&gCurrentColorRect, 0);
	
}

/***********************************************************************
 *
 * FUNCTION:    		PrvGotoSaveDialog
 *
 * DESCRIPTION: 		Performs all call to the Save Dialog, performs
 *						all the necessary initialization and clean-up before \
 *						and after the dialog-call
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvGotoSaveDialog()
{
	FormType *frmSaveP = NULL;
	Err err = errNone;
	
	frmSaveP = FrmInitForm(SavePhotoForm);
	gLstP = (ListType *)FrmGetObjectPtr(frmSaveP, FrmGetObjectIndex(frmSaveP, SavePhotoAlbumList));
			
	err = InitAlbumList();
	if(!err)
		InitAlbumListUI(gLstP);
	LstSetSelection(gLstP, 0);
			
	CtlSetLabel( (ControlType *)FrmGetObjectPtr(frmSaveP, FrmGetObjectIndex(frmSaveP, SavePhotoAlbumListPopTrigger)),
										LstGetSelectionText(gLstP,LstGetSelection(gLstP)) );
	FrmSetFocus(frmSaveP, FrmGetObjectIndex(frmSaveP, SavePhotoFileNameField));
			
	FrmSetEventHandler(frmSaveP, SavePhotoFormHandleEvent);
	FrmDoDialog(frmSaveP);
	FrmEraseForm(frmSaveP);
	FrmDeleteForm(frmSaveP);
	DeInitAlbumList();

}

/***********************************************************************
 *
 * FUNCTION:    		PrvGotoAlbumsOrganiseDialog
 *
 * DESCRIPTION: 		Performs all call to the Albums Organise Dialog, performs
 *						all the necessary initialization and clean-up before \
 *						and after the dialog-call
 *
 * PARAMETERS:  		None
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvGotoAlbumsOrganiseDialog()
{
	FormType *frmAlbumsP = NULL;
	ListType *lstP = NULL;
	Err err = errNone;
	
	frmAlbumsP = FrmInitForm(AlbumOrganiseForm);
	lstP = FrmGetObjectPtr(frmAlbumsP, FrmGetObjectIndex(frmAlbumsP, AlbumOrganiseAlbumList));
			
	err = InitAlbumList();
			
	if(!err)
	{
		LstSetListChoices(lstP, gAlbumList, gAlbumCount);
		LstSetSelection(lstP, 0);
	}
	
	FrmSetEventHandler(frmAlbumsP, AlbumsOrganiseFormHandleEvent);
		
	FrmDoDialog(frmAlbumsP);
	FrmEraseForm(frmAlbumsP);
	FrmDeleteForm(frmAlbumsP);
	DeInitAlbumList();
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    		PrvHandlePanSelection
 *
 * DESCRIPTION: 		Handles penDownEvent when the Panning option is selected
 *
 * PARAMETERS:  		EventType *eventP -> Pointer to the event structure 
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvHandlePanSelection(EventType *eventP)
{
	Boolean isPenDown = true;
	
	gNewX = eventP->screenX;
	gNewY = eventP->screenY;
						
	while(isPenDown)
	{
		gOldX = gNewX;
		gOldY = gNewY;
							
	
		EvtGetPen(&gNewX, &gNewY, &isPenDown);
							
		if(gNewX == gOldX && gNewY == gOldY)
			continue;

		gDiffX = (gNewX - gOldX);
		gDiffY = (gNewY - gOldY);
				
		PrvClipImage(gDiffX, gDiffY);
		PrvDisplayImageOnScreen();
	}
}

/***********************************************************************
 *
 * FUNCTION:    		PrvHandleCurvyLineSelection
 *
 * DESCRIPTION: 		Handles penDownEvent when the Curvy-Line option 
 *						is selected
 *
 * PARAMETERS:  		EventType * eventP -> Pointer to the event structure 
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvHandleCurvyLineSelection(EventType *eventP)
{
	Boolean isPenDown = true;
	UInt16	numPoints = 0;

	gNewX = eventP->screenX;
	gNewY = eventP->screenY;

						
	WinPaintPixel(gNewX, gNewY);

	numPoints = 0;
	
	// Add elements to the queue. 					
	gXCoordP[numPoints] = gNewX - gScreenDrawAreaRect.topLeft.x + gDrawAreaRect.topLeft.x;
	gYCoordP[numPoints] = gNewY - gScreenDrawAreaRect.topLeft.y + gDrawAreaRect.topLeft.y;
					
	gXCoordP[numPoints+1] = gNewX - gScreenDrawAreaRect.topLeft.x + gDrawAreaRect.topLeft.x;
	gYCoordP[numPoints+1] = gNewY - gScreenDrawAreaRect.topLeft.y + gDrawAreaRect.topLeft.y;

	numPoints+=2;
									
	while(isPenDown)
	{
		gOldX = gNewX;
		gOldY = gNewY;
							
		EvtGetPen(&gNewX, &gNewY, &isPenDown);
					
		if(gNewX == gOldX && gNewY == gOldY)
			continue;
						
		if(numPoints < MAX_POINTS - 1)
		{
			gXCoordP[numPoints] = gNewX - gScreenDrawAreaRect.topLeft.x + gDrawAreaRect.topLeft.x;
			gYCoordP[numPoints] = gNewY - gScreenDrawAreaRect.topLeft.y + gDrawAreaRect.topLeft.y;
					
			gXCoordP[numPoints+1] = gOldX - gScreenDrawAreaRect.topLeft.x + gDrawAreaRect.topLeft.x;
			gYCoordP[numPoints+1] = gOldY - gScreenDrawAreaRect.topLeft.y + gDrawAreaRect.topLeft.y;
										
			numPoints += 2;
		}
		else
		{
			PrvAddOpToQueue(0, 0, 0, 0, NULL, gXCoordP, gYCoordP, numPoints, FREELINE_OP);

			MemPtrFree(gXCoordP);
			gXCoordP = NULL;
			MemPtrFree(gYCoordP);
			gYCoordP = NULL;
								
			gXCoordP = MemPtrNew(MAX_POINTS * sizeof(Coord));
			gYCoordP = MemPtrNew(MAX_POINTS * sizeof(Coord));
								
			numPoints = 0;
		}
							
		WinDrawLine(gNewX, gNewY, gOldX, gOldY);
	}
						
	PrvAddOpToQueue(0, 0, 0, 0, NULL, gXCoordP, gYCoordP, numPoints, FREELINE_OP);
}

/***********************************************************************
 *
 * FUNCTION:    		PrvHandleStraightLineSelection
 *
 * DESCRIPTION: 		Handles penDownEvent when the Straight-Line option 
 *						is selected
 *
 * PARAMETERS:  		EventType * eventP -> Pointer to the event structure 
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvHandleStraightLineSelection(EventType * eventP)
{
	Coord startX = eventP->screenX;
	Coord startY = eventP->screenY;
	Boolean isPenDown = true;
	
	gNewX = eventP->screenX;
	gNewY = eventP->screenY;
						
	WinSetForeColorRGB(&gCurrentColor, NULL);

	while(isPenDown)
	{
		gOldX = gNewX;
		gOldY = gNewY;
							
		EvtGetPen(&gNewX, &gNewY, &isPenDown);

		if(gNewX == gOldX && gNewY == gOldY)
			continue;

		PrvDisplayImageOnScreen();
		WinDrawLine(startX , startY, gNewX, gNewY);
	}

	PrvAddOpToQueue(startX, startY, gNewX, gNewY, NULL, NULL, NULL, 0, STLINE_OP);

}

/***********************************************************************
 *
 * FUNCTION:    		PrvHandleTextSelection
 *
 * DESCRIPTION: 		Handles penDownEvent when the Text option is selected
 *
 * PARAMETERS:  		EventType * eventP -> Pointer to the event structure 
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvHandleTextSelection(EventType *eventP)
{
	Int16	widthOffset = 0;
	Int16 	heightOffset = 0;
	Boolean isPenDown = true;
	
	gNewX = eventP->screenX;
	gNewY = eventP->screenY;
						
	WinSetTextColorRGB(&gCurrentColor, NULL);
						
	if(gTextToAdd)
	{
		widthOffset = FntCharsWidth(gTextToAdd, StrLen(gTextToAdd)) / 2;
		heightOffset = FntCharHeight() / 2;
							
		WinPushDrawState();
		WinSetDrawMode(winOverlay);
							
		gNewX -= widthOffset;
		gNewY -= heightOffset;
							
		WinPaintChars(gTextToAdd, StrLen(gTextToAdd), gNewX, gNewY);
						
		while(isPenDown)
		{
			gOldX = gNewX;
			gOldY = gNewY;
								
			EvtGetPen(&gNewX, &gNewY, &isPenDown);
								
			if(gNewX == gOldX && gNewY == gOldY)
				continue;
			PrvDisplayImageOnScreen();
			gNewX -= widthOffset;
			gNewY -= heightOffset;
			WinPaintChars(gTextToAdd, StrLen(gTextToAdd), gNewX, gNewY);
		}
							
		WinPopDrawState();
		PrvAddOpToQueue(gNewX, gNewY, 0, 0, gTextToAdd, NULL, NULL, 0, TEXT_OP);
						
	}

}

/***********************************************************************
 *
 * FUNCTION:    		PrvHandleTimeSelection
 *
 * DESCRIPTION: 		Handles penDownEvent when the TimeStamp option 
 *						is selected
 *
 * PARAMETERS:  		EventType * eventP -> Pointer to the event structure 
 *
 * RETURNED:    		Nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static void PrvHandleTimeSelection(EventType *eventP)
{
	DateType	*date = NULL;
	Char 		*dateString = NULL;
	Int16		widthOffset = 0;
	Int16		heightOffset = 0;
	Boolean 	isPenDown = true;
	UInt16		dateSize = 20;
	
	date = (DateType *)GetSelectedPhotoExtraInformation(gLibRefNum,palmPhotoExtraInfoDate, gSelectedPhotoH);

	gNewX = eventP->screenX;
	gNewY = eventP->screenY;
						
	dateString = MemPtrNew(dateSize);

	// If the drawing is being done on a blank canvas, the system date is drawn on the 
	// picture
	if(date && date->month != 0 && date->day != 0 && date->year != 0)
		DateToAscii (date->month, date->day, date->year + firstYear, 
					(DateFormatType)PrefGetPreference(prefDateFormat), dateString);
	else
	{
		DateTimeType	sysDate;
		TimSecondsToDateTime(TimGetSeconds(), &sysDate);
		DateToAscii (sysDate.month, sysDate.day, sysDate.year, 
					(DateFormatType)PrefGetPreference(prefDateFormat), dateString);
	}	
						
	// Finds the width and height of the text, so as to center the text when it is being
	// drawn on to the screen
	widthOffset = FntCharsWidth(dateString, StrLen(dateString)) / 2;
	heightOffset = FntCharHeight() / 2;

	WinSetTextColorRGB(&gCurrentColor, NULL);
	isPenDown = true;
							
	WinPushDrawState();
	WinSetDrawMode(winOverlay);
							
	gNewX -= widthOffset;
	gNewY -= heightOffset;
							
	WinPaintChars(dateString, StrLen(dateString), gNewX , gNewY);

	while(isPenDown)
	{
		gOldX = gNewX;
		gOldY = gNewY;
					
		EvtGetPen(&gNewX, &gNewY, &isPenDown);
						
		gNewX -= widthOffset;
		gNewY -= heightOffset;
								
		if(gNewX == gOldX && gNewY == gOldY)
			continue;
		PrvDisplayImageOnScreen();
		if(dateString)
			WinPaintChars(dateString, StrLen(dateString), gNewX , gNewY);
	}

	if(dateString)
		PrvAddOpToQueue(gNewX, gNewY, 0, 0, dateString, NULL, NULL, 0, TEXT_OP);
							
	WinPopDrawState();	
								
	// De-Allocating memory
	if(date)
	{
		MemPtrFree(date);
		date = NULL;
	}	
							
	MemPtrFree(dateString);
	dateString = NULL;
}

#if 0
#pragma mark -
#endif

/***********************************************************************
 *
 * FUNCTION:    		PrvHandleMenu
 *
 * DESCRIPTION: 		Deals with all menu commands issued
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
	Err err = errNone;
	FormType *frmAboutP = NULL;
	
	switch(itemID)
	{
		case FileNew:
			
			DeInitOpQueue();

			gOpIndex = -1;
			gOpLastIndex = -1;
				
			if(gXCoordP)
			{
				MemPtrFree(gXCoordP);
				gXCoordP = NULL;
			}
			if(gYCoordP)
			{
				MemPtrFree(gYCoordP);
				gYCoordP = NULL;
			}
				
			gXCoordP = MemPtrNew(MAX_POINTS * sizeof(Coord));
			gYCoordP = MemPtrNew(MAX_POINTS * sizeof(Coord));
				
			err = PrvInitBackBuffer(NULL, 320, 320);
			
			if(!err)
				PrvDisplayImageOnScreen();
			PrvDrawBorder();
			
			break;
			
		case FileOpen:
			
			PrvInitOpenPhoto();
			PrvClipImage(gDiffX, gDiffY);
			PrvDisplayImageOnScreen();
			PrvDrawBorder();
			break;
			
		case FilePhotoDetails:
			
			PrvGotoOrganiseDialog();
			break;
		
		case FileSaveAs:
			
			PrvGotoSaveDialog();
			break;	
		
		case FileAlbums:
			
			PrvGotoAlbumsOrganiseDialog();
			break;
			
		case HelpAboutFotoFinish:
			frmAboutP = FrmInitForm(AboutForm);
			FrmDoDialog(frmAboutP);					// Display the About Box.
			FrmEraseForm(frmAboutP);
			FrmDeleteForm(frmAboutP);
			
			WinSetForeColorRGB(&gCurrentColor, NULL);
			WinSetTextColorRGB(&gCurrentColor, NULL);
			
			WinDrawRectangle(&gCurrentColorRect, 0);
			
			break;
			
		case EditUndo:
			if(gOpIndex >= 0)
			{
				gOpIndex--;
				PrvDisplayImageOnScreen();
			}
			break;

		case EditRedo:
			if(gOpIndex < gOpLastIndex)
			{
				gOpIndex++;
				PrvDisplayImageOnScreen();
			}
			break;
	}
}

/***********************************************************************
 *
 * FUNCTION:    		PrvHandleButton
 *
 * DESCRIPTION: 		Deals with all ctlSelectEvent events generateds
 *
 * PARAMETERS:  		ctlID -> ID of the button which has been pressed
 *
 * RETURNED:    		Boolean handled -> Whether the event has been handled 
 *											or not
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PrvHandleButton(Int16 ctlID)
{
	Boolean handled = false;
	FormType 	*frmP = NULL;
	FieldType 	*fldTextToAdd = NULL;
	
	switch(ctlID)
	{
		case MainOpenGraphicButton:
		
			PrvInitOpenPhoto();
			PrvClipImage(gDiffX,gDiffY);
			PrvDisplayImageOnScreen();
			PrvDrawBorder();
					
			handled = true;
			break;
				
		case MainPanningPushButton:			
					
			gSelection = PAN_SELECTION;
			handled = true;
			break;
			
		case MainStraightLinePushButton:
					
			gSelection = STRAIGHT_LINE_SELECTION;
			handled = true;
			break;
			
		case MainFreeLinePushButton:
					
			gSelection = CURVY_LINE_SELECTION;				
			handled = true;
			break;
			
		case MainAddTextPushButton:

					
			frmP = FrmInitForm(AddTextForm);
					
			fldTextToAdd = (FieldType *)FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, AddTextTextField));
			FrmSetFocus(frmP, FrmGetObjectIndex(frmP, AddTextTextField));

			if(gTextToAdd)
			{
				SetFieldText(fldTextToAdd, gTextToAdd, false);
				FldSetSelection(fldTextToAdd, 0, StrLen(gTextToAdd));
			}
					
			FrmSetEventHandler(frmP, AddTextFormHandleEvent);
			FrmDoDialog(frmP);
			FrmEraseForm(frmP);
			FrmDeleteForm(frmP);
					
			gSelection = TEXT_SELECTION;
			handled = true;
			break;
			
		case MainTimestampPushButton:	

			gSelection = TIME_SELECTION;			
			handled = true;
			break;
	}

	return handled;
}

/***********************************************************************
 *
 * FUNCTION:    		PrvHandleDrawAreaOps
 *
 * DESCRIPTION: 		Deals with all the draw-operations performed within
 *						the draw-area
 *
 * PARAMETERS:  		EventType * eventP -> Pointer to the event structure
 *						selection -> Code for the operation which has been
 *										performed
 *
 * RETURNED:    		Boolean handled -> Whether the event has been handled 
 *											or not
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PrvHandleDrawAreaOps(EventType *eventP, UInt16 selection)
{
	Boolean handled = false;
	
	switch(selection)
	{
		case PAN_SELECTION:
			
			PrvHandlePanSelection(eventP);		
			handled = true;
			break;

		case CURVY_LINE_SELECTION:

			PrvHandleCurvyLineSelection(eventP);
			handled = true;
			break;

		case STRAIGHT_LINE_SELECTION:

			PrvHandleStraightLineSelection(eventP);				
			handled = true;
			break;
					
		case TEXT_SELECTION:
					
			PrvHandleTextSelection(eventP);
			handled = true;
			break;
						
		case TIME_SELECTION:

			PrvHandleTimeSelection(eventP);
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


/***********************************************************************
 *
 * FUNCTION:    	MainFormHandleEvent
 *
 * DESCRIPTION: 	Event-handler for the MainForm - handles the Done
 *					and Cancel buttons on the form, as well as the popup list	
 *
 * PARAMETERS:  	EventType * -> Pointer to the Event structure which is passed
 *					to the event handler
 *
 * RETURNED:    	Boolean handled -> denotes whether the event has been
 *					handled or not	
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

Boolean MainFormHandleEvent(EventType *eventP) 
{
	Boolean handled = false;
	Err err = errNone;
	
	RectangleType	colorPaletteRect = { {COLOR_PALETTE_TOPLEFT_X, COLOR_PALETTE_TOPLEFT_Y}, 
											{COLOR_PALETTE_WIDTH, COLOR_PALETTE_HEIGHT}};
		
	switch (eventP->eType) 
	{
		case frmOpenEvent:
			err = PrvInitMainForm();
			handled = true;
			break;
            
 		case ctlSelectEvent:
			handled = PrvHandleButton(eventP->data.ctlSelect.controlID);
			break;
		
		case menuEvent:
			PrvHandleMenu(eventP->data.menu.itemID);
			handled = true;
			break;
	
		case penDownEvent:
			// If the color palette is chosen
			if( RctPtInRectangle(eventP->screenX, eventP->screenY, &gScreenDrawAreaRect))
			{	
				RectangleType tempRect;
				
				WinGetClip(&tempRect);
				WinSetClip(&gScreenDrawAreaRect);
				
				handled = PrvHandleDrawAreaOps(eventP, gSelection);

				WinSetClip(&tempRect);

			}
			// If the color palette is chosen
			if( RctPtInRectangle(eventP->screenX, eventP->screenY, &colorPaletteRect))
			{
				PrvChangeColor(eventP->screenX, eventP->screenY);
				handled = true;
			}
			break;	
	
		case keyDownEvent:
			// Undo
			if(eventP->data.keyDown.chr == 0x08)	//Ascii for back-space
			{
				if(gOpIndex >= 0)
				{
					--gOpIndex;
					PrvDisplayImageOnScreen();
				}
				handled = true;
			} 
			// Redo
			else if(eventP->data.keyDown.chr == ' ')
			{
				if(gOpIndex < gOpLastIndex)
				{
					++gOpIndex;
					PrvDisplayImageOnScreen();
				}
				handled = true;
			}
			
			break;
		
		case frmCloseEvent:
			PrvDeInitMainForm();
			handled = true;
			break;
			
		default:
			break;
	}
 
	return handled;
}

/*@}*/
