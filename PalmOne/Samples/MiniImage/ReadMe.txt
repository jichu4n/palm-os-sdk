---------------------------------------------------------------------------
Name

   MiniImage

---------------------------------------------------------------------------
Description

   MiniImage shows how to use the image codecs and encode and decode 
   between various file formats. (GIF codecs are in a separate 
   sample). 

   Note that the sample is designed to work in Decode/Encode pairs. For 
   example you can choose a file to decode and save it as a .raw file and 
   click on the encode button and encode the .raw file back to jpeg or bmp. 
   This is because lots of information from the decode session is carried 
   over to the encoded session like the width and height of the image 
   which is not known by looking at the raw data. So if you click encode 
   without first decoding, it may not work properly. The Display button 
   decodes the image to BGR565. For all other decode formats, click decode 
   after selecting your color format. This saves the image as a .raw file.
   The files need to be located on an SD Card.

   General Steps for decoding Images:

   1.  CodecMgrCreateSession()
   You can pass in the imageParamP width and height as needed, for example if 
   you need a 240x240 of a 320x320 image, you can pass it in, else the size 
   of the image is assumed. The rowbytes are also automatically calculated and 
   need not be passed in. If the CodecMgrCreateSession() succeeds, the 
   imageParamP will now contain the width and height and rowByte of the image 
   that's going to be used by the decoding session. Note that enlarging of an
   image is not possible, for example if you pass 320x320 as input to 
   CreateSession() and the image is 240x240, it still remains 240x240.

   2. CodecMgrEncodeDecode()
   One or multiple calls to this function. Note if you allocate a big enough 
   buffer, this call is executed only once. The buffer size is usually
   rowByte * height but this may take up too much dynamic heap space, so it is 
   advisable to use small buffers and loop like it is done in the sample. 
   To allocate large chunks use MemGluePtrNew(). 

   3. CodecMgrDeleteSession()
   Make sure you delete the session.


   General Steps for encoding images:

   1. CodecMgrCreateSession()
   Both BMP and JPEG take different output parameters. The imageParamP should be 
   known beforehand since you're encoding raw data.

   2. CodecMgrEncodeDecode()
   The output is a file which you have already opened.

   3. CodecMgrDeleteSession() 
   Make sure you delete a session. 

   Note that for Index Images, you need to fill in a separate structure that 
   needs to be passed to CodecMgrEncodeDecode. Please check the example.

   And also note that the bitmap data for the Palm has a coordinate system that 
   is inverted when compared to windows bitmaps, so don't be surprised if your 
   encoded bitmap looks inverted on Windows. 



---------------------------------------------------------------------------
Builds With

   CodeWarrior
   Palm OS Developer Suite
   gcc

---------------------------------------------------------------------------
Devices

   All Palm devices

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

   Codec Plugin Manager

---------------------------------------------------------------------------
How to Run

  1. Launch the MiniImage application
  2. Make sure the 'decode' button is selected
  3. Make sure SD card is inserted, and go to the directory where the image
     files are, and select an image file (.jpg)
  4. Select 'display' to display picture
  5. Select the Color Format
  6. Select 'decode' to decode the picture to a raw format

  7. Tap on the 'encode' button on top of the screen
  8. Tap on the first blue folder to open an input file from SD card [the
     raw file from step 6 can be used here]
  9. After a file is selected and the main screen is displayed, select file
     format to be JPEG. Also select the the Color Format
  10. Tap on 'encode' to encode the raw image file back to JPEG

---------------------------------------------------------------------------
Note

   For formats like GRAY8, Index8, Index4, Index1 the input has to be the 
   same format. For example the input bmp should be Index8 to get a Index8 
   decoded data. You will get a "No Codec found" if you try to pass a 24bit 
   bmp and try to decode it to Index8. 


---------------------------------------------------------------------------

