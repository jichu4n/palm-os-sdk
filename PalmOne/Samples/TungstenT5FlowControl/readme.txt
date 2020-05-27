Name

Tungsten T5 Flow Control Sample Code

-----------------------------------------------------------------------------------------------------------------------
Description
 

This sample code shows how to disable/enable flow control on Tungsten T5.
This helps built in applications, like Network Preferences, that were built assuming
the availability of flow control pins to work on Tungsten T5, which does not have
flow control pins. 

This sample code is a patch to Tungsten T5 but not a standalone application.
To make it work, deploy the patch to disable flow control
before using the application.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project was built using CodeWarrior 9 and GCC.

-------------------------------------------------------------------------------------------------------
Note:

WARNING: This sample code uses an ARMlet to disable/enable flow control.
This is not officially supported by Palm. Please use this at your own risk.

Features:

- Disable Flow Control (On Tungsten T5).
- Enable Flow Control (On Tungsten T5).

Files:

- TungstenT5FlowControl: Shows how to enable flow control on Tungsten T5.
                         Shows how to disable flow control on Tungsten T5.
			
