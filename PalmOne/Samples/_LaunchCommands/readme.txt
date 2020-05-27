Name

_LaunchCommands

-----------------------------------------------------------------------------------------------------------------------
Description

This sample code shows how to use various Phone app launch commands to dial or show keypad. 
It also shows how to launch the Web browser, including in minimal mode.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project was built using Palm OS Developer Suite version 1.2.0.23.
This project also buids with Codewarrior 9 and 
GCC command line (Please use GCC_makefile to build).


------------------------------------------------------------------------------------------------------------------------
Devices

Should run on all Palm OS devices
Should not crash on other devices

------------------------------------------------------------------------------------------------------------------------

Requirements

No specific requirements

-----------------------------------------------------------------------------------------------------------------------
Libraries Used

Telephony library

------------------------------------------------------------------------------------------------------------------------
How to Run

1. Launch the "_LaunchCommands" application before each of the steps below.
2. Tap on 'Call Log'
3. Tap on 'Contacts'   
4. Tap on 'Favorites'
5. Input 'www.google.com' in the text field and tap on 'Min BLazer'
6. Input 'www.google.com' in the Text field and tap on 'Max Blazer'
7. Input some numbers in text field and tap on 'dial'
8. Tap on 'DialPad Empty'
9. Input some numbers in the text field and tap 'DialPad Filled' 
   (Tapping 'DialPad Filled' before entering numbers should do nothing.)

 

Expected Results:

2. The device should launch Call Log application
3. The device should launch Contacts application in 'Edit' view
4. The device should launch Favorites application.
5. a. The device should launch the web browser in minimal view 
   (minimal view: no windows, only some buttons at the bottom of the screen)
   b. The browser should display the Google website
   c. Tapping 'Min Blazer' before entering 'www.google.com' should
      display browser homepage
6. a. The device should launch the web browser in full view 
   (full view: browser with windows, and some buttons on top of the screen)
   b. The browser should display the Google website
   c. Tapping 'Max Blazer' before entering 'www.google.com' should
   display browser homepage (each device will be different).
7. The device should dial the numbers
8. The device should launch the phone application in DialPad view
9. a. The device should launch the phone application in DialPad view
   b. The dialpad should already have some numbers prefilled 
   (this is not implemented on the Treo 680 and Centro, GSM)

-----------------------------------------------------------------------------------------------------------------------

Note

----------------------------------------------------------------------------------------------------------------------

