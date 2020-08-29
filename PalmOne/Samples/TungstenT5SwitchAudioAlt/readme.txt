Name

Tungsten T5 Switch Audio Alternative Sample Code

-----------------------------------------------------------------------------------------------------------------------
Description

This sample code shows how to switch audio from Speaker to Multi-conenctor and from Multi-conenctor
to Speaker on Tungsten T5.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project can be built using CodeWarrior 9 and
GCC command line (Please use GCC_makefile to build).


------------------------------------------------------------------------------------------------------------------------
Note:

WARNING: This sample code uses an ARMlet to Switch Audio.
This is not officially supported by palmOne. Please use this at your own risk.

WARNING: We have seen that it takes about 4 secs for the audio to be switched back to the Speaker
(if the switch audio from multi-connector to speaker was invoked) once the
application quits and returns to the launcher.

Features:

- Switch Audio from Speaker to Multi-connector (On Tungsten T5).
- Switch Audio from Multi-connector to Speaker (On Tungsten T5).

Files:

- TungstenT5SwitchAudio: Shows how to switch audio from Speaker to Multi-connector on Tungsten T5.
                         Shows how to switch audio from Multi-connector to Speaker on Tungsten T5.

BUG FIXED:
The Tungsten T5 Switch Audio Sample code switches the Audio to the multi-connector but the audio
gets switched back to internal when a user quits the application. The audio should stay
switched to the multi-connector. The Tungsten T5 Switch Audio Alternative Sample code fixes that
issue. Now the audio stays switched to the multi-connector even if the user switches from the
application.			
