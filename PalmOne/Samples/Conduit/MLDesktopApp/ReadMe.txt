========================================================================
    APPLICATION : MusicLibrary Project Overview
========================================================================

AppWizard has created this MusicLibrary Application for you.  

This file contains a summary of what you will find in each of the files that
make up your MusicLibrary application.

MusicLibrary.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

Form1.cpp
    This is the main application source file.
    Contains the code to display the form.

Form1.h
    Contains the implementation of your form class and InitializeComponent() function.

AssemblyInfo.cpp
    Contains custom attributes for modifying assembly metadata.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Win32.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////

NOTE:
    This application creates a file under the c drive called PalmMusicLibDB.txt


Known Issues:
	If you have multiple records with the same artist name and you try to delete one of them.  
	All of the records with that artist name will be deleted.
	This is a program logic bug, and have no implications on the conduit APIs used. 
	This is scheduled to be fixed.