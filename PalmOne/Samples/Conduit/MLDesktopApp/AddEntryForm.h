/********************************************************************************
*
* File:        AddEntryForm.h
* Description: Creates the AddEntryForm class. This form prompts the user for a
*              music library entry containing an artist and a title
* History:     May 26 2006 - DTS created
* 
/********************************************************************************/
#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing; 


namespace MusicLibrary
{
	/// <summary> 
	/// Summary for AddEntryForm
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class AddEntryForm : public System::Windows::Forms::Form
	{
	public: 
		AddEntryForm(void)
		{
			InitializeComponent();
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
	private: System::Windows::Forms::TextBox *  textBox1;
	private: System::Windows::Forms::TextBox *  textBox2;
	private: System::Windows::Forms::Label *  label1;
	private: System::Windows::Forms::Label *  label2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Configuration::AppSettingsReader *  configurationAppSettings = new System::Configuration::AppSettingsReader();
			this->button1 = new System::Windows::Forms::Button();
			this->button2 = new System::Windows::Forms::Button();
			this->textBox1 = new System::Windows::Forms::TextBox();
			this->textBox2 = new System::Windows::Forms::TextBox();
			this->label1 = new System::Windows::Forms::Label();
			this->label2 = new System::Windows::Forms::Label();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->button1->Location = System::Drawing::Point(48, 208);
			this->button1->Name = S"button1";
			this->button1->TabIndex = 0;
			this->button1->Text = S"Add";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(160, 208);
			this->button2->Name = S"button2";
			this->button2->TabIndex = 1;
			this->button2->Text = S"Cancel";
			this->button2->Click += new System::EventHandler(this, Cancel_Click);

			// 
			// textBox1
			// 
			this->textBox1->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->textBox1->Location = System::Drawing::Point(48, 56);
			this->textBox1->Name = S"textBox1";
			this->textBox1->Size = System::Drawing::Size(184, 26);
			this->textBox1->TabIndex = 2;
			this->textBox1->Text = (__try_cast<System::String *  >(configurationAppSettings->GetValue(S"textBox1.Text", __typeof(System::String))));
			// 
			// textBox2
			// 
			this->textBox2->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->textBox2->Location = System::Drawing::Point(48, 144);
			this->textBox2->Name = S"textBox2";
			this->textBox2->Size = System::Drawing::Size(184, 26);
			this->textBox2->TabIndex = 3;
			this->textBox2->Text = (__try_cast<System::String *  >(configurationAppSettings->GetValue(S"textBox2.Text", __typeof(System::String))));
			// 
			// label1
			// 
			this->label1->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label1->Location = System::Drawing::Point(48, 32);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(112, 24);
			this->label1->TabIndex = 4;
			this->label1->Text = S"Artist";
			// 
			// label2
			// 
			this->label2->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label2->Location = System::Drawing::Point(48, 120);
			this->label2->Name = S"label2";
			this->label2->TabIndex = 5;
			this->label2->Text = S"Title";
			// 
			// AddEntryForm
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->BackColor = System::Drawing::Color::LightSteelBlue;
			this->ClientSize = System::Drawing::Size(280, 254);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = S"AddEntryForm";
			this->Text = S"Add Library Entry";
			this->ResumeLayout(false);

		}		   
		Void AddEntryForm::Cancel_Click(System::Object *  sender, System::EventArgs *  e);

	 public:
		String *AddEntryForm::GetArtistStr();
		String *AddEntryForm::GetTitleStr();
	};
}



