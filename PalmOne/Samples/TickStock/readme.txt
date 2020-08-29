Name

Tickstock

-----------------------------------------------------------------------------------------------------------------------
Description


 * TickStock is a sample application that demonstrates:
 *
 * - mainly, how to detect and handle incoming call interruption
 * - how to detect if device is using EvDO connection
 * - downloading data while keeping UI responsive
 * - implementing [background] HTTP request as a state machine
 *
 * The application connects to Yahoo! Finance website and download
 * the HTTP content until it finds the latest price of the queried
 * stock.

 * The requested stocks are Palm, Yahoo!, Google, Microsoft, and Intel.

-----------------------------------------------------------------------------------------------------------------------
Builds with

This project can be built using CodeWarrior 9 and
GCC command line (Please use GCC_makefile to build).


------------------------------------------------------------------------------------------------------------------------
Devices

This sample app should run on all Treo and Centro devices.

--------------------------------------------------------------------------------------------------------
How to run

 * Once you tap on "Refresh" button, the application will open NetLib and establish socket
 * connections to retrieve stock data. It might take several seconds to see "Connecting" prompt
 * window after tapping on "Refresh". 

 * You can also choose "Auto-update every 15 minutes" to enable the application to set a 15 minutes
 * timer to go online and retrieve data.

 *
 
