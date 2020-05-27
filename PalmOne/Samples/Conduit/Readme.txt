---------------------------------------------------------------------------
Name

   Conduit

---------------------------------------------------------------------------
Description

   ------------------------------------------------------------------------
   IMPORTANT: Running MLDesktopApp will create the file c:\PalmMusicLibDB.txt
   ------------------------------------------------------------------------

   The purpose of the Conduit sample application is to demonstrate how to 
   synchronize between records on the handheld and records on the desktop.

   The conduit includes three applications: 
   1. MLConduit
   2. MLDesktopApp
   3. MLDeviceApp

   MLDesktopApp and MLDeviceApp run on the desktop and a Palm device 
   respectively, and allow the user to add and remove CD titles from
   music libraries. After the MLConduit is registered with Hotsync, it will
   synchronize the CD titles on the desktop with the CD titles on the
   device. 
  
---------------------------------------------------------------------------
Builds With

   Palm OS Developer Suite
   Visual C++ .NET
   Codewarrior 9
   GCC command line (Please use GCC_makefile to build)


---------------------------------------------------------------------------
Devices


---------------------------------------------------------------------------
Requirements

---------------------------------------------------------------------------
Libraries Used

---------------------------------------------------------------------------
How to Run

   1. Register the conduit with Hotsync. Specify c:\PalmMusicLibDB.txt as the
      desktop file to synchronize
   2. Run MLDesktopApp and add and remove CD titles
   3. Run MLDeviceApp and add and remove CD titles
   4. Exit MLDesktopApp and MLDeviceApp 
   4. Do a Hotsync
   5. Run MLDesktopApp and MLDeviceApp and verify that records have been
      synchronized

---------------------------------------------------------------------------
Note

   The maximum number of records that can be entered between MLDesktopApp 
   and MLDeviceApp is 10. There can therefore be max. 10 records in either 
   database both before and after synchronization.

   The artists and titles need to be unique for the synchronization to work

   On the Centro device, it may be difficult to select a record in the
   MLDeviceApp. The number column needs to be tapped in order to select a 
   row to delete.

---------------------------------------------------------------------------
Known Issues
	MLSDesktopApp
		If you have multiple records with the same artist name and you try to delete one of them.  
		All of the records with that artist name will be deleted.
		This is a program logic bug, and have no implications on the conduit APIs used. This is scheduled
		to be fixed.
