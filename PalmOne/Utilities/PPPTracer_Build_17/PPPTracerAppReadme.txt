
Preparing to get a PPP trace on the device:
-------------------------------------------
- Install PPPTracer application

- Enable PPP tracing, and set the log to be a reasonable size
	(like 10240 records)

- IMPORTANT: Disconnect (if you’re already connected) 
	Tracing takes effect on the next connection
   

Getting the PPP trace:
----------------------
- Run PalmDebugger

- On device, run PPPTracer

- Connect the device to the PC using USB cable or a serial cable with the
	Treo 650 serial adapter(dongle)

- Click the “Dump…” button

- In the “Debugger” window of PalmDebugger, enter the “g” command

- Log will be dumped to the debugger window

- Copy and paste the log into a text editor and save it
   

Converting a PPP Trace to PPPDump format (can be read by Ethereal):
-------------------------------------------------------------------
- Install ActiveState Perl (a free tool)

- Get the script files: “traceLogGetPPPData.pl” and “TraceLogUtils.pm”

- Put the script files in the same directory as your trace log

- From the DOS prompt, go to the directory with the logfile and type:
           perl traceLogGetPPPData.pl <LogFile>

- A new file, “<LogFile>.eth” has been created
 
- Reading the PPPDump

- Get Ethereal (a free tool — search the web)

- Open the log file

NOTE: a reset will reset the trace settings and trace database content.