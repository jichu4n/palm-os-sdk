/********************************************************************************
*
* File:        Form1.h
* Description: Contains the Form1 class. It creates a form that contains a 
*              checkedlistbox displaying the music library
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
#pragma once

#include <string>
#include <time.h>
#include "AddEntryForm.h"

#define MAX_LIBRARY_SIZE   10
#define MAX_STRING_LENGTH 100
#define MAX_BUFFER_LENGTH 300
#define MAX_DELETE_BUFFER 30000
#define CONDUIT_DELETE_FLAG 0x2

char sFilename[] = "C:\\PalmMusicLibDB.txt";
char sDeleteBuffer[MAX_DELETE_BUFFER];

namespace MusicLibrary
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	//using namespace System::String;

	/// <summary> 
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class Form1 : public System::Windows::Forms::Form
	{	
	public:
		Form1(void)
		{
			dIndex  = 10;
			dRecID  = 1000;
			fHandle = NULL;
            strcpy(sDeleteBuffer, "");

			InitializeComponent();
			PopulateLibrary();
		}
  
	protected:
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Button *  button1;
	private: System::Windows::Forms::Button *  button2;
	private: System::Windows::Forms::MainMenu *  mainMenu1;

	private: System::Windows::Forms::Button *  Add;
	private: System::Windows::Forms::Button *  Delete;


	private: System::Windows::Forms::CheckedListBox *  checkedListBox;
	private: System::Data::DataView *  dataView1;
	private: System::Windows::Forms::Button *  Quit;

	private: FILE *fHandle;
             int  dIndex, dRecID;
	private: System::Windows::Forms::Label *  label1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container * components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Configuration::AppSettingsReader *  configurationAppSettings = new System::Configuration::AppSettingsReader();
			this->Add = new System::Windows::Forms::Button();
			this->Delete = new System::Windows::Forms::Button();
			this->mainMenu1 = new System::Windows::Forms::MainMenu();
			this->checkedListBox = new System::Windows::Forms::CheckedListBox();
			this->dataView1 = new System::Data::DataView();
			this->Quit = new System::Windows::Forms::Button();
			this->label1 = new System::Windows::Forms::Label();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->dataView1))->BeginInit();
			this->SuspendLayout();
			// 
			// Add
			// 
			this->Add->BackColor = System::Drawing::Color::Gainsboro;
			this->Add->Location = System::Drawing::Point(240, 392);
			this->Add->Name = S"Add";
			this->Add->TabIndex = 6;
			this->Add->Text = S"Add";
			this->Add->Click += new System::EventHandler(this, Add_Click);
			// 
			// Delete
			// 
			this->Delete->BackColor = System::Drawing::Color::Gainsboro;
			this->Delete->Location = System::Drawing::Point(352, 392);
			this->Delete->Name = S"Delete";
			this->Delete->TabIndex = 5;
			this->Delete->Text = S"Delete";
			this->Delete->Click += new System::EventHandler(this, Delete_Click);
			// 
			// checkedListBox
			// 
			this->checkedListBox->BackColor = System::Drawing::Color::LightSteelBlue;
			this->checkedListBox->CheckOnClick = true;
			this->checkedListBox->ColumnWidth = (*__try_cast<__box System::Int32 *  >(configurationAppSettings->GetValue(S"checkedListBox.ColumnWidth", __typeof(System::Int32))));
			this->checkedListBox->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->checkedListBox->Location = System::Drawing::Point(24, 56);
			this->checkedListBox->Name = S"checkedListBox";
			this->checkedListBox->Size = System::Drawing::Size(616, 290);
			this->checkedListBox->Sorted = true;
			this->checkedListBox->TabIndex = (*__try_cast<__box System::Int32 *  >(configurationAppSettings->GetValue(S"checkedListBox.TabIndex", __typeof(System::Int32))));
			this->checkedListBox->ThreeDCheckBoxes = true;
			// 
			// Quit
			// 
			this->Quit->BackColor = System::Drawing::Color::Gainsboro;
			this->Quit->Location = System::Drawing::Point(560, 392);
			this->Quit->Name = S"Quit";
			this->Quit->Size = System::Drawing::Size(80, 24);
			this->Quit->TabIndex = 11;
			this->Quit->Text = S"Quit";
			this->Quit->Click += new System::EventHandler(this, Quit_Click);
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::Color::LightSteelBlue;
			this->label1->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label1->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->label1->Location = System::Drawing::Point(24, 24);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(616, 24);
			this->label1->TabIndex = 12;
			this->label1->Text = S"MUSIC   LIBRARY";
			this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// Form1
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->BackColor = System::Drawing::Color::DimGray;
			this->ClientSize = System::Drawing::Size(664, 446);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Quit);
			this->Controls->Add(this->checkedListBox);
			this->Controls->Add(this->Delete);
			this->Controls->Add(this->Add);
			this->Menu = this->mainMenu1;
			this->Name = S"Form1";
			this->Text = S"Music Library";
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->dataView1))->EndInit();
			this->ResumeLayout(false);

		}	





		void PopulateLibrary(void);
		System::Void Add_Click(System::Object *  sender, System::EventArgs *  e);
		Void Delete_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void Quit_Click(System::Object *  sender, System::EventArgs *  e);
		void Add_Entry_To_CheckedListBox(String *sArtist, String *sTitle);
        void Add_Entry(char *sAttrs, char *sTitle, char *sArtist);
		void Recreate_Datafile(char *sBuffer);
	};
}


