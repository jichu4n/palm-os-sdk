/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:
 *		DateTime.h
 *
 * Description:
 *		Date and Time calculations
 *
 * History:
 *   	1/19/95  rsf - Created by Roger Flores
 *
 *
 *******************************************************************/

#ifndef 	__DATETIME_H__
#define	__DATETIME_H__


typedef DWord time_t;

typedef struct {
	SWord second;
	SWord minute;
	SWord hour;
	SWord day;
	SWord month;
	SWord year;
	SWord weekDay;		// Days since Sunday (0 to 6)
	} DateTimeType;
	
typedef DateTimeType * DateTimePtr;


// This is the time format.  Times are treated as words so don't 
// change the order of the members in this structure.
//
typedef struct {
	Byte        hours;
	Byte        minutes;
} TimeType;

typedef TimeType * TimePtr;

#define noTime	-1		// The entire TimeType is -1 if there isn't a time.


// This is the date format.  Dates are treated as words so don't 
// change the order of the members in this structure.
//
typedef struct {
	Word year  :7;                   // years since 1904 (MAC format)
	Word month :4; 
	Word day   :5;
} DateType;

typedef DateType * DatePtr;


/************************************************************
 * Date Time Constants
 *************************************************************/

// Maximum lengths of strings return by the date and time formating
// routine DateToAscii and TimeToAscii.
#define timeStringLength	9
#define dateStringLength	9
#define longDateStrLength	15


#define firstYear				1904
#define numberOfYears		128
#define lastYear				(firstYear + numberOfYears - 1)



// Constants for time calculations
// Could change these from xIny to yPerX
#define minutesInSeconds	60
#define hoursInMinutes		60
#define hoursInSeconds		(hoursInMinutes * minutesInSeconds)
#define hoursPerDay			24
//#define daysInSeconds		((long)(hoursPerDay) * ((long)hoursInSeconds))
#define daysInSeconds		(0x15180)	// cc bug

#define daysInWeek			7
#define daysInYear			365
#define daysInLeapYear		366
#define daysInFourYears		(daysInLeapYear + 3 * daysInYear)

#define monthsInYear			12

#define maxDays				((ULong) numberOfYears / 4 * daysInFourYears - 1)
#define maxSeconds			((ULong) maxDays * daysInSeconds)
#define maxTime				0x 

// Values returned by DayOfWeek routine.
#define sunday					0
#define monday					1
#define tuesday				2
#define wednesday				3
#define thursday				4
#define friday					5
#define saturday				6

// Months of the year
#define january				1
#define february				2
#define march					3
#define april					4
#define may						5
#define june					6
#define july					7
#define august					8
#define september				9
#define october				10
#define november				11
#define december				12


// Values returned by DoyOfMonth routine.
typedef enum {
	dom1stSun, dom1stMon, dom1stTue, dom1stWen, dom1stThu, dom1stFri, dom1stSat,
	dom2ndSun, dom2ndMon, dom2ndTue, dom2ndWen, dom2ndThu, dom2ndFri, dom2ndSat,
	dom3rdSun, dom3rdMon, dom3rdTue, dom3rdWen, dom3rdThu, dom3rdFri, dom3rdSat,
	dom4thSun, dom4thMon, dom4thTue, dom4thWen, dom4thThu, dom4thFri, dom4thSat,
	domLastSun, domLastMon, domLastTue, domLastWen, domLastThu, domLastFri, 
	domLastSat
	} DayOfWeekType;


//************************************************************
//* Date and Time macros
//***********************************************************

// Convert a date in a DateType structure to an unsigned int.
 #define DateToInt(date) (*(UInt *) &date)
 
 
// Convert a date in a DateType structure to an unsigned int.
 #define TimeToInt(time) (*(Int *) &time)
 
 

//************************************************************
//* Date Time procedures
//************************************************************
#ifdef __cplusplus
extern "C" {
#endif


void TimSecondsToDateTime(ULong seconds, DateTimePtr dateTimeP)
			SYS_TRAP(sysTrapTimSecondsToDateTime);

ULong TimDateTimeToSeconds(DateTimePtr dateTimeP)
			SYS_TRAP(sysTrapTimDateTimeToSeconds);

void TimAdjust(DateTimePtr dateTimeP, Long adjustment)
			SYS_TRAP(sysTrapTimAdjust);

void TimeToAscii(Byte hours, Byte minutes, TimeFormatType timeFormat, 
				CharPtr pString)
			SYS_TRAP(sysTrapTimeToAscii);



UInt DaysInMonth(UInt month, UInt year)
			SYS_TRAP(sysTrapDaysInMonth);

UInt DayOfWeek (UInt month, UInt day, UInt year)
			SYS_TRAP(sysTrapDayOfWeek);

UInt DayOfMonth (UInt month, UInt day, UInt year)
			SYS_TRAP(sysTrapDayOfMonth);



// Date routines.
void DateSecondsToDate (ULong seconds, DatePtr date)
			SYS_TRAP(sysTrapDateSecondsToDate);

void DateDaysToDate (ULong days, DatePtr date)
			SYS_TRAP(sysTrapDateDaysToDate);

ULong DateToDays (DateType date)
			SYS_TRAP(sysTrapDateToDays);

void DateAdjust (DatePtr dateP, Long adjustment)
			SYS_TRAP(sysTrapDateAdjust);

void DateToAscii(Byte months, Byte days, Word years, 
				DateFormatType dateFormat, CharPtr pString)
			SYS_TRAP(sysTrapDateToAscii);

void DateToDOWDMFormat(Byte months, Byte days,  Word years,
				DateFormatType dateFormat, CharPtr pString)
			SYS_TRAP(sysTrapDateToDOWDMFormat);


#ifdef __cplusplus 
}
#endif


#endif //__DATETIME_H__
