#!perl -W
################################################################
# \file traceLogGetPPPData.pl
#
# Creates a PPPdump format binary file out of a T.V. log.  
# The TV log may have other data mixed in--it will be ignored.
#
# Usage:
#   traceLogGetPPPData.pl <logfile> [<outfile>]
#
# \license
# 
# Copyright (c) 2003 Handspring Inc., All Rights Reserved
#
# \author Doug Anderson
# 
################################################################

use File::Basename;
use HTTP::Date;

use lib dirname($0);  # TraceLogUtils.pm should be in the same directory
use TraceLogUtils;	  # as this script...

use constant kMarkSendStart       => 1;
use constant kMarkRecvStart       => 2;
use constant kMarkSendEnd         => 3;
use constant kMarkRecvEnd         => 4;

use constant kMarkTime            => 7;


sub main(@);
main (@ARGV);


##################################################################
# help
##
# Displays help and usage string to the user.
#
# \author  Doug Anderson
#
##################################################################

sub help()
{
  my $helpStr = "\n" .
  "Usage:  " . basename($0) . " <logfile> [<outfile>]\n" .
  "\n" .
  "Creates a PPPdump format binary file out of a T.V. log.  If an\n" .
  "outfile isn't specified, we'll add \".eth\" to the end of the logFile.\n" .
  "\n" .
  "";

  print $helpStr;
}


##################################################################
# outputPppDump
##
# Takes an array of trace records, as returned by TraceLogParse(),
# and creates a file in PPPDump format, which can be understood
# by Ethereal.
#
# This function is essentially converting between two logging
# formats.  The input is from the Handspring PPP driver using
# the Handspring tracing system.  The output is the "PPPDump"
# format.
#
# Handspring's tracing system provides timestampped records.  The
# PPP logging outputs two trace records every time it sends or
# receives bytes.  The first trace record contains 1 byte for
# the direction (0x01 for send, 0x02 for receive) and 2
# bytes for the length.  This record must be exactly 3 bytes.
# The second record contains the data that was sent or recieved.
# It should have exactly the number of bytes specified in the
# previous record.  As you can see, there's quite a bit of 
# redundancy here, which we use to error-check our input.  Since
# old records are purged from the trace system on a record-by-
# record basis, it's possible that we may may only get the 2nd of
# the two records that are part of a single transaction.  We use
# the above error checks to try to figure this out and skip the
# partial packet.
#
# The output format is PPPdump format.  The documentation I found
# for that follows, as documented by Daniel Thompson 
# (STMicroelectronics) <daniel.thompson@st.com>.
#
# +------+
# | 0x07 +------+------+------+         Reset time
# |  t3  |  t2  |  t1  |  t0  |         t = time_t
# +------+------+------+------+
# 
# +------+
# | 0x06 |                              Time step (short)
# |  ts  |                              ts = time step (tenths)
# +------+
#
# +------+
# | 0x05 +------+------+------+         Time step (long)
# | ts3  | ts2  | ts1  | ts0  |         ts = time step (tenths)
# +------+------+------+------+
# 
# +------+
# | 0x04 |                              Receive deliminator (not seen in practice)
# +------+
# 
# +------+
# | 0x03 |                              Send deliminator (not seen in practice)
# +------+
#
# +------+
# | 0x02 +------+                       Received data
# |  n1  |  n0  |                       n = number of bytes following
# |    data     |
# |             |
# 
# +------+
# | 0x01 +------+                       Sent data
# |  n1  |  n0  |                       n = number of bytes following
# |    data     |
# |             |
#
# Note that PPPDump format seems to want each record to consist of
# exactly 1 PPP frame.  A PPP frame starts with 0x7e and ends with
# 0x7e (and has no other instances of 0x7e in it).  I don't think I 
# ever saw that documented anywhere, but it seems to be the case.
#
#
# \param   outputFilePath		IN	The path to the output file
#									to create.
# \param   traceRecRefArrayRef	IN	The trace records, as returned
#									from TraceLogParse().
#
# \retval  nothing
#
# \author  Doug Anderson
#
##################################################################

sub outputPppDump($$)
{
  my ($outputFilePath, $traceRecRefArrayRef) = @_;
  my @traceRecRefArray = @$traceRecRefArrayRef;
  my $traceRecRef;
  my $sendFrameBinaryData;
  my $recvFrameBinaryData;
  my $curDirection;
  my $curLength = 0;
  my $numBytesSent = 0;
  my $numBytesRecv = 0;
  my $numFramesSent = 0;
  my $numFramesRecv = 0;
  my $skippedRecordsWarning = 0;
  my $firstTraceRecRef = ($traceRecRefArray[0]);
  my %firstTraceRec = %$firstTraceRecRef;
  my $lastTraceRecRef = ($traceRecRefArray[@traceRecRefArray - 1]);
  my %lastTraceRec = %$lastTraceRecRef;

  open (OUTFILE, ">", "$outputFilePath") or die "Couldn't open $outputFilePath";
  binmode (OUTFILE);

  foreach $traceRecRef (@traceRecRefArray)
	{
	  my %traceRec = %$traceRecRef;
	  my $binaryData = $traceRec{"binaryData"};
	  my $time = $traceRec{"time"};
	  my $ticks = $traceRec{"ticks"};

	  if ($curLength != 0)
		{
		  # The previous record looked like a direction/length record.  If it really
		  # was, we know the length of this record.  Let's double-check.

		  if (length ($binaryData) != $curLength)
			{
			  print STDERR "WARNING: Trace record length mismatch (" . length ($binaryData) . " != $curLength); skipping...\n";
			  print STDERR time2str ($time) . " " . $ticks . " " . unpack ("H*", $binaryData) . "\n\n";

			  # We'll try to interpret this record as a direction and length when we hit the next if statement...
			  $curLength = 0;
			}
		  else
			{
			  # Data is valid...
			  my $frameBinaryDataRef;
			  my $numFramesRef;
			  my $numBytesRef;
			  my $kPppMarker = pack ("C", 0x7e);

			  # Figure out direction and setup a reference to the place to store data...
			  if ($curDirection == kMarkSendStart)
				{
				  $frameBinaryDataRef = \$sendFrameBinaryData;
				  $numBytesRef = \$numBytesSent;
				  $numFramesRef = \$numFramesSent;
				}
			  else
				{
				  $frameBinaryDataRef = \$recvFrameBinaryData;
				  $numBytesRef = \$numBytesRecv;
				  $numFramesRef = \$numFramesRecv;
				}

			  while (defined ($binaryData))
				{
				  my $frameEndIndex;
				  my $binaryFrame;
				  
				  # Binary data may contain more than one frame (ugh).  If we detect an
				  # end of frame in the data, we'll just process the data up to that point,
				  # then loop through again...
				  if (not defined $$frameBinaryDataRef)
					{
					  # We expect a start of frame as the first character, so skip over it
					  # when looking for the end of frame (frames start and end with kPppMarker)...
					  $frameEndIndex = index ($binaryData, $kPppMarker, 1);
					}
				  else
					{
					  # We've already seen the start of frame, so look immediately for the
					  # end of frame.
					  $frameEndIndex = index ($binaryData, $kPppMarker, 0);
					}

				  # If we didn't find the end of the frame, use the whole binary data; 
				  # otherwise, just use up till the end of the frame...
				  if ($frameEndIndex == -1)
					{
					  $binaryFrame = $binaryData;
					}
				  else
					{
					  $binaryFrame = substr ($binaryData, 0, $frameEndIndex + 1);
					}

				  # If this is the first record in this direction, it should be the start of a
				  # PPP frame.  If it's not, that means we missed the start of the the frame.
				  # We'll skip this record.
				  if ((not defined $$frameBinaryDataRef) && (substr ($binaryFrame, 0, 1) ne $kPppMarker))
					{
					  my $outputBinaryData;

					  if (!$skippedRecordsWarning)
						{
						  print STDERR "\nWARNING: Frame start missing; trace records dumped as 'padding'...\n{\n";
						  $skippedRecordsWarning = 1;
						}
					  print STDERR time2str ($time) . " (" . $ticks . "): " . unpack ("H*", $binaryFrame) . "\n";

					  # Output the frame as the 'padding' protocol.  Ethereal doesn't seem to mind that
					  # I don't put a 0x7e at the beginning.
					  $outputBinaryData = pack ("CNCnna*", 
												kMarkTime, $time, 
												$curDirection, length ($binaryFrame) + 2, 0x0001,
												$binaryFrame);
					  print OUTFILE $outputBinaryData;
					}
				  else
					{
					  if ($skippedRecordsWarning)
						{
						  print STDERR "}\n...this is normal at the beginning of a trace log (or shows a carrier drop).\n\n";
						  $skippedRecordsWarning = 0;
						}
						  
					  # Add this data to previous data...
					  if (defined ($$frameBinaryDataRef))
						{
						  $$frameBinaryDataRef = $$frameBinaryDataRef . $binaryFrame;
						}
					  else
						{
						  $$frameBinaryDataRef = $binaryFrame;
						}

					  # See if we're done...
					  if ((length ($$frameBinaryDataRef) > 1) && (substr ($$frameBinaryDataRef, -1, 1) eq $kPppMarker))
						{
						  # We're done...

						  my $outputBinaryData;

						  # Keep track of statistics...
						  $$numBytesRef += length ($$frameBinaryDataRef);
						  $$numFramesRef += 1;

						  $outputBinaryData = pack ("CNCna*", 
													kMarkTime, $time, 
													$curDirection, 
													length ($$frameBinaryDataRef), $$frameBinaryDataRef);
						  print OUTFILE $outputBinaryData;

						  undef ($$frameBinaryDataRef);
						}
					}

				  if (length ($binaryData) == length ($binaryFrame))
					{
					  undef ($binaryData);
					}
				  else
					{
					  $binaryData = substr ($binaryData, length ($binaryFrame));
					}
				}
			}
		}

	  # If we're looking for a direction/length trace record...
	  if ($curLength == 0)
		{
		  # Looking for a direction and length...

		  if (length ($binaryData) != 3)
			{
			  print STDERR "WARNING: Trace record not a length / direction; skipping...\n";
			  print STDERR time2str ($time) . " " . $ticks . " " . unpack ("H*", $binaryData) . "\n\n";
			}
		  else
			{
			  ($curDirection, $curLength) = unpack ("Cn", $binaryData);

			  if (($curDirection != kMarkSendStart) && ($curDirection != kMarkRecvStart))
				{
				  print STDERR "WARNING: Trace record has invalid direction; skipping...\n";
				  print STDERR time2str ($time) . " " . $ticks . " " . unpack ("H*", $binaryData) . "\n\n";
				  $curLength = 0;
				}
			}
		}
	  else
		{
		  # We already dealt with the data...
		  $curLength = 0;
		}
	}

  if (defined $sendFrameBinaryData)
	{
	  print STDERR "Missing end of last sent packet; skipping...\n";
	  print STDERR unpack ("H*", $sendFrameBinaryData) . "\n\n";
	}
  if (defined $recvFrameBinaryData)
	{
	  print STDERR "Missing end of last received packet; skipping...\n";
	  print STDERR unpack ("H*", $recvFrameBinaryData) . "\n\n";
	}

  print "Sent:        $numBytesSent bytes ($numFramesSent frames)\n";
  print "Recv:        $numBytesRecv bytes ($numFramesRecv frames)\n";
  print "First frame: " . time2str ($firstTraceRec{"time"}) . "\n";
  print "Last  frame: " . time2str ($lastTraceRec{"time"}) . "\n\n";

  close (OUTFILE);
}



##################################################################
# main
##
# The main entry point of this script.
#
# \author  Doug Anderson
#
##################################################################

sub main(@)
{
  if ((not defined ($_[0])) || ($_[0] =~ /^--help$/))
	{
	  help();
	}
  else
	{
	  my $line;
	  my $traceRecRefArrayRef;
	  my $logFilePath;
	  my $outputFilePath;

	  $logFilePath = shift;
	  $outputFilePath = shift;

	  # Make sure a log file was specified...
	  if (not defined ($logFilePath))
		{
		  die "Need to specify a log file.  Try --help";
		}

	  # If no output file was specified, make one up by adding ".eth" to the
	  # end of the input file.
	  if (not defined ($outputFilePath))
		{
		  $outputFilePath = $logFilePath . ".eth";
		}

	  print "Parsing log file...\n";
	  $traceRecRefArrayRef = TraceLogParse ($logFilePath, "PPP!", "DATA");

	  print "Outputting data...\n";
	  outputPppDump ($outputFilePath, $traceRecRefArrayRef);
	}
}

