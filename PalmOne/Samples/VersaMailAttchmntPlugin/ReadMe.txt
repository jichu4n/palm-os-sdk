VMTextPlugin is a Versamail attachment plugin to view text plugins. You see it in action, if you add an 
attachment, in the drop down list you can see Text(.txt) in the drop down list. Similarly when you want 
to view a .txt attachment you will see our VMTextPlugin in the Viewer drop down list.

Name

VersaMailAttchmntPlugin  (VMTextPlugin)

-----------------------------------------------------------------------------------------------------------------------
Description

VMTextPlugin is a Versamail attachment plugin for text plugins. While sending an email, if you try to 
add an attachment, you can see Text(.txt) as an option in the drop down list. This plugin can also be used to 
view a .txt attachment that comes with a received email. 

What the plugin does is to allow Versamail to _recognize_ files that you want to attach to an email, or that
you have received, not _opening_ an attachment. Docs To Go is used to open an attachment.

Please refer to the "How to Run" section for details on how to see the plug in in action.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project also builds with Codewarrior 9 and
GCC command line (Please use GCC_makefile to build).

This wizard-generated code is based on code adapted from the
stationery files distributed as part of the Palm OS SDK 4.0.

------------------------------------------------------------------------------------------------------------------------
Devices

Should run on all Palm OS smartphone devices
Should not crash on other devices

------------------------------------------------------------------------------------------------------------------------

Requirements

The plug in has to be hotsynced over to the device. Can be seen on the device using Filez.

-----------------------------------------------------------------------------------------------------------------------
Libraries Used

VersaMail

------------------------------------------------------------------------------------------------------------------------
How to Run

1. Hotsync the VMTextPlugin over to the device. Can be seen on the device using Filez.

2. Create a few memos on the device.

3. Compose a new email and try to attach a file

4. Tap on 'OK' button to clear alert

5. Tap on the drop-down list next to 'Type' in the Attachments window

6. Tap on the '.txt' item in the drop-down list.

7. Clear the alert.



A. Send an email with a word attachment (.doc) to the device 

B. Receive the email on the device and open the attachment

C. Clear the alert.



Expected Results:

3. You get an alert telling you that VMTextPlugin is called.

5. You'll see in the drop-down list that a '.txt' item is added.

6. You see the alert again.

7. You see a list of all the memos in Memos DB 



B. You get an alert telling you that VMTextPlugin is called.

C. Docs To Go is used to open the attachment. 




-----------------------------------------------------------------------------------------------------------------------

Note

----------------------------------------------------------------------------------------------------------------------

