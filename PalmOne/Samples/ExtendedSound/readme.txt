---------------------------------------------------------------------------
Name

ExtendedSound

---------------------------------------------------------------------------
Description

This sample code shows how to use the extended sound manager API.
The application 'MyPlayer' is a recorder/player for WAVE files.
Currently only PCM and IMA_ADPCM 4-bit WAVE files are supported.
This application was built using CodeWarrior 9 but there should be
no issues switching to CodeWarrior 8.

Features:

- Recording.
- Playback.
- Write WAVE files to Memory or Card.
- Select recording options.
- Fully working application.
- Use different sample rate for playback (it's fun!).

Application Structure:

- The entry point of the program is MyPlayer.c.
- There are 2 forms: RecordForm and PlayForm.
- Each have there own event loops and menus.
- Each form has a mini state machine for play or record/pause/idle.

Files:

- Common : Common functions for PalmOS
- MyPlayer : Main application entry point
- PlayForm : Player form
- RecordForm : Record form
- Wave : Basic functionality to read/write WAVE headers to a file.

---------------------------------------------------------------------------
Builds With

CodeWarrior
Gcc

---------------------------------------------------------------------------
Devices

All Palm devices

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

Extended Sound Manager

---------------------------------------------------------------------------
How to Run

Make sure that the ringer is in the 'ON' position and insert an SD card
into the device

1. Launch the 'My Player' or 'Extended Sound' application
2. Change the filename to something else (optional)
3. Tap on 'record' button to begin recording
4. Tap on 'stop' button to stop recording
5. Tap on 'play' button to start playing the file that was just recorded

---------------------------------------------------------------------------
Note

When selecting the IMA_ADPCM codec, the recommended sample rate is
up to 22050 Hz. If set higher, the result may be poor audio quality
during playback.

There needs to be an SD card in the device when running this application.

---------------------------------------------------------------------------
