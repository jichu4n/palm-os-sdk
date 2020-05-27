---------------------------------------------------------------------------
Name

  MiniGIF

---------------------------------------------------------------------------
Description

  The MiniGIF sample application displays GIF images.

  GIF decoder decodes GIF89a images to RGB565, BGR565 or a GIFFRame 
  ( actual frame with 8bit color and a color table )

  The decoder only supports 8-bit frame output with color table. In order 
  to include the frame descriptor in output, a special parameter type 
  PalmCodecGIFFrameParamType is defined. 

  There is no frame resampling for GIF89a. No matter what width/height is 
  passed in with PalmImageParamType, it is ignored (each frame has its 
  own imageWidth and imageHeight).

  Passing in (0, 0) for output size will make the codec adjust the 
  dimension to the global frame size described in GIF header.

  The Decoder supports frame-by-frame decoding for GIF89a so the app can 
  show the animation. Each EncodeDecode() call outputs exactly one frame. 
  This implies that the output buffer must be large enough to contain one 
  frame (otherwise there will be an error).

  The rest of the details are similar to other Image codecs. Please check 
  MiniImage sample and readme file.

---------------------------------------------------------------------------
Builds With

  CodeWarrior
  Gcc

---------------------------------------------------------------------------
Devices

  All Palm devices

---------------------------------------------------------------------------
Requirements

  Before running the sample code, you need to save a Gif file (you can 
  find a Gif file called banana.gif in the folder) on an SD card and 
  insert it into your device.

---------------------------------------------------------------------------
Libraries Used

  Codec Plugin Manager

---------------------------------------------------------------------------
How to Run

  1. Launch the application
  2. Tap "file" and select a .gif file from the SD card (in DCIM folder)
  3. Tap 'display' to display selected image file

---------------------------------------------------------------------------
Note

---------------------------------------------------------------------------


