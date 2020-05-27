Name

_HelperReceiver

-----------------------------------------------------------------------------------------------------------------------
Description

The HelperReceiver application is a sample application that shows how to use Palm's
Helper API.  The application shows how to register, enumerate, validate, and execute
the system's helper notifications for SMS, Email, and WEB.

To use Palm's Helper architecture, an application will register with the system for
the service class(es) that the application wishes to accept and respond to.  A Helper "sending" 
application can send the service request directly to this Helper receiver or send indirectly 
to the system which will be forwarded to the application selected in the Default App Preference
Panel.

Please see Note below.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project builds with Codewarrior 9 and
GCC command line (Please use GCC_makefile to build)


------------------------------------------------------------------------------------------------------------------------
Devices

Should run on all Palm OS devices
Should not crash on other devices

------------------------------------------------------------------------------------------------------------------------

Requirements

Needs HelperSender sample app for testing

-----------------------------------------------------------------------------------------------------------------------
Libraries Used

Helper APIs and Helper service class

------------------------------------------------------------------------------------------------------------------------
Setup
------
1. Copy the "Helper Receiver" application to your device.

--- Check Results ---

2. Click on the "Enable" button.

--- Check Results ---

3. Go to "Prefs" in the Launcher.
4. Click on "Default apps".
5. Select "Helper Receiver" for "Email, Messaging, and Browser".
6. Click on the "Done button.



Expected Results:
 
1. If you copy it from the SD card you will see the message "Register Helper Event Successfully YES!"
 
2. You might see the dialog box "Could Not Register "Could not register:App already registered".


How to Run
----------
1. After you setup "Helper Receiver", launch "Helper Sender" sample app.
2. Click on the "SMS" button.
3. Enter a telephone # in the "To:" field and enter something in the Message field.
4. Click on the "Send" button.

--- Check Results ---
5. Click on the "View Message".
6. Click "OK" and "Cancel" button.
7. Click on the "Email" button.
8. Enter in the "To, Subject, and Message" field and click on the "Send" button.

----  Check Results ---

9. Click on the "View Message".
10. Dismiss the dialog boxes.
11. Click on the "Web" button.
12. Enter "www.yahoo.com" or whatever you want and click on the "Go" button.

--- Check Results ---

Expected Results
----------------
4. You will see the "Helper Receiver" dialog box appears.  
You will see "Action:Send SMS, [Who]:Helper Sender, To:phone # you entered.".

5. You will see the Helper Receiver dialog box and "Message: test (whatever you enter in the message
field).

8. You will see the "Helper Receiver" dialog box appears. 
You will see "Action:Send Email, [Who]:Helper Sender, To:(whatever you enter in the To field)".

9. You will see the Helper Receiver dialog box and "Subject: (whatever you entered in the Subject
field. Message: test (whatever you enter in the message field).

12. You will see the Helper Receiver dialog box and "Action:Go To Link. URL: www.yahoo.com (or whatever
you enter in the URL field).



-----------------------------------------------------------------------------------------------------------------------

Note

Since this sample code registers for sysNotifyHelperEvent upon system reset and app launch, run this 
app again after either.If you have run it before in the same device or reset, you will see an alert saying 
cannot register for sysNotifyHelperEvent. You can either ignore the message or Delete this app first and do 
a soft reset. The alert should disappear when running the app after the soft reset.

----------------------------------------------------------------------------------------------------------------------

