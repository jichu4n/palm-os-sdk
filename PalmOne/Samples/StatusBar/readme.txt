Name

StatusBar

-----------------------------------------------------------------------------------------------------------------------
Description

This sample code shows how to detect the Slider position on Tungsten T3.
This sample code also shows how to detect the screen rotation on Tungsten T3 and Tungsten T5.

Features:

- Detect the Slider position (Tungsten T3 Only).
- Detect the Screen Rotation (Tungsten T3 and Tungsten T5).

WARNING: This sample code uses an ARMlet to determine the Slider Position.
This is not officially supported by Palm. Please use this at your own risk.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project can be built using CodeWarrior 9 and
GCC command line (Please use GCC_makefile to build).


------------------------------------------------------------------------------------------------------------------------
Devices

Detecting slider position should run on Tungsten T3
Detecting screen rotation should run on Tungsten T3 and Tungsten T5

Should not crash on other devices
Please see Note below

------------------------------------------------------------------------------------------------------------------------

Requirements

This sample code uses an ARMlet, ARMlet.r, to determine the Slider Position. the ARMlet is provided 
with the sample app

-----------------------------------------------------------------------------------------------------------------------
APIs Used

Data Manager and PACE calls

------------------------------------------------------------------------------------------------------------------------

Note

When this sample app is launched on a Treo device or a PDA (LifeDrive, Tungsten E2) and the "Get Status" button is hit, the following should be 
seen:

The Slider status should say "No slider"
The Status Bar status should say "This device does not rotate" or "0"

----------------------------------------------------------------------------------------------------------------------

