Name

_HelperSender

-----------------------------------------------------------------------------------------------------------------------
Description

The HelperSender application is a sample application that shows how to use the palmOne's
Helper API.  The applications shows how to broadcast helper notifications for SMS, Email, 
and WEB to "receiving" applications.  The application that is selected by the user in the 
Default App Preference Panel will receive the service class request send by this HelperSender
application.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project builds with Codewarrior 9 and 
GCC command line (Please use GCC_makefile to build).

This wizard-generated code is based on code adapted from the
stationery files distributed as part of the Palm OS SDK 4.0.

------------------------------------------------------------------------------------------------------------------------
Devices

Should run on all Palm OS devices
Should not crash on other devices

------------------------------------------------------------------------------------------------------------------------

Requirements

No specific requirements

-----------------------------------------------------------------------------------------------------------------------
Libraries Used

Helper APIs

------------------------------------------------------------------------------------------------------------------------
How to Run

HelperSender for SMS
--------------------

1. Launch the "HelperSender" application
2. Tap on "SMS"
3. Tap into and enter the appropriate data for the "To" ,and "Message" fields
4. Tap on "Send" button
   - Check Results

Expected Results:

4. a. You will see SMS application open and you will see a new SMS window with your information 
entered.  The "To" field will be filled with "Helper Sender;".  The body of the SMS will be the 
message you entered.
   b. If you send this SMS to a device this device will receive this SMS.


HelperSender for Email
----------------------

1. Launch the "Helper Sender" application
2. Tap on "Email"
3. Tap into and enter the appropriate data for the "To", "CC", "Subject", and "Message" fields
4. Tap on "Send" at the bottom
   - Check Results

Expected Results:

4. a. If your device's default email application is not already set up, then its setup screen will appear.
   b. If your device's default email application is already setup, then that application should be launched
and all of the appropriate fields entered above should be copied into a blank email


HelperSender for Web
--------------------

1. Launch the "Helper Sender" application
2. Tap on "Web" button
3. Tap on the "URL:" field and enter an URL.
4. Tap on "Go" button.
   - Check Results

Expected Results:

4.If your device's has data turned on you will see Blazer launch and you will see the URL you entered.
   

-----------------------------------------------------------------------------------------------------------------------

Note

----------------------------------------------------------------------------------------------------------------------

       

