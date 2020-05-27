---------------------------------------------------------------------------
Name

  BtClientServer

---------------------------------------------------------------------------
Description

  This application allows for setting up a device as a client or server
  and for the client to send files over Bluetooth to the server device.

---------------------------------------------------------------------------
Builds With

  PODS
  CW

---------------------------------------------------------------------------
Devices

  Treo 700p, Treo 755p smartphones  

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

  Serial Manager APIs

---------------------------------------------------------------------------
How to Run

  1. Launch the application on a device and press the Server button. This
     sets the device up as a server
  2. Launch the application on a second device and press the Client button. 
     This sets the device up as a client. When the Discovery form appears, 
     select the server device and press OK.
  3. On the client device, press Client browse and select a .png, .bmp 
     or .jpg file to send to the server device over Bluetooth.  
  4. On the server device, press Server receive.
  5. On the client device, press Client send. 
  6. The file should be transferred to the server device.

  At any time, if an error message appears in the text field or to reset
  the client or server, press the Reset client/server button. Then set
  the device up again as a client or server.

---------------------------------------------------------------------------
Note

  Supported test file formats are png, bmp and jpg. The max file size 
  supported is 2K

---------------------------------------------------------------------------