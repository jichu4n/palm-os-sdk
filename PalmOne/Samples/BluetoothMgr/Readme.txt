---------------------------------------------------------------------------
Name

   BluetoothMgr

---------------------------------------------------------------------------
Description

   This application demonstrates how to use the Bluetooth Library including
   the Bluetooth Manager APIs and Socket and Service Discovery APIs. It
   establishes an ACL connection between two Bluetooth enabled devices and
   allows querying for the name of the connected device and sending a 
   string from one device to the other.

---------------------------------------------------------------------------
Builds With

   PODS
   Codewarrior 9
   GCC command line (Please use GCC_makefile to build)


---------------------------------------------------------------------------
Devices

   Treo smartphones

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

   Bluetooth Manager APIs

---------------------------------------------------------------------------
How to Run

  1. Launch the application on two devices
  2. Tap on 'Connect' in device 1. Select device 2 from the list of
     Bluetooth enabled devices and tap on 'OK'. The log field of device 1 
     will indicate that outbound connection was made
  3. Tap on 'Get Device Name' in device 1 and the name of device 2 will
     appear in the log field
  4. In device 2, tap on 'Receive' and the log field will indicate that 
     socket is listening and that an sdp service record was created
  5. In device 1, tap on 'Send'. The log field of device 2 will print data 
     received on the socket ("This is the sent string"). The log field of  
     device 1 will say that data was sent and socket closed.
  6. Tap on 'Disconnect' in device 1

---------------------------------------------------------------------------
Note


---------------------------------------------------------------------------