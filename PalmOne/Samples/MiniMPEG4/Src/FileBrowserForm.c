/***********************************************************************
 *
 * Copyright (c) 2004 PalmOne, Inc. or its subsidiaries.
 * All rights reserved.
 *
 ***********************************************************************/

/***********************************************************************
 *
 * File:
 *	FileBrowserForm.c
 *
 * Description:
 *	File browsing form
 *
 *
 ***********************************************************************/
 
#include <PalmOS.h>
#include <VfsMgr.h>
#include <DataMgr.h>
#include <PalmUtils.h>
#include <HsNav.h>
#include <HsKeyCommon.h>

#include "Common.h"
#include "FileBrowserForm.h"
#include "FileBrowserFormRsrc.h"

/***********************************************************************
 *
 *	Internal Defines
 *
 ***********************************************************************/

#define dataBaseType 	'temp'
#define dbCardNumber	0

/***********************************************************************
 *
 *	Internal typedef
 *
 ***********************************************************************/

typedef struct
{
	Boolean selected;
	UInt16	attribute;
	UInt32	fileSize;
	Char*	fileName;

} FileBrowserFileInfo;

/***********************************************************************
 *
 *	Internal functions
 *
 ***********************************************************************/

Err 	PrvFormInit(FormType* frmP);
void	PrvFormDeInit(FormType* frmP);

Err		PrvEnumerate();
Err		PrvEmptyDB();
Err 	PrvAddRecord(FileInfoType* fileInfoP, UInt32 fileSize);
Err 	PrvGetRecord(UInt16 index, FileBrowserFileInfo* fileInfoP, MemHandle* hP);
void 	PrvReleaseRecord(UInt16 index, MemHandle h);
void 	PrvDBSort();

void 	PrvListViewInit(FormType* frmP);
void 	PrvListViewDrawFile(void* tableP, Int16 row, Int16 column, RectangleType* boundsP);
void 	PrvListViewLoadTable(FormType* frmP, Boolean redraw);
void 	PrvDrawTableBorder(FormType* frmP);
void 	PrvSelectFile(UInt16 index);
void FileBrowserTestScrolling(TablePtr tableP);


/***********************************************************************
 *
 *	Internal Globals
 *
 ***********************************************************************/

static DmOpenRef 	gDB = NULL;			// Global pointer to the database
static UInt16		gRowCount = 0;		// Number of row in table/list view
static UInt16		gTopIndex = 0;		// Top index item of the table
static UInt32		gFileCount = 0;		// Number of files in database
static UInt16		gVolumeCount = 0;	// Number of cards
static UInt16		gCurrentVolume = 0;
static Char			gCurrentPath[MAX_PATH_LENGTH] = { NULL };
static Char			gCurrentFile[MAX_FILENAME_LENGTH] = { NULL };

static WinHandle	gDocumentWinH = 0;	// Document Bitmap buffer
static WinHandle 	gFolderWinH = 0;	// Folder Bitmap buffer
static UInt32		gReturnForm = 0;	// Form to return to
static FileBrowserCallback gCallback = NULL; // Callback

//static FileBrowserView gViewType = DIRECTORY | FILE; // Show files and folders
//static FileBrowserSelection	gSelectionType = SINGLE; // Single selection

Int8 selectedRow;

//flag to keep track of when scrolling has been used
Boolean gScrolling = false;
//flag to keep track of when the 5-way has been used 
//this flag is true when 5-way is used but need not be set to false
Boolean gUsedUpDown = false;



//#pragma mark ---------- File Browser Functions ----------

/***********************************************************************
 *
 * FUNCTION:    FileBrowserFormHandleEvent
 *
 * DESCRIPTION: This routine handles the events for the File Browser Form
 *
 * PARAMETERS:  eventP	- The pointer to the event.
 *
 * RETURNED:    True if handled.
 *
 ***********************************************************************/

Boolean FileBrowserFormHandleEvent(EventType * eventP)
{
    TablePtr tableP;
    Boolean handled = false;
    FormType * frmP = FrmGetActiveForm();

    switch (eventP->eType) 
        {        
		   case keyDownEvent:
	           
	              switch( eventP->data.keyDown.chr)
	              {
	                 case vchrRockerUp:
	                 case vchrPageUp:
	                 {
	   		            tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));	            
	   		            
	   		            /* to get 5-way to sync with scrolling */
	   		            FileBrowserTestScrolling(tableP);
    		            	
    		            //after we sync 5-way with scrolling, we can set the flag to false
    		            gScrolling = false;
  
						// Check that the selected row is within bounds and adjust focus to the selected row

	                    // Table is highlighted, we're not at the top of list
	                    if( selectedRow != -1 && selectedRow != 0) { 
	                       // Scroll up
	                       if(selectedRow == gTopIndex) {    					      
	                          gTopIndex--;	 
	                          selectedRow--;       	        
				        	  PrvListViewLoadTable(frmP, true);
				        	  TblUnhighlightSelection(tableP);            
				        	  TblRedrawTable(tableP);				 
	   			           } else { // No scrolling
	   					      selectedRow--;				    			           
	   			           }
	   	  	               TblSelectItem (tableP, selectedRow - gTopIndex, 0);  
	   	  	            } 
	   	  	            
	   	  	            // set flag to show that 5-way has been used  
    	  	            gUsedUpDown = true;

	   	  	               	  	                
	                    handled = true;
	                    break;
	                 }
	                  
	                 case vchrRockerDown:
	                 case vchrPageDown:
	                 {
	   		            tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));	            
	   		            
	   		            /* to get 5-way to sync with scrolling */
	   		            FileBrowserTestScrolling(tableP);
    		            	
    		            //after we sync 5-way with scrolling, we can set the flag to false
    		            gScrolling = false;

						// Check that the selected row is within bounds and adjust focus to the selected row

	                    // Table is highlighted and we're not at the bottom of list
	                    if(selectedRow != -1 && selectedRow != (gFileCount - 1)) {

	                      if(selectedRow < (gTopIndex + gRowCount - 1)) { // No scrolling 
	                         selectedRow++;
	   		                 TblSelectItem (tableP, selectedRow - gTopIndex, 0);
	                        
	   		              } else { // Scroll
	   		              
	   					     gTopIndex++;	 
	   					     selectedRow++;       	        
				        	 TblUnhighlightSelection(tableP);            
				        	 TblRedrawTable(tableP);
				        	 PrvListViewLoadTable(frmP, true);
	     	  	             TblSelectItem (tableP, selectedRow - gTopIndex, 0);
				          }			 
	   				    }		 
			 			
			 			// set flag to show that 5-way has been used
    				    gUsedUpDown = true;	
			 			
	                    handled = true;
	                    break;
	                 }
	                 
	                 case vchrRockerRight:
	                 {
		                 // Table is highlighted. Set focus on cancel button
		   				 if( selectedRow != -1 ) {
		   
		   		             tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));	            
		   	                 TblUnhighlightSelection(tableP);
		   	                 FrmSetFocus(frmP, FrmGetObjectIndex(frmP, FileBrowserCancelButton));  
		   	                 PrvFormInit(frmP);
		   	                 FrmDrawForm(frmP);  
		   	                 PrvDrawTableBorder(frmP);
		   	                 selectedRow = -1;
		   	             }
	                     handled = true;
	                     break;
	                 }
	                 
	                 case vchrRockerLeft:
	                 { 
	                    // Cancel button is highlighted. Set focus on table
	                    if(selectedRow == -1) {
	                        selectedRow = 0;
	                        gTopIndex = 0;
	                        tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));	
	                        PrvFormDeInit(frmP);              
		           	        PrvFormInit(frmP);
		       	            FrmDrawForm(frmP);  
		           	        PrvDrawTableBorder(frmP);
		           	        FrmNavObjectTakeFocus(frmP, FileBrowserFileTable);
		           	        TblSelectItem (tableP, selectedRow, 0);
	                    }
	                    handled = true;
	                    break;
	                 }   
	                 
	                 case vchrHardRockerCenter:
	                 case vchrRockerCenter:
	                  	
	                  	tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));
	                    
	                    /* to get 5-way to sync with scrolling */
	   		         	FileBrowserTestScrolling(tableP);
   		             	
	                    // Table is highlighted. Select file/folder
	                    if(selectedRow != -1) {
	                    
	                    	selectedRow = selectedRow - gTopIndex;
                    		gTopIndex = 0;
                    		PrvSelectFile(TblGetRowID(tableP, selectedRow));

	                    	selectedRow = 0;
		   					TblSelectItem (tableP, selectedRow, 0);               
		   				// Cancel button is highlighted. Go back to main form		
	                    } else {                         
	                       gTopIndex = 0; 	        	        
				           PrvListViewLoadTable(frmP, true);
	                       FrmGotoForm(gReturnForm);
	                    }
	                    
	                    //after we sync 5-way with scrolling and select our file, we can set the flag to false
                        gScrolling = false;

	                    
	                    handled = true;
	                    break;
	                      
	         }
	         handled = true;
	         break;

        case frmOpenEvent:
        	PrvFormInit(frmP);
	        FrmDrawForm(frmP);
	        PrvDrawTableBorder(frmP);
	        FrmNavObjectTakeFocus(frmP, FileBrowserFileTable);
	        tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));	            
  	        TblSelectItem (tableP, selectedRow, 0);

	        handled = true;
            break;
            
        case ctlSelectEvent:
        	switch(eventP->data.ctlSelect.controlID)
        	{
        	case FileBrowserCancelButton:
        		FrmGotoForm(gReturnForm);
        		handled = true;
        		break;
        	}
        	break;
        case sclRepeatEvent:
        	tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));
        	gTopIndex += (eventP->data.sclRepeat.newValue - eventP->data.sclRepeat.value);
      	
        	PrvListViewLoadTable(frmP, true);
        	TblUnhighlightSelection(tableP);
        	TblRedrawTable(tableP);
                	
        	TblSelectItem (tableP, 0, 0);
        	//Show that scrolling has been used
 			gScrolling = true;

        	
        	handled = false;
        	break;
            
        case frmUpdateEvent:
        	PrvDrawTableBorder(frmP);
            break;
            
        case frmCloseEvent:
        	PrvFormDeInit(frmP);
        	selectedRow = 0;
        	gTopIndex = 0;
        	handled = false;
			break;
		
		case tblEnterEvent:
			break;
		
		case tblSelectEvent:
			if( gFileCount > eventP->data.tblSelect.row  )
			{
				gTopIndex = 0;
				PrvSelectFile(TblGetRowID(eventP->data.tblSelect.pTable, eventP->data.tblSelect.row));
				//after we select our file, we can set the scrolling flag to false
 				gScrolling = false;
				
				handled = true;
			}
			break;

        default:
            break;
        }
    
    return handled;
}


/***********************************************************************
  *
  * FUNCTION:    FileBrowserTestScrolling
  *
  * DESCRIPTION: 
  *
  * PARAMETERS:  
  *
  * RETURNED:    
  *
  ***********************************************************************/
  
  void FileBrowserTestScrolling(TablePtr tableP)
  {
  	
     /* If scrolling has been used, the focus is moved to the top item in the frame */
 	if (gScrolling == true) {
    		selectedRow = gTopIndex;
    		TblSelectItem (tableP, selectedRow, 0);
    		            	
    	}
    	
  }   



/***********************************************************************
 *
 * FUNCTION:    FileBrowserSetCallback
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

void FileBrowserSetCallback(FileBrowserCallback callback, UInt32 formID)
{
	gCallback = callback;
	gReturnForm = formID;
}

/***********************************************************************
 *
 * FUNCTION:    FileBrowserIsCallbackSet
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Boolean FileBrowserIsCallbackSet()
{
	return (gCallback)?true:false;
}

/***********************************************************************
 *
 * FUNCTION:    FileBrowserIsReturnFormSet
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Boolean FileBrowserIsReturnFormSet()
{
	return (gReturnForm)?true:false;
}

/***********************************************************************
 *
 * FUNCTION:    FileBrowserEnumerateSelection
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

Err FileBrowserEnumerateSelection(UInt16 *volume, Char* path, Char* file, fbIterator *iterator)
{
	Err err = errNone;

	if( *iterator == fbIteratorStop ) return fbErrNoMoreSelection;
	
	// Find the first selection from the iterator
		
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    FileBrowserRefresh
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

void FileBrowserRefresh()
{
	FormType *frmP = FrmGetActiveForm();
	// Empty the current database
	PrvEmptyDB();
	
	// Load new files
	gRowCount = 0;
	gTopIndex = 0;
	gFileCount = 0;
	gVolumeCount = 0;
	gCurrentVolume = 0;
	*gCurrentPath =  NULL;
	*gCurrentFile =  NULL;
	
	// Redraw the form
	if( frmP )
	{
		PrvFormInit(frmP);
		FrmDrawForm(frmP);
		PrvDrawTableBorder(frmP);
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvSelectFile
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

void PrvSelectFile(UInt16 index)
{
	MemHandle h;
	FileBrowserFileInfo fileInfo;
    FormType * frmP = FrmGetActiveForm();
	
	// Check for error. Will also catch empty SD card.
	if(PrvGetRecord(index, &fileInfo, &h) != errNone)	
		return;
	
	if( fileInfo.attribute & vfsFileAttrDirectory )
	{
		if( !StrCompare("..", fileInfo.fileName) )
		{
			// One directory above
			UInt32 strLen = StrLen(gCurrentPath);
			// Skip last '/' <- always there
			strLen -= 2;
			while( gCurrentPath[strLen--] != '/' ) {}
			// Don't delete the first '/'
			strLen += 2;
			gCurrentPath[strLen] = NULL;
			
		}
		else
		{
			// One directory below
			StrCat(gCurrentPath, fileInfo.fileName);
			StrCat(gCurrentPath, "/");
		}
		
		PrvReleaseRecord(index, h);
		
		// Clean up database
		PrvEmptyDB();
				
		// Enumerate with the new path
		PrvEnumerate();
		
  		// Display the path
		{
			FieldType *fieldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserPathField));
			SetFieldTextFromStr(fieldP, gCurrentPath, false);
		}
		
		// Reload the table
		PrvListViewLoadTable(frmP, true);
		
		FrmDrawForm(frmP);
		
		PrvDrawTableBorder(frmP);
	}
	else
	{
		StrCopy(gCurrentFile, fileInfo.fileName);
		PrvReleaseRecord(index, h);
			
		// Play it!
		if( gCallback ) {
			gCallback(gCurrentVolume, gCurrentPath, gCurrentFile); 
		}
		if( gReturnForm ) {
			FrmGotoForm(gReturnForm);
		}
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvFormInit
 *
 * DESCRIPTION: This routine initialize the File Browser form.
 *
 * PARAMETERS:  frmP	- Pointer to the form.
 *
 * RETURNED:    Error Code.
 *
 ***********************************************************************/

Err PrvFormInit(FormType *frmP)
{
	Err err = errNone;
	UInt32 volumeIterator;
	Boolean enumerate = false;
	LocalID 	dbId;
	
	if( !gDB )
	{
		// Open the database
		dbId = DmFindDatabase(fbCardNumber, fbDBName);
		if( dbId )
		{
			// If exists open it
			gDB = DmOpenDatabase(fbCardNumber, dbId, dmModeReadWrite);
			if(!gDB) {
				err = DmGetLastErr();
				goto Done;
			}
		}
		else
		{
			// Else create the database
			err = DmCreateDatabase(fbCardNumber, fbDBName, fbCreatorID, 'temp', false);
			if (err != errNone)	return err;
			
			dbId = DmFindDatabase(fbCardNumber, fbDBName);
			if( !dbId ) {
				err = DmGetLastErr();
				goto Done;
			}
			
			gDB = DmOpenDatabase(fbCardNumber, dbId, dmModeReadWrite);
			if( !gDB ) {
				err = DmGetLastErr();
				goto Done;
			}
			
			// Need to enumerate the files
			enumerate = true;
		}
	}
	
	//
	// Get the first card if no volume selected
	//
	if( !gCurrentVolume )
	{
		volumeIterator = vfsIteratorStart;
		err = VFSVolumeEnumerate(&gCurrentVolume, &volumeIterator);
		if( err == expErrEnumerationEmpty ) {
			gCurrentVolume = 0;
		}
		enumerate = true;
	}
	
	//
	// Set path to the root if not set
	//
	if( !(*gCurrentPath) )
	{
		StrCopy((Char*)gCurrentPath, "/");
		enumerate = true;
	}
	
	//
	// No file selected for now
	//
	*gCurrentFile = NULL;
	
	//
	// Load the files in the Database if needed
	//
	if( enumerate )	{
		PrvEnumerate();
	}
	
	//
	// Load the bitmaps
	//
	{
		MemHandle h;
		BitmapType* bmpP;
		Coord width, height;
		WinHandle winH = WinGetDrawWindow();
		Err err;

		h = DmGet1Resource(bitmapRsc, FolderBitmapFamily);
		bmpP = (BitmapType*)MemHandleLock(h);
		BmpGetDimensions(bmpP, &width, &height, 0);
		gFolderWinH = WinCreateOffscreenWindow(width, height, nativeFormat, &err);
		WinSetDrawWindow(gFolderWinH);
		WinDrawBitmap(bmpP, 0, 0);
		MemHandleUnlock(h);
		DmReleaseResource(h);
		
		h = DmGet1Resource(bitmapRsc, DocumentBitmapFamily);
		bmpP = (BitmapType*)MemHandleLock(h);
		BmpGetDimensions(bmpP, &width, &height, 0);
		gDocumentWinH = WinCreateOffscreenWindow(width, height, nativeFormat, &err);
		WinSetDrawWindow(gDocumentWinH);
		WinDrawBitmap(bmpP, 0, 0);
		MemHandleUnlock(h);
		DmReleaseResource(h);
		
		WinSetDrawWindow(winH);
	}
	
	//
	// Display the path
	//
	{
		FieldType *fieldP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserPathField));
		SetFieldTextFromStr(fieldP, gCurrentPath, false);
	}

Done:	
	//
	// Setup the table
	//
	PrvListViewInit(frmP);
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    PrvFormDeInit
 *
 * DESCRIPTION: This routine de-initialize the File Browser form.
 *
 * PARAMETERS:  frmP	- Pointer to the form.
 *
 * RETURNED:    None.
 *
 ***********************************************************************/

void PrvFormDeInit(FormType* frmP)
{
	// Release the offscreen windows
	if( gFolderWinH ) {
		WinDeleteWindow(gFolderWinH, false);
		gFolderWinH = 0;
	}
	
	if( gDocumentWinH ) {
		WinDeleteWindow(gDocumentWinH, false);
		gDocumentWinH = 0;
	}
	
	// Close the database
	if( gDB ) {			
		DmCloseDatabase(gDB);
		gDB = NULL;
	}
}

/***********************************************************************
 *
 * FUNCTION:    PrvEnumerate
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:  None.
 *
 * RETURNED:    N/A.
 *
 ***********************************************************************/

Err PrvEnumerate()
{
	Err 			err = errNone;
	FileRef 		dirRef = 0;
	FileRef			fileRef = 0;
	FileInfoType	fileInfo;
	//UInt32			fileType = 0;
	UInt32			fileSize = 0;
	UInt32 			fileIterator = 0;
	
	Char *fileName = (Char*)MemPtrNew(MAX_FILENAME_LENGTH);
	Char *fileName2 = (Char*)MemPtrNew(MAX_FILENAME_LENGTH);
	
	fileInfo.nameP = fileName;
	fileInfo.nameBufLen = MAX_FILENAME_LENGTH;
	
	// Clean the database
	PrvEmptyDB();
	
	// If not the root, add UP directory
	if( StrCompare( "/", gCurrentPath) )
	{
		StrCopy( fileName, "..");
		fileInfo.attributes = vfsFileAttrDirectory;
		PrvAddRecord(&fileInfo, 0);
	}
	
	// Open at the current volume and path	
	err = VFSFileOpen(gCurrentVolume, gCurrentPath, vfsModeRead, &dirRef);
	if( err == errNone )
	{	
		fileIterator = vfsIteratorStart;
		while( fileIterator != vfsIteratorStop )
		{		
			err = VFSDirEntryEnumerate(dirRef, &fileIterator, &fileInfo);
			if( err == errNone )
			{
				if( ! (fileInfo.attributes & UNWANTED_FILES) )
				{
					if( fileInfo.attributes & vfsFileAttrDirectory ) {
						PrvAddRecord(&fileInfo, 0);
					} else {
						StrCopy(fileName2, gCurrentPath);
						StrCat(fileName2, fileInfo.nameP);
						err = VFSFileOpen(gCurrentVolume, fileName2, vfsModeRead, &fileRef);
						err = VFSFileSize(fileRef, &fileSize);
						PrvAddRecord(&fileInfo, fileSize);
						err = VFSFileClose(fileRef);
					}
				}
			}
		}
		
		VFSFileClose( dirRef );
	}
	
	MemPtrFree(fileName);
	MemPtrFree(fileName2);
	
	
	gFileCount = DmNumRecords(gDB);
	
	PrvDBSort();
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    PrvDrawTableBorder
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    N/A
 *
 ***********************************************************************/

void PrvDrawTableBorder(FormType *frmP)
{
	RectangleType bounds;
	TablePtr tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));

	TblGetBounds(tableP, &bounds);
	WinDrawRectangleFrame(menuFrame, &bounds);
}

//#pragma mark ---------- File Browser DB Functions ----------

/***********************************************************************
 *
 * FUNCTION:    FileBrowserDeleteDB
 *
 * DESCRIPTION: This routine should be called when the app stops.
 *				It deletes the temporary database.
 *
 * PARAMETERS:  None.
 *
 * RETURNED:    Error Code.
 *
 ***********************************************************************/

Err FileBrowserDeleteDB()
{
	LocalID dbID;
	UInt16 cardNo;

	Err err = errNone;
	
	if(!gDB)
	{
		// Open the database
		dbID = DmFindDatabase(fbCardNumber, fbDBName);
		if( dbID )
		{
			// If exists open it
			gDB = DmOpenDatabase(fbCardNumber, dbID, dmModeReadWrite);
			if(!gDB) {
				err = DmGetLastErr();
				return err;
			}
		}
		else
			return err;
	}
	
	err = DmOpenDatabaseInfo(gDB, &dbID, 0, 0, &cardNo, 0);
	err = DmCloseDatabase(gDB);
	err = DmDeleteDatabase(cardNo, dbID);		
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    PrvEmptyDB
 *
 * DESCRIPTION: This routine deletes all the records of the temp database.
 *
 * PARAMETERS:  None.
 *
 * RETURNED:    Error Code.
 *
 ***********************************************************************/

Err PrvEmptyDB()
{
	Err err = errNone;
	UInt16 index = DmNumRecords(gDB);
	
	while (index--)
	{
		err = DmRemoveRecord(gDB, index);
	}
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    PrvAddRecord
 *
 * DESCRIPTION: This routine adds a record to the temp database
 *
 * PARAMETERS:  fileInfoP 		- Pointer to a FileInfo Structure
 *
 * RETURNED:    Error Code
 *
 ***********************************************************************/

Err PrvAddRecord(FileInfoType* fileInfoP, UInt32 fileSize)
{

	Err			err = errNone;
	void*		memLockP = NULL;
	MemHandle	recordHandle = NULL;
	UInt32		recordSize = 0;
	UInt16		recordIndex = dmMaxRecordIndex;
	UInt32		offset = 0;
	FileBrowserFileInfo fbFileInfo;
	
	// Local store
	fbFileInfo.selected = false;
	fbFileInfo.attribute = fileInfoP->attributes;
	fbFileInfo.fileName = fileInfoP->nameP;
	fbFileInfo.fileSize = fileSize;

	recordSize = (StrLen(fbFileInfo.fileName)+1);

	recordHandle = DmNewRecord(gDB, &recordIndex, recordSize + sizeof(UInt32) + sizeof(UInt16) + sizeof(Boolean));
	if( !recordHandle )
	{
		err = DmGetLastErr();
		return err;
	}
	
	memLockP = MemHandleLock(recordHandle);
	
	// Write selection
	offset = 0;
	err = DmWrite( memLockP, offset, &fbFileInfo.selected, sizeof(Boolean));
	if(err) goto Done;
	
	offset += sizeof(Boolean);
	err = DmWrite( memLockP, offset, &fbFileInfo.attribute, sizeof(UInt16));
	if(err) goto Done;
	
	offset += sizeof(UInt16);
	err = DmWrite( memLockP, offset, &fbFileInfo.fileSize, sizeof(UInt32));
	if(err) goto Done;
	
	offset += sizeof(UInt32);
	err = DmWrite( memLockP, offset, fbFileInfo.fileName, recordSize);
	if(err) goto Done;

Done:	
	MemHandleUnlock(recordHandle);
	err = DmReleaseRecord(gDB, recordIndex, true);
	
	if( err )
	{
		DmRemoveRecord(gDB, recordIndex);
	}
	
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    PrvGetRecord
 *
 * DESCRIPTION: This routine returns the file info for the given index
 *
 * PARAMETERS:  index		- File index in the database
 *				fileInfoP	- File info Ptr
 *
 * RETURNED:    
 *
 ***********************************************************************/

Err PrvGetRecord(UInt16 index, FileBrowserFileInfo* fileInfoP, MemHandle* hP)
{
	Err err = errNone;
	MemHandle recH;
	UInt8 *recP = NULL;
	
	recH = DmQueryRecord(gDB, index);
	if (recH == NULL)
	{
		err = DmGetLastErr();
		goto Done;
	}
	else *hP = recH;
	
	recP = (UInt8 *)MemHandleLock(recH);
	if (recP == NULL)
	{
		err = memErrChunkNotLocked;
		goto Done;
	}
	
	fileInfoP->selected = (Boolean)*recP;
	recP += sizeof(Boolean);
	fileInfoP->attribute = MisalignedReadBEUInt16(recP, 0);
	recP += sizeof(UInt16);
	fileInfoP->fileSize = MisalignedReadBEUInt32(recP, 0);
	recP += sizeof(UInt32);
	fileInfoP->fileName = (Char*)recP;
	
Done:
	return err;
}

/***********************************************************************
 *
 * FUNCTION:    PrvReleaseRecord
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/
 
void PrvReleaseRecord(UInt16 index, MemHandle h)
{
	MemHandleUnlock(h);
}

/***********************************************************************
 *
 * FUNCTION:    
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:  
 *
 * RETURNED:    
 *
 ***********************************************************************/

static Int16 PrvDBCompareRecords( Char* r1P, Char* r2P, 
	Int16 UNUSED_PARAM(param), SortRecordInfoType* UNUSED_PARAM(info1), 
	SortRecordInfoType* UNUSED_PARAM(info2), MemHandle UNUSED_PARAM(appInfoH))
{
	Int16 result;
	FileBrowserFileInfo rec1, rec2;
	
	rec1.selected = (Boolean)*r1P;
	r1P += sizeof(Boolean);
	rec1.attribute = MisalignedReadBEUInt16(r1P, 0);
	r1P += sizeof(UInt16) + sizeof(UInt32);
	rec1.fileName = (Char*)r1P;
	
	rec2.selected = (Boolean)*r2P;
	r2P += sizeof(Boolean);
	rec2.attribute = MisalignedReadBEUInt16(r2P, 0);
	r2P += sizeof(UInt16) + sizeof(UInt32);
	rec2.fileName = (Char*)r2P;


	// First check to display the folder first
	if (rec1.attribute & vfsFileAttrDirectory)
	{
		if (!(rec2.attribute & vfsFileAttrDirectory))
			return -1;
	}
	else if (rec2.attribute & vfsFileAttrDirectory)
			return 1;
			
	// Alphabetize;
	result = TxtCompare(	rec1.fileName,	// const Char *s1
							0xFFFF,			// UInt16 s1Len,
							NULL,			// UInt16 *s1MatchLen,
							rec2.fileName,	// const Char *s2,
							0xFFFF,			// UInt16 s2Len,
							NULL);			// UInt16 *s2MatchLen
	return result;
}
/************************************************************
 *
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 * 
 * RETURNS:
 *
 *************************************************************/
 
void PrvDBSort()
{
	DmInsertionSort(gDB, (DmComparF *)&PrvDBCompareRecords, 0);
}

//#pragma mark ---------- File Browser List View ----------

/***********************************************************************
 *
 * FUNCTION:    PrvListViewInit
 *
 * DESCRIPTION: This routine initializes the table
 *
 * PARAMETERS:  frmP 		- Pointer to a form
 *
 * RETURNED:    N/A
 *
 ***********************************************************************/

void PrvListViewInit(FormType *frmP)
{
	UInt16 rowCount;
	TablePtr tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));
	
	gRowCount = rowCount = TblGetNumberOfRows(tableP);
	
	// Init the row elements
	while( rowCount-- )
	{
		TblSetRowUsable(tableP, rowCount, false);
		TblSetItemStyle(tableP, rowCount, FILE_COLUMN, customTableItem);
	}
	
	TblHasScrollBar(tableP, true);
	TblSetColumnUsable(tableP, FILE_COLUMN, true);
	TblSetCustomDrawProcedure(tableP, FILE_COLUMN, PrvListViewDrawFile);
	
	// Load the table
	PrvListViewLoadTable(frmP, false);
}

/***********************************************************************
 *
 * FUNCTION:    PrvListViewDrawFile
 *
 * DESCRIPTION: This routine draws a file in the table
 *
 * PARAMETERS:  tableP 		- Pointer to the table
 *				row			- The row to draw
 *				column		- The column to draw
 *				boundsP		- Pointer to the element bound
 *
 * RETURNED:    N/A
 *
 ***********************************************************************/

void PrvListViewDrawFile(void *tableP, Int16 row, Int16 column, RectangleType *boundsP)
{
	UInt16 index;
	MemHandle h;
	FileBrowserFileInfo fileInfo;
	WinHandle winH;
	Char * ptr;
	UInt16 titleLen;
	Char text[32];
	UInt32 fileSizeWidth = 0;
	Err err = errNone;
	
	index = TblGetRowID(tableP, row);
	
	err = PrvGetRecord( index, &fileInfo, &h);
	if( err ) goto Done;
	
	// Draw icon
	ptr = StrChr (fileInfo.fileName, linefeedChr);
	titleLen = (ptr == NULL ? StrLen (fileInfo.fileName) : (UInt16) (ptr - fileInfo.fileName));
	
	if( fileInfo.attribute & vfsFileAttrDirectory )
	{
		RectangleType rct;
		
		winH = gFolderWinH;
				
		WinGetWindowFrameRect(winH, &rct);			
		WinCopyRectangle(winH, 0, &rct, boundsP->topLeft.x, boundsP->topLeft.y + 1, winPaint);
	}
	else
	{
		RectangleType rct;
		
		winH = gDocumentWinH;
				
		WinGetWindowFrameRect(winH, &rct);			
		WinCopyRectangle(winH, 0, &rct, boundsP->topLeft.x, boundsP->topLeft.y + 1, winPaint);

	}
	
	if( !(fileInfo.attribute & vfsFileAttrDirectory) )
	{
		// Display the file size
		if(fileInfo.fileSize > 1048576)
		{
			// Use the KB style (123.4 MB)
			UInt32 mantissa = fileInfo.fileSize / 1048576;
			UInt32 fraction = ( fileInfo.fileSize - mantissa * 1048576 ) / 104858;
			
			StrPrintF(text, "%ld.%ld M", mantissa, fraction);			
		}
		else if(fileInfo.fileSize > 1024)
		{
			// Use the KB style (123.4 KB)
			UInt32 mantissa = fileInfo.fileSize / 1024;
			UInt32 fraction = ( fileInfo.fileSize - mantissa * 1024 ) / 103;
			
			StrPrintF(text, "%ld.%ld K", mantissa, fraction);
		}
		else
		{
			// Use the B style (24 B)
			StrPrintF(text, "%ld B", fileInfo.fileSize);
		}
	
		// Get the length
		fileSizeWidth = FntCharsWidth(text, StrLen(text));
		WinDrawChars(text, StrLen(text), boundsP->topLeft.x + boundsP->extent.x - fileSizeWidth, boundsP->topLeft.y);
		//fileSizeWidth + 4; // Give me some space
	}
	
	// Draw File Name
	if( FntWidthToOffset (fileInfo.fileName, titleLen, boundsP->extent.x - 15 - fileSizeWidth, NULL, NULL) == titleLen)
	{
		WinDrawChars (fileInfo.fileName, titleLen, boundsP->topLeft.x + 15, boundsP->topLeft.y);
	}
	else
	{
		Int16 titleWidth;
		titleLen = FntWidthToOffset (fileInfo.fileName, titleLen, boundsP->extent.x - 15 - fileSizeWidth - FntCharWidth (chrEllipsis), NULL, &titleWidth);
		WinDrawChars (fileInfo.fileName, titleLen, boundsP->topLeft.x + 15, boundsP->topLeft.y);
		WinDrawChar (chrEllipsis, boundsP->topLeft.x + 15 + titleWidth, boundsP->topLeft.y);
	}
	
	// Release the record
	PrvReleaseRecord(index, h);
	
Done:
	return;
}

/***********************************************************************
 *
 * FUNCTION:    PrvListViewLoadTable
 *
 * DESCRIPTION: This routine loads the table.
 *
 * PARAMETERS:  frmP		- Pointer to a form.
 *
 * RETURNED:    N/A
 *
 ***********************************************************************/

void PrvListViewLoadTable(FormType *frmP, Boolean redraw)
{
	UInt16 row;
	UInt16 rowCount;
	UInt16 displayCount;
	TablePtr tableP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileTable));
	ScrollBarPtr barP = FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, FileBrowserFileScrollBar));
	
	rowCount = TblGetNumberOfRows(tableP);
	
	//If scrolling or 5-way have been used and there is only 1 file, reset gTopIndex

	if (((gScrolling == true)||(gUsedUpDown == true)) && gFileCount == 1){
 		gTopIndex = 0;
 		
 	}

	displayCount = gFileCount - gTopIndex;
	
	for(row = 0; row < gRowCount; row++)
	{
		if( row < displayCount )
		{
			TblSetRowID(tableP, row, row + gTopIndex);
			TblSetRowUsable(tableP, row, true);
			TblSetRowSelectable(tableP, row, true);
		}
		else
		{
			TblSetRowUsable(tableP, row, false);
			TblSetRowSelectable(tableP, row, false);
		}
	}
	
	if( redraw )
	{
		TblEraseTable(tableP);
		TblDrawTable(tableP);
	}
	
	if( gFileCount <= gRowCount )
	{
		SclSetScrollBar(barP, 0, 0, 0, gRowCount);
	}
	else
	{
		SclSetScrollBar(barP, gTopIndex, 0, gFileCount - gRowCount, gRowCount);
	}
}











