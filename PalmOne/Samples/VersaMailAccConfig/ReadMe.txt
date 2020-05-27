Name

VersaMailAccConfig

-----------------------------------------------------------------------------------------------------------------------
Description

If you need to configure mail accounts using the __MMDevice database, you can use this app to generate it.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project was built using Palm OS Developer Suite version 1.2.0.23.
This project also builds with Codewarrior 9 and
GCC command line (Please use GCC_makefile to build).

This wizard-generated code is based on code adapted from the
stationery files distributed as part of the Palm OS SDK 4.0.

------------------------------------------------------------------------------------------------------------------------
Devices

Should run on all Palm OS smartphone devices with Versamail 3.x
Should not crash on other devices

This sample app does not run on Centro devices with Versamail 4.0

------------------------------------------------------------------------------------------------------------------------

Requirements

Palm OS Smartphone devices with Versamail 3.x
No other specific requirements

-----------------------------------------------------------------------------------------------------------------------
Libraries Used

VersaMail

------------------------------------------------------------------------------------------------------------------------
How to Run -- please read the Note section below before starting

1. Launch the "VMAccConfig" application

2. Enter information into fields on all forms

3. Save

4. Launch Versamail on the device

5. Tap 'next' a few times and complete creating the account.

6. Tap on the "Get mail" button.

7. Compose a new message and send it.


Expected Results:

1. If the app is being launched for the first time, all fields should be blank.

2. If the "Title" field is left blank, the app should show an alert.

3. The information should be saved. On launching the app again, the information entered earlier 
   should be filled in. (These values are retained until the account in Versamail has
   been set up)

4. The new account configured with the sample app will show up. Versamail displays an error message 
   if the values entered are incorrect.
   Note: Different accounts take different values to configure the account. Please make sure you are
   using the correct values to configure your account.

6. If the account has been configured properly, you will get all your email from the account you have 
   set up, for example, gmail account. If not, Versamail will display an error message.

7. The message will be sent.


-----------------------------------------------------------------------------------------------------------------------

Note

To configure an account properly using this sample app, you will have to obtain correct values for 
the type of account being created. Launch the Versamail app on your device and begin account setup 
for the type of account you want to setup (for example, gmail account). Versamail fills in the
default values for all the network related fields like incoming/outgoing servers, port numbers, etc. 
You can use these settings to fill in the fields in the sample app. 


Note
This sample app does not run on Centro devices with Versamail 4.0


----------------------------------------------------------------------------------------------------------------------
