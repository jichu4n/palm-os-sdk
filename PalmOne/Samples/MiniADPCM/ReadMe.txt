---------------------------------------------------------------------------
Name

  MiniADPCM

---------------------------------------------------------------------------
Description

  This sample application show how to encode and decode ADPCM samples 
  using the CodecPlugin Manager 3.0. If you click record the recorded file 
  is saved as ADPCMRecord.wav. For more information about the Codec Plugin 
  Manager see the SDK documentation folder. 

---------------------------------------------------------------------------
Builds With

  CodeWarrior
  GCC command line (Please use GCC_makefile to build)


---------------------------------------------------------------------------
Devices

  All Palm devices

---------------------------------------------------------------------------
Requirements

  The ringer should be in the 'ON' position

---------------------------------------------------------------------------
Libraries Used

  Codec Plugin Manager

---------------------------------------------------------------------------
How to Run

  1. Launch the application
  2. Tap 'Record' and talk into the device
  3. Tap 'Stop'
  4. Tap 'Play'
     - Application should play recorded audio

---------------------------------------------------------------------------
Note
 
  Zire72 has a ADPCM encoder and Decoder.

  The device will not auto power off while the application is running.

  There is a known problem with the Record function not working properly. The
  device is left in a non-responsive state while recording data to an SD card.
  The sample app will be updated after this issue has been fixed. 

---------------------------------------------------------------------------
