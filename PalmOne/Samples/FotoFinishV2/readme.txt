---------------------------------------------------------------------------
Name

  FotoFinishV2

---------------------------------------------------------------------------
Description

  This sample code shows how to use the Photo Library APIs.

  Features:
  - Simple Photo Editor Application.
  - Has support for editing existing JPEG pictures as well as creating new ones.
  - Has support for organizing albums and pictures present in albums.
  - Has support to edit photo-properties such as file-name and notes associated with each picture.
  - Has support for multiple undo/redo, so that draw operations can be un-done and re-done.
  - Version 2 API to rotate picture
  - Version 2 API to Crop and Scale pics

  Files:
  - FotoFinishV2 : Main application. Does all the work.
  - MainForm: Contains all the event-handling code for MainForm.
  - DetailsForm: Contains all the event-handling code for DetailsForm.
  - AlbumsOrganiseForm: Contains all the event-handling code for AlbumsOrganiseForm.
  - SavePhotoForm: Contains all the event-handling code for SavePhotoForm.
  - AddTextForm: Contains all the event-handling code for AddTextForm.
  - SaveUtils: Contains utility functions for saving a photo as a jpeg in an album.
  - Utils: Contains utility functions which are used throughout the project.

---------------------------------------------------------------------------
Builds With

  CodeWarrior
  GCC command line (Please use GCC_makefile to build)


---------------------------------------------------------------------------
Devices

  All Palm devices

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

  Photo Library

---------------------------------------------------------------------------
How to Run

  1. Launch the application
  2. Tap on menu and choose 'open'
  3. Select an image file from the SD card
  4. Draw lines/curves/text/time on the canvas
  5. Tap on the menu and choose 'save as'
  6. Input filename (and image category (optional))
  7. Launch the 'Media' application to see the saved image

---------------------------------------------------------------------------
Note

  The number of draw operations supported in the application is 50. This 
  value is hardcoded in the MAX_OP macro.

---------------------------------------------------------------------------



