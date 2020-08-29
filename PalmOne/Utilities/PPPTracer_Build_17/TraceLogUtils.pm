#!perl -W
################################################################
# \file TraceLogUtils.pl
#
# This is a perl module for parsing a trace log file generated
# using the "Dump" button in T.V.  It loads the whole file 
# into an array of hashes and does a bunch of processing for
# you.
#
# The fact that this loads the whole file into RAM is a bit
# on the expensive side, but sould be OK.  We shouldn't ever
# have trace dumps that are > 32 megs anyway, since that's
# the total amount of RAM on current devices.  If trace logs
# ever get too big, we'll have to rely on virtual memory to 
# bail us out.  ;)
#
# \license
# 
# Copyright (c) 2003 Handspring Inc., All Rights Reserved
#
# \author Doug Anderson
# 
################################################################

package TraceLogUtils;

use vars qw(@ISA @EXPORT);

use Exporter;
$VERSION = 1.00;
@ISA = qw(Exporter);

@EXPORT = qw(
	TraceLogParse
);

use HTTP::Date;

##################################################################
# compareTraceRecsByRecNum 
##
# Compares two trace records, trying to identify which one was
# entered into the database first.  It uses two pieces of data
# for this determincation: the record number, and whether this
# record came before a wraparound (due to a full database) was
# detected.
#
# \param   rec1Ref	  IN	A reference to the 1st record.
# \param   rec2Ref	  IN	A reference to the 2nd record.
#
# \retval  -1 if rec1Ref <  rec2Ref
#           0 if rec1Ref == rec2Ref
#          +1 if rec1Ref >  rec2Ref
#
# \author  Doug Anderson
#
##################################################################

sub compareTraceRecsByRecNum($$)
{
  my ($rec1Ref, $rec2Ref) = @_;
  my %rec1 = %$rec1Ref;
  my %rec2 = %$rec2Ref;
  my $recNum1;
  my $recNum2;
  my $result;

  # If it has wrapped around, it's earlier than if it hasn't...
  # ...note that we make negative numbers, since a 1 is less than a 0.
  $result = -($rec1{"didWraparound"} <=> $rec2{"didWraparound"});
  if ($result != 0)
	{
	  return $result;
	}

  # If both have the same wraparound status, just compare by recNum.
  $result = ($rec1{"recNum"} <=> $rec2{"recNum"});
  if ($result != 0)
	{
	  return $result;
	}

  die "ERROR: Two identical recNums?\n";
}


##################################################################
# TraceLogParse
##
# This will parse a TV log file and extract records that you
# specify.  It also does some amount of processing on the records.
#
# Records are returned as a reference to an array of references to
# hashes.  The array is sorted by our best guess as to the order
# the records were entered into the trace database.  Currently, 
# the hash contains the following keys:
#   recNum - the record number of this record in the file.  Might
#			 not start at 0.  However, lower record numbers always
#			 were before higher record numbers in the file.
#   didWraparound - if 1, we detected a full tracelog and this
#					record came after the detection.  Any records
#					with didWraparound as 1 are believe to have
#					been put in the trace database before any 
#					records with didWraparound as 0.
#	cid - The CID of this record, as a string.
#	scid - The SCID of this record, as a string.
#	time - The time of this record, as a time_t.
#	ticks - The ticks of this record, as an integer.
#	binaryData - The binary data of this record (the data between
#				 the <data> and </data> tags.  This is given as
#				 a binary string.
#	lineStr - The line containing the record, where the client can
#			  do further parsing.
#
# IMPORTANT: This function isn't perfect.  There are currently
# several limitations.  Namely:
#   - Requires trace records to start with "<TraceRec>", which
#     is CASE SENSITIVE.
#   - It makes the assumption that the trace database is mostly
#	  in order (the only out of order allowed is when the trace
#	  database is full and we wrap around).
#	- the 'lineStr' may be a big bogus for the first and last
#	  trace records, since it may contain the tags that begin
#	  and end the trace database.
#   - it would be great to try to interpret 'ticks' into a 
#	  millisecond offset into 'time'.
#
# Note that this function may output errors or warnings.  A future
# version might take in a parameter saying whether this function
# should be silent.
#
#
# \param   logFilePath	  IN	The path to the TV logFile to read.
# \param   requiredCid	  IN	CID of records to extract; "*" is 
#								wildcard.
# \param   requiredScid	  IN	SCID of records to extract; "*" is
#								wildcard.
#
# \retval  A reference to an array of references to hashes.  Each
#		   hash represents a matching trace record.
#
# \author  Doug Anderson
#
##################################################################

sub TraceLogParse($$$)
{
  my ($logFilePath, $requiredCid, $requiredScid) = @_;
  my @traceRecArray;
  my $lastTime;
  my $didWraparound = 0;
  local $/ = "<TraceRec>";

  # Allow undefined CID / SCID as wildcard...
  if (not defined ($requiredCid))
	{
	  $requiredCid = "*";
	}
  if (not defined ($requiredScid))
	{
	  $requiredScid = "*";
	}

  if ($logFilePath eq "-")
	{
	  *LOG = STDIN;
	}
  else
	{
	  open (LOG, $logFilePath) or die "Can't open: $logFilePath\n";
	}

  while ($line = <LOG>)
	{
	  my $cid;
	  my $scid;
	  my $doesCidMatch;
	  my $doesScidMatch;

	  # Extract CID and SCID from the line...
	  if ($line =~ m|^.*<CID>(.+)</CID>|is)
		{
		  $cid = $1;
		}
	  if ($line =~ m|^.*<SCID>(.+)</SCID>|is)
		{
		  $scid = $1;
		}

	  # We match as long as the line actually had a CID and SCID, and it's equal to the
	  # required one (or we've got a wildcard)
	  $doesCidMatch  = defined ($cid) && (($requiredCid eq "*")  || ($requiredCid eq $cid));
	  $doesScidMatch = defined ($scid) && (($requiredScid eq "*") || ($requiredScid eq $scid));

	  if ($doesCidMatch && $doesScidMatch)
		{
		  my $dateStr = "";
		  my $timeStr = "";
		  my $data;
		  my $binaryData;
		  my $time;
		  my $ticks = 0;
		  my %traceRec;

		  # Extract data from the record, if the record has a data tag (not all records do).
		  if ($line =~ m|^.*<Data>(.+)</Data>|is)
			{
			  $data = $1;

			  # Strip out quotes, if they exist (they might not)...
			  if ($data =~ /^(.*)".*"/s)
				{
				  $data = $1;
				}

			  # Pack the data so it's binary, which will make it easier to deal with later...
			  $binaryData = pack ("H*", join ("", split (' ', $data)));
			}

		  # Extract the date, if it exists (can it ever not exist?)
		  # ...format is "year.month.day"
		  if ($line =~ m|^.*<Date>(.+)\.(.+)\.(.+)</Date>|is)
			{
			  my $year = $1;
			  my $month = $2;
			  my $day = $3;
			  
			  # Some trace output accidentally cast the year to 8-bit.  If
			  # we detect that, we'll adjust.  No real harm done, since
			  # that goes from 1792 to 2047...
			  if ($year <= 255)
				{
				  $year += 1792;
				}

			  $dateStr = "$year/$month/$day";
			}

		  # Time is pretty easy to extract...
		  if ($line =~ m|^.*<Time>(.+)</Time>|is)
			{
			  $timeStr = $1;
			}

		  # Convert from time and date strings to a time_t.
		  $time = str2time ("$dateStr $timeStr");

		  # Ticks are pretty easy to extract
		  if ($line =~ m|^.*<Ticks>(.+)</Ticks>|is)
			{
			  $ticks = $1;
			}

		  # If we ever go backward in time by 2 or more seconds, we'll
		  # assume we've hit the wraparound that happens in a full trace database...
		  # ...we probably could check for 1 second of backwards, but 2 seconds
		  # seems safer and lets us deal with slightly out of order trace databases (?)
		  if (defined ($lastTime) && ($time - $lastTime <= -2))
			{
			  if ($didWraparound)
				{
				  # If we get here, we've detected two wraparounds, which is illegal.
				  print STDERR "\nWARNING: out of order trace database\n\n";
				}
			  else
				{
				  print STDERR "\nWARNING: Full tracelog detected; first entry at: " . time2str ($time) . "\n";
				  print STDERR "...this is normal, but be aware packets before this time have been dropped\n\n";
				}
			  $didWraparound = 1;
			}
		  $lastTime = $time;

		  # Store the trace record in our array...
		  %traceRec = 
			(
			  "recNum"        => $.,
			  "didWraparound" => $didWraparound,
			  "cid"           => $cid,
			  "scid"          => $scid,
			  "time"          => $time, 
			  "ticks"         => $ticks, 
			  "binaryData"    => $binaryData,
			  "lineStr"       => $line,
			);
		  push (@traceRecArr, \%traceRec);
		}
	}

  # close file
  if ($logFilePath ne "-")
	{
	  close(LOG);
	}

  # Sort, to fix the wraparound...
  @traceRecArr = sort compareTraceRecsByRecNum @traceRecArr;

  return \@traceRecArr;
}
  

