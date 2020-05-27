---------------------------------------------------------------------------
Name

   PalmPhotoCapturePreview

---------------------------------------------------------------------------
Description

   This sample code shows how to use the Palm Photo Library to use the Camera
   interface to capture a photo, preview the photo captured, and save it to
   the device's photo album.   

---------------------------------------------------------------------------
Builds With

   CodeWarrior
   Palm OS Developer Suite
   gcc

---------------------------------------------------------------------------
Devices

   Devices with the Palm Photo Library (please consult the Developer Guide
   for that list)


---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

   Palm Photo Library

---------------------------------------------------------------------------
How to Run

   1. Launch the PalmPhotoCapturePreview application
   2. Select the "Launch Camera" button, which should launch the camera 
   3. Aim and press the select button on the 5 way navigation to capture picture
   4. Picture will be presented to be saved or deleted
   5. Select to save the picture
   6. You will be taken back to the Sample app
   7. When you are finished, go to the pictures on your device, and notice
      that the picture that you took is now there as well.


---------------------------------------------------------------------------
Note

   The picture is taken in JPEG format, and first placed into a file stream.
   It is then added to the device's photo album by using the PalmPhotoAddImageV2
   API.


---------------------------------------------------------------------------