---------------------------------------------------------------------------
Name

  FotoFinish

---------------------------------------------------------------------------
Description

  This sample code is a simple photo editor application. It suppports
  editing of existing JPEG pictures and creating of new ones. I allows for
  organizing albums and pictures present in albums and for editing photo-
  properties such as filename and notes associated with each picture. It 
  also supports multiple undo/redo on draw operations. 

  Files:

  - FotoFinish : Main application. Does all the work.
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
  Palm OS Developer Suite 
  gcc

---------------------------------------------------------------------------
Devices

  Treo smartphones
  LifeDrive

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

  Photo Library

---------------------------------------------------------------------------
How to Run

  1. Launch the FotoFinish application
  2. Tap on the menu and choose 'Albums'. Create, rename or delete albums
  3. Draw lines/curves/text/time on the canvas. Undo/redo operations by
     using the 'Edit' menu
  4. Tap on the menu and choose 'save as'
  5. Input filename (and optionally image category)
  6. Tap on the menu and choose 'Photo Details'. Select the album that 
     contains the newly created picture. 
     Locate the picture in the album by turning the album pages. 
  7. Tap on 'Edit' and enter text in the 'Notes' field. Tap on 'Save'.
  8. Launch the 'Media' application and see that the saved image is
     visible there.

---------------------------------------------------------------------------
Note

  This application does not support saving images or albums to the SD card

---------------------------------------------------------------------------

