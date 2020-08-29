/********************************************************************************
*
* File:        Form1.cpp
* Description: Contains function implementations for the Form1 class. The class
*              creates a form that contains a checkedlistbox displaying a 
*              music library
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
#include "stdafx.h"
#include "Form1.h"
#include <windows.h>
#undef MessageBox //Avoids conflicting APIs

using namespace MusicLibrary;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
	Application::Run(new Form1());

    

	return 0;
}

/********************************************************************************
*
* Function:    ReadNextRecEntry
* Description: Get next string from buffer, starting at specified index
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
bool ReadNextRecEntry(char *sRecord, char *sRecEntry, int &index)
{
	int   newIndex = 0;
	bool  bDone    = false;
	char  sTmpEntry[MAX_STRING_LENGTH];

	// Get next string. Exit loop on newline or tab
	for(; *(sRecord + index) != '\n' && *(sRecord + index) != '\t'; index++) {
		sTmpEntry[newIndex] = *(sRecord + index);
		newIndex++;
	}

  	sTmpEntry[newIndex] = '\0';
	strcpy(sRecEntry, sTmpEntry);  

	// We are at the end of the buffer
	if(*(sRecord + index) == '\n') {
		bDone = true;
	}
	index++;
	return bDone;
}

/********************************************************************************
*
* Function:    Add_Entry_To_CheckedListBox
* Description: Add entry to checkedlistbox
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
void Form1::Add_Entry_To_CheckedListBox(String *sArtist, String *sTitle) 
{
			char   sEntry[MAX_BUFFER_LENGTH];
			String *sArray[] = {""};

            sprintf(sEntry, "%-s     %-s", sArtist, sTitle); 
		    sArray[0] = sEntry;
		    this->checkedListBox->Items->Add(sArray[0], false);
}

/********************************************************************************
*
* Function:    Add_Entry
* Description: If entry does not have delete flag set, add to checkedlistbox
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
void Form1::Add_Entry(char *sAttrs, char *sTitle, char *sArtist)
{
	String *sTmp      = new String("");
	String *sNewTitle = new String("");

	// Only display entries that don't have the delete flag set
	if(atoi(sAttrs) != CONDUIT_DELETE_FLAG) { 
		sTmp = System::String::Concat("\"", sTitle); 
		sNewTitle = System::String::Concat(sTmp, "\"");
		String *sNew = new String(sArtist);
		Add_Entry_To_CheckedListBox(sArtist, sNewTitle);
	}
}

/********************************************************************************
*
* Function:    PopulateLibrary
* Description: Read data file and populate library
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
void Form1::PopulateLibrary(void)
{
    char sLine[MAX_BUFFER_LENGTH];
	fHandle = fopen(sFilename, "a+");
	
	int  i = 0;
	int  count = 0;
	int  bufferIndex = 0;
	char sArtist[MAX_STRING_LENGTH];
	char sTitle[MAX_STRING_LENGTH];
	char sRecID[MAX_STRING_LENGTH];  // Record ID
    char sIndex[MAX_STRING_LENGTH];  // Record index
	char sAttrs[MAX_STRING_LENGTH];  // Record attributes

	while(fgets(sLine, MAX_BUFFER_LENGTH, fHandle) != NULL && i < MAX_LIBRARY_SIZE && strlen(sLine) != 0) {
		if(count == MAX_LIBRARY_SIZE) { break; }
		bufferIndex = 0;
		ReadNextRecEntry(sLine, sArtist, bufferIndex);
		ReadNextRecEntry(sLine, sTitle,  bufferIndex);
		ReadNextRecEntry(sLine, sRecID,  bufferIndex);
		ReadNextRecEntry(sLine, sIndex,  bufferIndex);
		ReadNextRecEntry(sLine, sAttrs,  bufferIndex);

		Add_Entry(sAttrs, sTitle, sArtist);
		count++;
	}
}


/********************************************************************************
*
* Function:    Add_Click
* Description: The add button was clicked. Read in the new library entry and add
*              to data file and checkedlistbox
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
System::Void Form1::Add_Click(System::Object *  sender, System::EventArgs *  e)
{
	int dAttrs = 8;
	char buffer[MAX_BUFFER_LENGTH];
	String *sArray[] = {""}; 

	// Give an error if library is full.
	if(this->checkedListBox->Items->Count == MAX_LIBRARY_SIZE) {					 
		MessageBox::Show(S"No more entries can be added", "Error - the library is full");
	} else {
		// Launch add form
		AddEntryForm *form = new AddEntryForm();
		form->ShowDialog();
		form->BringToFront();
		String *sNewTitle  = form->GetTitleStr();
		String *sNewArtist = form->GetArtistStr();

		// Add entry to file and checkedlistbox
		if(sNewArtist != "" && sNewTitle != "\"\"") {
			fseek (fHandle, 0, SEEK_END); 
			sprintf(buffer, "%s\t%s\t%d\t%d\t%d\n", sNewArtist, sNewTitle, dRecID++, dIndex++, dAttrs);
			fprintf(fHandle, "%s", buffer);    
			
			String *sTmp = System::String::Concat("\"", sNewTitle); 
			sNewTitle = System::String::Concat(sTmp, "\"");
			Add_Entry_To_CheckedListBox(sNewArtist, sNewTitle);					
		}
	}
}


/********************************************************************************
*
* Function:    Store_Deleted_Entry
* Description: Store deleted entry in deletedBuffer
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
void Store_Deleted_Entry(char *sEntry)
{
	int  index = 0;
	char sArg1[MAX_STRING_LENGTH];
    char sArg2[MAX_STRING_LENGTH];
	char sArg3[MAX_STRING_LENGTH];
	char sArg4[MAX_STRING_LENGTH];
	char tmpBuffer[MAX_BUFFER_LENGTH];

	ReadNextRecEntry(sEntry, sArg1, index);
	ReadNextRecEntry(sEntry, sArg2, index);
	ReadNextRecEntry(sEntry, sArg3, index);
	ReadNextRecEntry(sEntry, sArg4, index);

	// Write entry to buffer (with deleted attribute)
	sprintf(tmpBuffer, "%s\t%s\t%s\t%d\t2\n", sArg1, sArg2, sArg3, sArg4);
	if(!strcmp(sDeleteBuffer, "")) {
		strcpy(sDeleteBuffer, tmpBuffer);
	} else {
		strcat(sDeleteBuffer, tmpBuffer);
	}
}

/********************************************************************************
*
* Function:    Recreate_Datafile
* Description: Clear and write new buffer to data file
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
void Form1::Recreate_Datafile(char *sBuffer)
{
	fclose(fHandle);
	fHandle = fopen(sFilename, "w+");
	fprintf(fHandle, sBuffer);
	strcpy(sBuffer, "");
}

/********************************************************************************
*
* Function:    Delete_Click
* Description: The delete button was clicked. 
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
Void Form1::Delete_Click(System::Object *  sender, System::EventArgs *  e)
{	 	
	int  index;
	char sLine[MAX_BUFFER_LENGTH];
	char sArtist[MAX_STRING_LENGTH];
	char sTitle[MAX_STRING_LENGTH];
	char sBuffer[MAX_BUFFER_LENGTH * MAX_LIBRARY_SIZE] = "";
	String *sFormText = new String("");
	String *sTmp1 = new String("");
	String *sTmp2 = new String("");
	String* sSpaceDelim = S" ";
	String* sQuoteDelim = S"\"";
	Char    cQuoteDelim[] = {'\"'};
	String* splitArr[] = 0;

	int count = this->checkedListBox->Items->Count;
	MessageBox::Show(S"The selected entry/entries will be deleted from the library", "Deleting Selection");

	for (int i = 0; i < count; i++) { // Iterate over all entries in checkedListBox. Find and delete checked ones
		if(this->checkedListBox->GetItemChecked(i)) {
			// Parse and remove entry from checkedlistbox
			sFormText = this->checkedListBox->GetItemText(this->checkedListBox->Items->get_Item(i));
			this->checkedListBox->Items->Remove(this->checkedListBox->Items->get_Item(i));	
			i--;
			count--;
			sFormText->Trim();
	        String* sTmp = sFormText->TrimEnd(sSpaceDelim->ToCharArray());
			splitArr = sTmp->Split(cQuoteDelim, 2);
			String *sTmpArtist = splitArr[0]->TrimEnd(sSpaceDelim->ToCharArray());
			String *sTmpTitle  = splitArr[1]->TrimEnd(sQuoteDelim->ToCharArray());
            // Search for entry in file. All entries different than deleted entry, add to sBuffer. 
			// Deleted entry is added to deletedBuffer
			fseek (fHandle, 0, SEEK_SET);
   			while(fgets(sLine, MAX_BUFFER_LENGTH, fHandle) != NULL && i < MAX_LIBRARY_SIZE && strlen(sLine) != 0) {
				index = 0;
				ReadNextRecEntry(sLine, sArtist, index);
				ReadNextRecEntry(sLine, sTitle,  index);
				
				// Library entry is different from entry to delete so keep it in data file
				if(String::Compare(sArtist, sTmpArtist) != 0 && String::Compare(sTitle, sTmpTitle) != 0) {
					if(!strcmp(sBuffer, "")) {
						strcpy(sBuffer, sLine);
					} else {
						strcat(sBuffer, sLine);
					}							
				} else { // Found entry to delete. Add delete flag to it and store in deleteBuffer
					Store_Deleted_Entry(sLine);	  
				}
			}
		    Recreate_Datafile(sBuffer);
		}
	}
	fclose(fHandle);
	fHandle = fopen(sFilename, "r+");
}

/********************************************************************************
*
* Function:    Quit_Click
* Description: The quit button was clicked. Add deleted library entries to data
*              file (with delete attribute set) and close file. Clean up and exit 
*              the application
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
System::Void Form1::Quit_Click(System::Object *  sender, System::EventArgs *  e)
{	  
	fseek (fHandle, 0, SEEK_END);
 	fprintf(fHandle, sDeleteBuffer);
    fclose(fHandle);
	
	this->Dispose(true);
	Application::Exit();
}


