/*******************************************************************
 Copyright © 1995 - 1998, 3Com Corporation or its subsidiaries ("3Com").  
 All rights reserved.
   
 This software may be copied and used solely for developing products for 
 the Palm Computing platform and for archival and backup purposes.  Except 
 for the foregoing, no part of this software may be reproduced or transmitted 
 in any form or by any means or used to make any derivative work (such as 
 translation, transformation or adaptation) without express written consent 
 from 3Com.

 3Com reserves the right to revise this software and to make changes in content 
 from time to time without obligation on the part of 3Com to provide notification 
 of such revision or changes.  
 3COM MAKES NO REPRESENTATIONS OR WARRANTIES THAT THE SOFTWARE IS FREE OF ERRORS 
 OR THAT THE SOFTWARE IS SUITABLE FOR YOUR USE.  THE SOFTWARE IS PROVIDED ON AN 
 "AS IS" BASIS.  3COM MAKES NO WARRANTIES, TERMS OR CONDITIONS, EXPRESS OR IMPLIED, 
 EITHER IN FACT OR BY OPERATION OF LAW, STATUTORY OR OTHERWISE, INCLUDING WARRANTIES, 
 TERMS, OR CONDITIONS OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
 SATISFACTORY QUALITY.

 TO THE FULL EXTENT ALLOWED BY LAW, 3COM ALSO EXCLUDES FOR ITSELF AND ITS SUPPLIERS 
 ANY LIABILITY, WHETHER BASED IN CONTRACT OR TORT (INCLUDING NEGLIGENCE), FOR 
 DIRECT, INCIDENTAL, CONSEQUENTIAL, INDIRECT, SPECIAL, OR PUNITIVE DAMAGES OF 
 ANY KIND, OR FOR LOSS OF REVENUE OR PROFITS, LOSS OF BUSINESS, LOSS OF INFORMATION 
 OR DATA, OR OTHER FINANCIAL LOSS ARISING OUT OF OR IN CONNECTION WITH THIS SOFTWARE, 
 EVEN IF 3COM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 3Com, HotSync, Palm Computing, and Graffiti are registered trademarks, and 
 Palm III and Palm OS are trademarks of 3Com Corporation or its subsidiaries.

 IF THIS SOFTWARE IS PROVIDED ON A COMPACT DISK, THE OTHER SOFTWARE AND 
 DOCUMENTATION ON THE COMPACT DISK ARE SUBJECT TO THE LICENSE AGREEMENT 
 ACCOMPANYING THE COMPACT DISK.

 *-------------------------------------------------------------------
 * FileName:  
 *		PilotStdio.h
 *
 * Description:
 *		Pilot standard IO equivalents.
 *
 * History:
 *		11/24/97	vmk		- Created by Vitaly Kruglikov 
 *
 *******************************************************************/

#ifndef __PILOTSTDIO_H__
#define __PILOTSTDIO_H__

#include <FileStream.h>


#define FILE		VoidPtr
#define size_t		ULong

#ifndef EOF
	#define EOF		(-1)
#endif

#define SEEK_SET	fileOriginBeginning
#define SEEK_CUR	fileOriginCurrent
#define SEEK_END	fileOriginEnd

#define fopenHelper(cardNo, filename, mode)																\
	FileOpen((cardNo)/*cardNo*/, (filename), 0/*def type*/, 0/*def creator*/,					\
	(((mode)[0] == 'r' && (mode)[1] == '+') ?	(fileModeUpdate		| fileModeExclusive) :	\
	((mode)[0] == 'r') ?								(fileModeReadOnly) :									\
	((mode)[0] == 'w') ?								(fileModeReadWrite	| fileModeExclusive) :	\
	((mode)[0] == 'a') ?								(fileModeAppend		| fileModeExclusive) :	\
	0) | fileModeAnyTypeCreator,																				\
	NULL)

#define fopen(filename, mode)	fopenHelper(0/*cardNo*/, (filename), (mode))
#define fclose(stream)			(FileClose(stream) ? EOF : 0)
#define fread(buffer, objsize, count, stream)	FileRead(stream, buffer, objsize, count, NULL)
#define fwrite(buffer, objsize, count, stream)	FileWrite(stream, buffer, objsize, count, NULL)
#define ferror(stream)			FileError(stream)
#define feof(stream)				FileEOF(stream)
#define ftell(stream)			FileTell(stream, NULL, NULL)
#define clearerr(stream)		FileClearerr(stream)
#define fseek(stream, offset, whence)	FileSeek(stream, offset, whence)
#define rewind(stream)			FileRewind(stream)
#define fflush(stream)			(FileFlush(stream) ? EOF : 0)
#define unlink(filename)		(FileDelete(0, (filename)) ? -1 : 0)


#define assert(condition)		ErrFatalDisplayIf(!(condition), "assertion failed: " # condition );

#endif
