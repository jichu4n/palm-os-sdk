---------------------------------------------------------------------------
Name

  SerialCommunication

---------------------------------------------------------------------------
Description

  This sample code shows how to use the Serial Manager to transmit and 
  receive Serial Data.

  Features:
  - Transmit data using the Serial Port.
  - Receive data from the Serial Port.
  - Detect Serial Peripheral (Uses Workaround for Tungsten T5).
  - Power On the POWER_OUT pin (Tungsten T5 Only).
  - Power Off the POWER_OUT pin (Tungsten T5 Only).
  - Workaround for the flow control problem on Tungsten T3.
  - Added support for the class notifications

  Files:
  - SerialTransRecv	: Shows how to transmit and receive data using the serial port.
                          Shows how to detect a serial peripheral on Tungsten T5.
		          Shows how to power on/off the POWER_OUT pin on Tungsten T5.
			  Shows how to workaround the flow control problem on Tungsten T3.
			  Shows how to detect class Notifications.

---------------------------------------------------------------------------
Builds With

  CodeWarrior 9
  Gcc

---------------------------------------------------------------------------
Devices

  All Palm devices

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

  Serial Manager

---------------------------------------------------------------------------
How to Run

Please see Note below for running on Centro devices.

Serial cable detection
1. Launch Serial TransRecv
2. Tap on 'En Det'
3. Attach serial cable
   - Several alerts will appear signaling that cable was attached
4. Detach serial cable
   - Several alerts will appear signaling that cable was detached
5. Tap on 'Dis Det' and attach serial cable
   - No notifications will be received as class detection is disabled

Serial communication with PC
1. Launch Serial TransRecv
2. Attach serial cable between device and PC
3. On PC, run program such as HyperTerminal that will allow serial 
   communication with device
4. Enter compatible settings in HyperTerminal and Serial TransRecv 
5. Open serial port on device by tapping on the Ser Open button. 
6. Tap on Send button and enter data in Enter Data field to send data to 
   HyperTerminal. Tap on Receive and enter data in HyperTerminal to receive
   data in Serial TransRecv. Close and reopen the port between each 
   operation. 

---------------------------------------------------------------------------
Note

  WARNING: This sample code uses an ARMlet to workaround the flow control 
  problem on Tungsten T3. This is not officially supported by Palm. Please 
  use this at your own risk.

When running this app on a Centro device, please enter data in the HyperTerminal
first, before hitting the Receive button on the device.

---------------------------------------------------------------------------





