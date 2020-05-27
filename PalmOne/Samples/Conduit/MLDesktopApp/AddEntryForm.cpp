/********************************************************************************
*
* File:        AddEntryForm.cpp
* Description: Creates the AddEntryForm class. This form prompts the user for a
*              music library entry containing an artist and a title
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
#include "StdAfx.h"
#include "AddEntryForm.h"

using namespace MusicLibrary;


/********************************************************************************
*
* Function:    GetArtistStr
* Description: Returns the artist string from the artist text field
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
String *AddEntryForm::GetArtistStr()
{
    return textBox1->Text;
}	
		
/********************************************************************************
*
* Function:    GetTitleStr
* Description: Returns the title string from the title text field
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
String *AddEntryForm::GetTitleStr()
{
    return textBox2->Text;
}
	
/********************************************************************************
*
* Function:    Cancel_Click
* Description: Closes the form
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
Void AddEntryForm::Cancel_Click(System::Object *  sender, System::EventArgs *  e)
{
    this->Dispose(true);
}
