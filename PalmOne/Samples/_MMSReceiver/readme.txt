---------------------------------------------------------------------------
Name

	MMSReceiver


---------------------------------------------------------------------------
Description


The MMSReceiver application is a sample application that shows how to use the palmOne's
Helper API.  This application shows how to register, enumerate, validate, and execute
the system's helper notifications for MMS.

To use the palmOne's Helper architecture, an application will register with the system for
the service class(es) that the application wish to accept and respond to.  A Helper "sending" 
application can send the service request directly to this Helper receiver or send indirectly 
to the system which will be forwarded to the application selected in the Default App Preference
Panel.

---------------------------------------------------------------------------
Builds With

	CodeWarrior9
	GCC command line (Please use GCC_makefile to build)

---------------------------------------------------------------------------
Devices

	Treo devices

---------------------------------------------------------------------------
Requirements
	
	
---------------------------------------------------------------------------
Libraries Used

	SMS/MMS Library

---------------------------------------------------------------------------
How to Run

To run this sample code, you need to install both MMSSender and MMSReceiver. If you copy MMSReceiver 
from your SD card, once you do that, the MMSReceiver will register for MMS helper event notification.You 
do not need to launch MMSReceiver to regsiter notification.

If you install MMSReceiver from your computer, launch it on your device and tap on "Enable". MMSReceiver
will register for Helper notification. 

Once you enable MMSReceiver, go and launch MMSSender. Compose a MMS and click on Send. MMSReceiver should
be able to catch the MMS sending helper notification and you should see a message poping up saying 
"MMS Receiver: Action: Send MMS [Who]: MMS Sender To:xxxx".

If you disable MMSReceiver and then send a MMS from MMSSender, you should not see the same message. Instead,
you will see something like "Notification Not Handled". It is a known issue and the reason of that is because
Messaging app is not responding to the helper notification. 
