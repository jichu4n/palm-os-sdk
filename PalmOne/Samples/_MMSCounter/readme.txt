
---------------------------------------------------------------------------
Name

	MMSCounter

---------------------------------------------------------------------------
Description
	
	This sample code shows how to register the app for incoming message notification, 
	and retrieve the number of unread SMS/MMS..  
	
Files:

  - MMSCounter.c : Main application. Does all the work.

  Major functions used:

  - extern void PrefSetAppPreferences (UInt32 creator, UInt16 id, Int16 version, 
	const void *prefs, UInt16 prefsSize, Boolean saved) : to set application preferences
	
  - extern Boolean PrefGetAppPreferencesV10 (UInt32 type, Int16 version, void *prefs,
	UInt16 prefsSize): to get application preferences	
	
  Major events used:
  - kHelperServiceClassIDMMS : Helpers of this Service Class send an SMS message
  - kHelperServiceClassIDSMS : New helper class registered with ACCESS for sending by MMS

---------------------------------------------------------------------------
Builds With

	CodeWarrior9
	GCC command line (Please use GCC_makefile to build)
	PODS (Please use PODS_makefile to build)

---------------------------------------------------------------------------
Devices

	Treo devices

---------------------------------------------------------------------------
Requirements
	
	
---------------------------------------------------------------------------
Libraries Used

	Helper service class

---------------------------------------------------------------------------
How to Run
	
	0. Send several SMS and MMS to the phone that will run this application
	   but do not read these messages when they come in
	1. Launch the 'MMSCounter' application
	2. Tap on "Count" button
	3. The sample code should count and display how many unread SMS, MMS and 
	   total number of unread messages.
	4. Go to Messaging app and read some messages
	5. Launch MMSCounter again and you should see the unread message numbers 
	   change accordingly.
   
---------------------------------------------------------------------------
Note
	
	MMSCounter_Rsc.h is automatically generated from GSMStatus_Rsc.rcp using Pilrc command. 

---------------------------------------------------------------------------