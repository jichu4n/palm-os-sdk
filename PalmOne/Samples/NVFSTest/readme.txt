---------------------------------------------------------------------------
Name

  NVFSTest

---------------------------------------------------------------------------
Description

  This sample file shows how to commit database changes permanently to 
  NAND (Flash) drive.

  'Write to DBCache' button writes the field changes to the app database, 
  and 'sync to flash' button commits the changes to NAND (Flash) drive 
  without closing the database.

  Changes that are already committed to the NAND (Flash) drive should be 
  seen even if the device loses power or resets when the application is 
  running.

---------------------------------------------------------------------------
Builds With

  CodeWarrior
  Gcc

---------------------------------------------------------------------------
Devices

  All Palm devices with NVFS

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used


---------------------------------------------------------------------------
How to Run

  1. Launch the application
  2. Enter some text in the text field
  3. Tap on 'Write to DB' then 'Sync to Flash'
  4. Remove the battery and relaunch the application after the device boots   
   - The string entered should appear in the text field
  5. Enter new text in the text field and tap on 'Write to DB'
  6. Remove the battery and relaunch the application after the device boots   
   - The first string entered should appear in the text field

---------------------------------------------------------------------------
Note


---------------------------------------------------------------------------
