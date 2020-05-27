Name

  PhoneEventReceiver (Application name on device: PhnEvt)

---------------------------------------------------------------------------
Description

This sample application shows how to register for Phone Events PhnEvtStartIncomingCall
to get phone event notifications from the phone library.  This application
implements a sound alarm when phnEvtStartIncomingCall is received.  A real
application should use this notification to make any pre-call initialization
or finalization of their application. Any processing on this notification should be very quick 
(less than a few hundred miliseconds)

  Files:

  - PhoneEventReceiver.c : Main application. Does all the work.

  Major functions used:

  - HsGetPHoneLibrary (): Load the phone library if not loaded, open and get the reference number to the library
  - HsStatusUpdateGadget (): Explicitly update all status gadgets on the current form.
  - PhnLibRegister (): Register an application so that notifications from the Phone Library can be broadcasted to registered applications
  - SndPlaySystemSound (): Play system alart sound when receieing the incoming call notification, in PilotMain Routine.

---------------------------------------------------------------------------
Builds With

  CodeWarrior 9 
  GCC command line (Please use GCC_makefile to build)

---------------------------------------------------------------------------
Devices

  Treo 650

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

  Photo Library

---------------------------------------------------------------------------
How to Run

  1. Launch the PhnEvt application
  2. Turn one the phone volume
  3. Use another phone to call the phone you are running the application
  4. The phone should sound an alarm prior to having the phone ring as usual

---------------------------------------------------------------------------
Note

Once you run the application on your Treo device, the alarm sound will be played everytime when you have an incoming call. 
To remove the alarm sound, simply to go PhoneEventReceiver.c commenting out "SndPlaySystemSound(sndAlarm);", compile the code,
download the new PRC file, and run it on your Treo. After that, the alarm sound will not be palyed when having an incoming call.