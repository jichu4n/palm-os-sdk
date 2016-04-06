Palm OS 5 SDK (68K) R3 - Readme File

IMPORTANT:  Read Before Using the Accompanying Software

= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
  P a l m   O S   S o f t w a r e   D e v e l o p m e n t   K i t
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

IMPORTANT: This Software Development Kit (SDK) is at a "pre-Alpha"
stage. PalmSource makes no guarantees as to the usability of this SDK.

**********************************************************************
* ENHANCEMENTS AND FIXES -- WHAT'S NEW                               *
**********************************************************************

- Palm OS Support Folder

  All header files now include the following version string
  in their header blocks:  Palm OS 5 SDK (68K) R3.

- Palm OS APIs

  "Palm OS Programmer's Companion" and "Palm OS Programmer's API
  Reference" contain new sections documenting additions and changes
  to the Palm OS APIs.

  Graffiti 2 Feature Set: Support for the Graffiti(R) 2 handwriting
  recognition engine.

- PalmRez

  o  Support for QVGA (1.5X) density screens.  This support is added
     for app icon families, bitmap families, and font families.
  o  Support for the "hidden" menu item attribute.

- Constructor for Palm OS 1.9

  Constructor for Palm OS includes the following new features:

  o  Support for Simplified Chinese as a Palm OS target setting and
     as a default text encoding for editing PRC files.
  o  Support for QVGA (1.5X) density screens.  This support is added
     for app icon families, bitmap families, and font families.
  o  Support for adding custom data resources.
  o  Support for the "hidden" menu item attribute.

  For more information, see the "Constructor for Palm OS" book.

- PRC2OVL

  Additional Chinese locale support.

- Virtual Phone 1.7

  Additional functions for testing SIM, Speech, and GPRS functions.

**********************************************************************
* MULTI-SEGMENT APPLICATION SUPPORT                                  *
**********************************************************************

This is a stationery project for a multi-segment application that runs
on Palm OS devices.  Even though you can create applications with
multiple segments, you must follow the example used in this project.
See the "Targeting Palm OS" manual in the CodeWarrior Documentation
folder for more information on working with multi-segment
applications.

Segments are numbered from 0.  Segment 0 contains information for
the segment loader and is created by the linker.  Segment 1 contains
your application code and the startup code.  This segment __must__
contain "MSL Runtime Palm OS (xx).lib", PilotMain(), and any code that
is executed when PilotMain() receives a launch command other than
sysAppLaunchCmdNormalLaunch.  This stipulation is caused by the fact
that A5-relative data is not relocated when the application is called
with any other launch code, thus all global variable addresses will be
wrong and cause a crash.

The best way to determine what files can go into segments other than 1
is to comment out the code in PilotMain() that handles the
sysAppLaunchCmdNormalLaunch command, select the "Link Map" option from
the 68K Linker preferences, and rebuild.  Any file that shows up in
the link map on a line beginning with "Code:" must be in the first
segment.

If you follow the coding style used by the Palm OS examples, form
handlers and the static functions called by the form handlers are good
candidates to be placed in secondary segments.

**********************************************************************
* IMPORTANT INFORMATION REGARDING FUTURE OS COMPATIBILITY            *
**********************************************************************

Starting with the Palm OS SDK 4.0 Update 1, the default compilation
options disallow direct access to Palm OS internal data structures.
The file BuildDefaults.h (which is located within the "Incs" folder
inside the "Palm OS Support" folder) contains the compiler flags
correctly set:

    #define DO_NOT_ALLOW_ACCESS_TO_INTERNALS_OF_STRUCTS

Compile your application as you normally do, and fix the potential
incompatibilities that are identified.

Some samples shipped with older versions of the SDK may have used
direct access of internal structures instead of using APIs. Those
samples will no longer build correctly as a result of this change to
the default compilation options.  To work around this problem, you
can edit the header file buildDefaults.h and comment out the
following line near the end of the file:

//  #define DO_NOT_ALLOW_ACCESS_TO_INTERNALS_OF_STRUCTS

With this change, the old samples will compile properly.  However,
Palm engineering recommends that developers DO NOT directly access
internal data structures.

**********************************************************************
* IMPORTANT INFORMATION REGARDING METROWERKS CODEWARRIOR             *
**********************************************************************

- Palm OS SDK 5 SDK (68K) R3 and CodeWarrior for Palm OS R8

  All of the examples and other Metrowerks project (MCP) files were
  last saved using CodeWarrior for Palm OS R8.

- Checking for updates for Metrowerks components

  Metrowerks occasionally provides updates to some components, such
  as the IDE and other plug-ins. To make sure that your Metrowerks
  components are up to date, please check for updates and patches on
  the Metrowerks web site:

  http://www.metrowerks.com/download/

*********************************************************************
* KNOWN ISSUES                                                      *
*********************************************************************

- Constructor for Palm OS

  No font editor:  Constructor does not provide a native font editor.
  You can use Constructor to create Font Families, but you must first
  create the fonts with an external font editor.


  Windows limitations:

  Chinese double-byte support makes use of the Altura emulation
  library, which does not have support for Simplified Chinese.  In
  general, Constructor still supports Simplified Chinese, but you
  may see some text drawing flickering, garbage drawing results, or
  text editing issues due to double-byte characters not being handled
  correctly.

  Chinese features are not supported on the English version of
  Windows XP with Chinese regional support.  Some third-party
  utilities may solve this problem, such as NJStar Communicator 2.30
  (www.njstar.com).

  Supported CodeWarrior IDE on Windows:

  Constructor requires version 3.1.0.0 of ASINTPPC.DLL, which is
  shipped with the Metrowerks CodeWarrior for Palm OS R-series
  environments.  If you want to use Constructor with a Pro-series
  version of CodeWarrior, you need to copy ASINTPPC.DLL and SHW32.DLL
  (version 4.0.0.0) into the same directory as the "Constructor for
  Palm OS.EXE" file.

*********************************************************************
* INSTALLATION INSTRUCTIONS                                         *
*********************************************************************

+--------------------------------------------------------------------+
+ Manual Installation                                                +
+--------------------------------------------------------------------+

Macintosh

1.  Locate the "CodeWarrior Support" folder which is in the "SDK"
    folder.

2.  Move the items located inside the "Codewarrior plugins" folder in
    the "SDK" folder into their respective folders where CodeWarrior
    is installed on your local drive. (Make sure to use the correct
    version of a tool depending on the OS version you are running when
    both a classic version and a Carbonized versions are present.)

    This will include the following:

    - Move "PalmRez Post Linker" from the "Linkers" folder on the SDK
      to the "Linkers" folder inside the "CodeWarrior Plugins" folder
      where your CodeWarrior is installed on your local drive. This
      will replace the old "PalmRez Post Linker" file.

    - Move "PalmRez Panel" from the "Preference Panels" folder on the
      SDK to the "Preference Panels" folder inside the "CodeWarrior
      Plugins" folder where CodeWarrior is installed on your local
      drive.  This will replace the old "PalmRez Panel" file.

      Note: If you are doing development on a Japanese system, then
      replace the "PalmRez Panel" file with "PalmRez-J Panel" file.
      You can have only one PalmRez Preference Panel on your system
      at a time.

    - Move the "Support" folder from the SDK to the "Codewarrior
      Plugins" folder on your local system. If a "Support" folder
      already exists, then copy only the "PalmRez Japanese Support"
      and "PalmRez ChinSimp Support" files to your existing "Support"
      folder.

    - Merge the "Palm OS Support" folder from the SDK into the
      directory where CodeWarrior is installed on your local drive.

    - Manually copy any other files located on the SDK or downloaded
      from the web to your local drive.


Windows - CodeWarrior

1.  Locate the "CodeWarrior Support" folder which is in the "SDK"
    folder.

2.  Move the items located inside the "Codewarrior plugins" folder on
    the SDK into their respective folders where CodeWarrior is
    installed on your local drive.

    This will include the following:

    - Move the "PalmRezPostLinker.dll" from the "Linkers" folder on
      the SDK to the "Linkers" folder inside the "Bin\Plugins" folder
      where CodeWarrior is installed on your local drive. This will
      replace the old "PalmRezPostLinker.dll" file.

    - Move the "PPalmRez.dll" from the "Preference Panels" folder on
      the SDK to the "Preference Panels" folder inside "Bin\Plugins"
      folder where CodeWarrior is installed on your local drive. This
      will replace the old "PPalmRez.dll" file.

      Note: If you are doing development on a Japanese system replace
      the "PPalmRez.dll" with "PPalmRez-J.dll". You can have only one
      PalmRez Preference Panel in your system at a time.

    - Move the "Support" folder from the SDK to the "Bin\Plugins"
      folder on your local system. If a "Support" folder already
      exists, copy only the "PalmRez Japanese Support" and "PalmRez
      ChinSimp Support" files to your existing "Support" folder.

    - Merge the content of the "Palm OS Support" folder from the SDK
      with the "Palm OS Support" folder contained in your CodeWarrior
      environment.

    - Manually copy any other files located on the SDK or downloaded
      from the web to your local drive.

+--------------------------------------------------------------------+
+ Installation for prc-tools / GCC                                   +
+--------------------------------------------------------------------+

This archive contains the GCC versions of the NetSocket and PalmOSGlue static
linker libraries that were omitted from the initial 5.0 R3 SDK archive.  It is
available in three formats:

- Header files and libraries packaged as an RPM

  This RPM contains the SDK header files and static libraries and is suitable
  for RPM-based Linux distributions and other Unix-like operating systems
  which support RPM.

- A tarball containing header files (with Unix line termination) and libraries

  This tar archive contains the SDK header files and static libraries and is
  suitable for any Unix-like operating system.

- A zipfile containing the libraries

  This zip archive contains the SDK static libraries only; it can be used,
  on Windows or any other operating system, to add GCC SDK libraries to an
  existing installation of the SDK header files.

This archive does not contain the SDK examples or documentation; you can obtain
those from the initial full 5.0 R3 SDK archive.  Windows users who need the SDK
header files will find these too in the initial full 5.0 R3 SDK archive.

The RPM can be installed in the usual RPM way; check the package's information
(rpm -qip) for considerations for users of non-Linux operating systems or
non-standard PalmDev tree locations.

Unpacking the tar or zip archives will create a hierarchy of files underneath
a "sdk-5r3" directory.  The archive, whichever of the two you use, should be
unpacked directly within your PalmDev tree (e.g., /opt/palmdev), so that its
files are arranged underneath /opt/palmdev/sdk-5r3.

If you are using the zip archive to add the libraries alongside an existing
installation of the 5.0 R3 SDK header files, you should unpack the archive so
that the "lib" directory is a sibling of the "Incs" or "include" directory
containing the header files.  (Most likely the header files are in
/PalmDev/sdk-5r3/include or /PalmDev/sdk-5r3/Incs, so this means that the
libraries will be in /PalmDev/sdk-5r3/lib.)

To recap: you need to unpack this archive so that the SDK header files and
libraries are arranged in a directory tree as follows:

    /opt/palmdev/sdk-5r3/include
    /opt/palmdev/sdk-5r3/include/PalmTypes.h ...etc...
    /opt/palmdev/sdk-5r3/include/Core/...etc...
    /opt/palmdev/sdk-5r3/lib
    /opt/palmdev/sdk-5r3/lib/m68k-palmos-coff
    /opt/palmdev/sdk-5r3/lib/m68k-palmos-coff/libPalmOSGlue.a ...etc...
    /opt/palmdev/sdk-5r3/lib/m68k-palmos-coff/mown-gp/...etc...

(On Windows, replace "/opt/palmdev" here by "/PalmDev".  It is also
acceptable for the header files to be under a directory named "Incs" rather
than "include"; this may be more convenient if you have unpacked the initial
Windows 5.0 R3 SDK archive or used the installer.)

Remember to rerun palmdev-prep after installing these libraries!

To verify that the SDK libraries have been detected, check the report
generated when palmdev-prep is run; it should include a line such as

    sdk-5r3  headers in 'include', libraries in 'lib'

rather than a line containing "no libraries".

+--------------------------------------------------------------------+
+ Installation Using Installers                                      +
+--------------------------------------------------------------------+

NOTE:  The installer will replace any older version of Palm OS SDK.

Macintosh

This installer will only install the components needed by CodeWarrior
to your local drive. For all other items you can drag the folder to
the necessary place.

1.  Start the "Palm OS 5 SDK (68K) R3 Installer" application.

2.  Follow the instructions displayed by the installer.

3.  The following items will be installed by this installer:

    - "PalmPlugin" will be added to your
      "Metrowerks CodeWarrior:CodeWarrior Plugins:Debuggers" folder.

    - "PalmRez Post Linker" will be added to your
      "Metrowerks CodeWarrior:CodeWarrior Plugins:Linkers" folder.

    - "PalmRez Panel" will be added to your
      "Metrowerks CodeWarrior:CodeWarrior Plugins:Preference Panels"
      folder.

    - "PalmRez Japanese Support" will be added to your
      "Metrowerks CodeWarrior:CodeWarrior Plugins:Support" folder.

    - "PalmRez ChinSimp Support" will be added to your
      "Metrowerks CodeWarrior:CodeWarrior Plugins:Support" folder.

    - "Constructor for Palm OS" will be added to your "Palm OS
      Constructor" folder and remove the old version.

    - "Palm OS Support" folder in the "Metrowerks CodeWarrior" folder
      will be updated with the latest header files.

    - On English systems, the PalmRez-J panel plug-in will be
      installed to your "Palm OS Extras:Japanese" folder.

    - "Palm OS 5 SDK 68K R3 Readme.txt" will be installed to the
      root of your CodeWarrior installation.

    - The Palm documentation will be added to your "Palm
      Documentation:Palm OS 5 SDK (68K) Docs"
      folder.

    - The Palm examples will be added to your "(CodeWarrior
      Examples):Palm OS 5 SDK (68K) R3 Examples" folder.

    - The Palm Stationery will be added to your "Stationery:Palm OS
      Application" folder. Any existing stationery will be replaced.

    - The Palm debugger will be added to your "Palm Tools:Palm OS 5
      SDK (68K) Other Tools:PalmDebugger" folder.

    - The overlay tools will be added to your "Palm Tools:Palm OS 5
      SDK (68K) Other Tools:Overlay Tools" folder.


Windows - CodeWarrior

This installer will only install the components needed by CodeWarrior
to your local drive. For all other items you can drag the folder to
the necessary place.

1.  Start the "Palm OS 5 SDK (68K) R3.exe" application. This will
    unarchive and start the installer.

2.  Follow the instructions displayed by the installer.

3.  The following items will be installed by this installer:

    - "PalmRezPostLinker.dll" will be added to your
      "bin\plugins\Linker" folder.

    - "PPalmRez.dll" will be added to your "bin\plugins\Preference
      Panel" folder.

    - "PalmRez Japanese Support" will be added to your
      "bin\plugins\Support" folder.

    - "PalmRez ChinSimp Support" will be added to your
      "bin\plugins\Support" folder.

    - "Constructor for Palm OS.exe" will be added to your "Palm
      Tools\Constructor" folder.

    - "Palm OS Support" folder in the "Metrowerks CodeWarrior" folder
      will be updated with the latest header files and libraries. Any
      existing version will be renamed "(old Palm OS Support)"

    - "PPalmRez-J.dll" will be installed to your "Palm Tools\Palm OS 5
      SDK (68K) Other Tools\Palm OS Extras\Japanese Support\
      CodeWarrior Support\Plugins\Preference Panel" folder.

    - "Palm OS 5 SDK 68K R3 Readme.txt" will be installed to the
      root of your CodeWarrior installation.

    - The Palm documentation will be added to your "Palm
      Documentation\Palm OS 5 SDK (68K) Docs" folder.

    - The Palm examples will be added to your "(CodeWarrior
      Examples)\Palm OS 5 SDK (68K) R3 Examples" folder.

    - The Palm Stationery will be added to your "Stationery\Palm OS
      Application" folder. Any existing stationery will be replaced.

    - The Palm debugger will be added to your "Palm Tools\Palm OS 5
      SDK (68K) Other Tools\PalmDebugger" folder.

    - The overlay tools will be added to your "Palm Tools\Palm OS 5
      SDK (68K) Other Tools\Overlay Tools" folder.


PalmSource, Inc.
1240 Crossman Avenue
Sunnyvale, California 94089
(408) 400-3000
