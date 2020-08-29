
---------------------------------------------------------------------------
Name

	GSMStatus

---------------------------------------------------------------------------
Description
	
	This sample code shows how to retriece radio connection status, operator name, and voicemail number on
	GSM phones.  
	
Files:

  - GSMStatus.c : Main application. Does all the work.
  - Common.c: Implementation of SetFieldTextFromStr()

  Major functions used:

  - HsGetPHoneLibrary ():	Load the phone library if not loaded, open and get the reference number to the library
  - PhnLibModulePowered ():	Check if the phone module is on or off.
  - PhnLibGetRadioState ():	Retrieve radio state information.
  - PhnLibRegistered (): 	Determine weather a smartphone is registered with a network.
  - PhnLibCurrentOperator (): 	Get value for current network’s operator string. 
  - PhnLibGetBoxNumber (): 	Return the number of the voice box of the given type for the given line as it is
  							stored on the SIM. 
  - PhnLibSignalQuality ():     Get the signal quality
  - PhnLibCurrentOperatorID (): Get MCC (Mobile Country Code) and MNC (Mobile Network Code)

  !!!NEW API FOR GNOME!!!
  - PhnLibGetGSMCellInfo():     Get Cell ID/COO (Cell of Origin) and LAC (Location Area Code)

---------------------------------------------------------------------------
Builds With

	CodeWarrior9
	GCC command line (Please use GCC_makefile to build)
	PODS (Please use PODS_makefile to build)

---------------------------------------------------------------------------
Devices

	Treo 650 GSM, Treo 680, Gnome

---------------------------------------------------------------------------
Requirements
	
	
---------------------------------------------------------------------------
Libraries Used

	Phone Library

---------------------------------------------------------------------------
How to Run
	
	1. Launch the 'GSM Status' application
	2. If the phone radio is on, you will see connection status, operator name, voicemail number and 
		radio status displayed on the main screen
	3. If the phone radio is off, tapping on 'refresh' will turn on the phone radio and the information 
		above will be displayed.
   
---------------------------------------------------------------------------
Note
	
	GSMStatus_Rsc.h is automatically generated from GSMStatus_Rsc.rcp using Pilrc command. 

	For Gnome, this applicaton has been updated with the new API PhnLibGetGSMCellInfo() to get
        Cell ID/COO and LAC. This is a GSM specific API.

---------------------------------------------------------------------------