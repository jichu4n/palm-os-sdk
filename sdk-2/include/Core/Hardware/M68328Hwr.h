/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		M68328Hwr.h
 *
 * Description:
 *		Hardware Equates for the Motorola 68328 Dragonball chip 
 *
 * History:
 *   	4/19/95  RM		- Created by Ron Marianetti
 *		5/15/95	VMK	- Added macros for RTC bits
 *
 *******************************************************************/  

#ifdef	NON_PORTABLE						// So apps don't include this...


#ifndef	__M68328HWR_H__
#define	__M68328HWR_H__


#include <Common.h>							// Touchdown includes


/***********************************************************************
 * Motorola 68328  registers
 ***********************************************************************/
typedef struct HwrDBallType {

	Byte		scr;								// $000: System Control Register
	Byte											___filler1[0xFF];				 
	
	Word		csAGroupBase;					// $100: Chip Select Group A Base Register
	Word		csBGroupBase;					// $102: Chip Select Group B Base Register
	Word		csCGroupBase;					// $104: Chip Select Group C Base Register
	Word		csDGroupBase;					// $106: Chip Select Group D Base Register
	
	Word		csAGroupMask;					// $108: Chip Select Group A Mask Register
	Word		csBGroupMask;					// $10A: Chip Select Group B Mask Register
	Word		csCGroupMask;					// $10C: Chip Select Group C Mask Register
	Word		csDGroupMask;					// $10E: Chip Select Group D Mask Register
	
	DWord		csASelect0;						// $110: Group A Chip Select 0 Register
	DWord		csASelect1;						// $114: Group A Chip Select 1 Register
	DWord		csASelect2;						// $118: Group A Chip Select 2 Register
	DWord		csASelect3;						// $11C: Group A Chip Select 3 Register
	
	DWord		csBSelect0;						// $120: Group B Chip Select 0 Register
	DWord		csBSelect1;						// $124: Group B Chip Select 1 Register
	DWord		csBSelect2;						// $128: Group B Chip Select 2 Register
	DWord		csBSelect3;						// $12C: Group B Chip Select 3 Register
	
	DWord		csCSelect0;						// $130: Group C Chip Select 0 Register
	DWord		csCSelect1;						// $134: Group C Chip Select 1 Register
	DWord		csCSelect2;						// $138: Group C Chip Select 2 Register
	DWord		csCSelect3;						// $13C: Group C Chip Select 3 Register
	
	DWord		csDSelect0;						// $140: Group D Chip Select 0 Register
	DWord		csDSelect1;						// $144: Group D Chip Select 1 Register
	DWord		csDSelect2;						// $148: Group D Chip Select 2 Register
	DWord		csDSelect3;						// $14C: Group D Chip Select 3 Register
	
	Word		csDebug;							// $150: Chip Select debug register
	Byte											___filler2[0x200-0x152];		
	
	Word		pllControl;						// $200: PLL Control Register
	Word		pllFreqSel;						// $202: PLL Frequency Select Register
	Word		pllTest;							// $204: PLL Test Register
	Byte											__filler44;
	Byte		pwrControl;						// $207: Power Control Register
	
	Byte											___filler3[0x300-0x208];
	
	Byte		intVector;						// $300: Interrupt Vector Register
	Byte											___filler4;
	Word		intControl;						// $302: Interrupt Control Register
	Word		intMaskHi;						// $304: Interrupt Mask Register/HIGH word
	Word		intMaskLo;						// $306: Interrupt Mask Register/LOW word
	Word		intWakeupEnHi;					// $308: Interrupt Wakeup Enable Register
	Word		intWakeupEnLo;					// $30A: Interrupt Wakeup Enable Register
	Word		intStatusHi;					// $30C: Interrupt Status Register/HIGH word
	Word		intStatusLo;					// $30E: Interrupt Status Register/LOW word
	Word		intPendingHi;					// $310: Interrupt Pending Register
	Word		intPendingLo;					// $312: Interrupt Pending Register
	
	Byte 											___filler4a[0x400-0x314];
	
	Byte		portADir;						// $400: Port A Direction Register
	Byte		portAData;						// $401: Port A Data Register
	Byte											___filler5;
	Byte		portASelect;					// $403: Port A Select Register
	
	Byte											___filler6[4];
	
	Byte		portBDir;						// $408: Port B Direction Register
	Byte		portBData;						// $409: Port B Data Register
	Byte											___filler7;
	Byte		portBSelect;					// $40B: Port B Select Register	
	
	Byte											___filler8[4];
	
	Byte		portCDir;						// $410: Port C Direction Register
	Byte		portCData;						// $411: Port C Data Register
	Byte											___filler9;
	Byte		portCSelect;					// $413: Port C Select Register	
	
	Byte											___filler10[4];
	
	Byte		portDDir;						// $418: Port D Direction Register
	Byte		portDData;						// $419: Port D Data Register
	Byte		portDPullupEn;					// $41A: Port D Pull-up Enable
	Byte											___filler11;
	Byte		portDPolarity;					// $41C: Port D Polarity Register
	Byte		portDIntReqEn;					// $41D: Port D Interrupt Request Enable
	Byte											___filler12;
	Byte		portDIntEdge;					// $41F: Port D IRQ Edge Register
	
	Byte		portEDir;						// $420: Port E Direction Register
	Byte		portEData;						// $421: Port E Data Register
	Byte		portEPullupEn;					// $422: Port E Pull-up Enable
	Byte		portESelect;					// $423: Port E Select Register
	
	Byte											___filler14[4];
	
	Byte		portFDir;						// $428: Port F Direction Register
	Byte		portFData;						// $429: Port F Data Register
	Byte		portFPullupEn;					// $42A: Port F Pull-up Enable
	Byte		portFSelect;					// $42B: Port F Select Register
	
	Byte											___filler16[4];
	
	Byte		portGDir;						// $430: Port G Direction Register
	Byte		portGData;						// $431: Port G Data Register
	Byte		portGPullupEn;					// $432: Port G Pull-up Enable
	Byte		portGSelect;					// $433: Port G Select Register
	
	Byte											___filler18[4];
	
	Byte		portJDir;						// $438: Port J Direction Register
	Byte		portJData;						// $439: Port J Data Register
	Byte											___filler19;
	Byte		portJSelect;					// $43B: Port J Select Register
	
	Byte											___filler19a[4];
	
	Byte		portKDir;						// $440: Port K Direction Register
	Byte		portKData;						// $441: Port K Data Register
	Byte		portKPullupEn;					// $442: Port K Pull-up Enable
	Byte		portKSelect;					// $443: Port K Select Register
	
	Byte											___filler21[4];
	
	Byte		portMDir;						// $448: Port M Direction Register
	Byte		portMData;						// $449: Port M Data Register
	Byte		portMPullupEn;					// $44A: Port M Pull-up Enable Register
	Byte		portMSelect;					// $44B: Port M Select Register
	
	Byte											___filler22[4];
	
	Byte											___filler23[0x500-0x450];
	
	Word		pwmControl;						// $500: PWM Control Register
	Word		pwmPeriod;						// $502: PWM Period Register
	Word		pwmWidth;						// $504: PWM Width Register
	Word		pwmCounter;						// $506: PWM Counter
	
	Byte											___filler24[0x600-0x508];
	
	Word		tmr1Control;					// $600: Timer 1 Control Register
	Word		tmr1Prescaler;					// $602: Timer 1 Prescaler Register
	Word		tmr1Compare;					// $604: Timer 1 Compare Register
	Word		tmr1Capture;					// $606: Timer 1 Capture Register
	Word		tmr1Counter;					// $608: Timer 1 Counter Register
	Word		tmr1Status;						// $60A: Timer 1 Status Register
	
	Word		tmr2Control;					// $60C: Timer 2 Control Register
	Word		tmr2Prescaler;					// $60E: Timer 2 Prescaler Register
	Word		tmr2Compare;					// $610: Timer 2 Compare Register
	Word		tmr2Capture;					// $612: Timer 2 Capture Register
	Word		tmr2Counter;					// $614: Timer 2 Counter Register
	Word		tmr2Status;						// $616: Timer 2 Status Register
	
	Word		wdControl;						// $618: Watchdog Control Register
	Word		wdReference;					// $61A: Watchdog Reference Register
	Word		wdCounter;						// $61C: Watchdog Counter
	
	Byte											___filler25[0x700-0x61E];
	
	Word		spiSlave;						// $700: SPI Slave Register
	
	Byte											___filler26[0x800-0x702];
	
	Word		spiMasterData;					// $800: SPI Master Data Register
	Word		spiMasterControl;				// $802: SPI Master Control Register
	
	Byte											___filler27[0x900-0x804];
	
	Word		uControl;						// $900: Uart Control Register
	Word		uBaud;							// $902: Uart Baud Control Register
	Word		uReceive;						// $904: Uart Receive Register
	Word		uTransmit;						// $906: Uart Transmit Register
	Word		uMisc;							// $908: Uart Miscellaneous Register
	
	Byte											___filler28[0xA00-0x90A];
	
	DWord		lcdStartAddr;					// $A00: Screen Starting Address Register
	Byte											___filler29;
	Byte		lcdPageWidth;					// $A05: Virtual Page Width Register
	Byte											___filler30[2];
	Word		lcdScreenWidth;				// $A08: Screen Width Register
	Word		lcdScreenHeight;				// $A0A: Screen Height Register
	Byte											___filler31[0xA18-0xA0C];
	Word		lcdCursorXPos;					// $A18: Cursor X Position
	Word		lcdCursorYPos;					// $A1A:	Cursor Y Position
	Word		lcdCursorWidthHeight;		// $A1C: Cursor Width and Height
	Byte											___filler32;
	Byte		lcdBlinkControl;				// $A1F: Blink Control Register
	Byte		lcdPanelControl;				// $A20: Panel Interface Control Register
	Byte		lcdPolarity;					// $A21: Polarity Config Register
	Byte											___filler33;						
	Byte		lcdACDRate;						// $A23: ACD (M) Rate Control Register
	Byte											___filler34;
	Byte		lcdPixelClock;					// $A25: Pixel Clock Divider Register
	Byte											___filler35;
	Byte		lcdClockControl;				// $A27: Clocking Control Register
	Byte											___filler36;
	Byte		lcdLastBufferAddr;			// $A29: Last Buffer Address Register
	Byte											___filler37;
	Byte		lcdOctetTermCount;			// $A2B: Octet Terminal Count Register
	Byte											___filler38;
	Byte		lcdPanningOffset;				// $A2D: Panning Offset Register
	Byte											___filler39[3];
	Byte		lcdFrameRate;					// $A31: Frame Rate Control Modulation Register
	Word		lcdGrayPalette;				// $A32: Gray Palette Mapping Register
	Byte		lcdReserved;					// $A34: Reserved
	
	Byte											___filler40[0xB00-0xA35];
	
	DWord		rtcHourMinSec;					// $B00: RTC Hours, Minutes, Seconds Register
	DWord		rtcAlarm;						// $B04: RTC Alarm Register
	DWord		rtcReserved;					// $B08: RTC Reserved
	Word		rtcControl;						// $B0C: RTC Control Register
	Word		rtcIntStatus;					// $B0E: RTC Interrupt Status Register
	Word		rtcIntEnable;					// $B10: RTC Interrupt Enable Register
	Word		stopWatch;						// $B12: Stopwatch Minutes
	
	} HwrDBallType;

typedef	volatile   HwrDBallType*	HwrDBallPtr;


/************************************************************************
 * LCD Controller Bits
 ************************************************************************/
#define	hwr328LcdCursorXPosCtlMask			0xC000
	#define	hwr328LcdCursorXPosCtlTrans	0x0000
	#define	hwr328LcdCursorXPosCtlBlack	0x4000
	#define	hwr328LcdCursorXPosCtlReverse	0x8000
	
#define	hwr328LcdBlinkControlEnable		0x80 

#define	hwr328LcdPanelControlBusMask		0x06
	#define	hwr328LcdPanelControlBus1Bit	0x00
	#define	hwr328LcdPanelControlBus2Bit	0x02
	#define	hwr328LcdPanelControlBus4Bit	0x04
#define	hwr328LcdPanelControlGrayScale	0x01

#define	hwr328LcdPolarityShiftClock		0x08
#define	hwr328LcdPolarityFLM					0x04
#define	hwr328LcdPolarityLP					0x02
#define	hwr328LcdPolarityPixel				0x01

#define	hwr328LcdClockControlEnable			0x80
#define	hwr328LcdClockControl16WordBursts	0x40
#define	hwr328LcdClockControlBurstRateMask	0x30
// Old dragonball allows up to 4 clocks/word
#if HW_REV < 3
	#define	hwr328LcdClockControlBurstRate1		0x00
	#define	hwr328LcdClockControlBurstRate2		0x10
	#define	hwr328LcdClockControlBurstRate3		0x20
	#define	hwr328LcdClockControlBurstRate4		0x30
// New dragonball allows up to 16 clocks/word
#else
	#define	hwr328LcdClockControlBurstRate1		0x00
	#define	hwr328LcdClockControlBurstRate2		0x04
	#define	hwr328LcdClockControlBurstRate3		0x08
	#define	hwr328LcdClockControlBurstRate4		0x0C
	#define	hwr328LcdClockControlBurstRate5		0x10
	#define	hwr328LcdClockControlBurstRate6		0x14
	#define	hwr328LcdClockControlBurstRate7		0x18
	#define	hwr328LcdClockControlBurstRate8		0x1C
	#define	hwr328LcdClockControlBurstRate9		0x20
	#define	hwr328LcdClockControlBurstRate10		0x24
	#define	hwr328LcdClockControlBurstRate11		0x28
	#define	hwr328LcdClockControlBurstRate12		0x2C
	#define	hwr328LcdClockControlBurstRate13		0x30
	#define	hwr328LcdClockControlBurstRate14		0x34
	#define	hwr328LcdClockControlBurstRate15		0x38
	#define	hwr328LcdClockControlBurstRate16		0x3C
#endif	
#define	hwr328LcdClockControl8BitBus			0x02
#define	hwr328LcdClockControlPixelClkSrc		0x01


/************************************************************************
 * Interrupt Controller Bits
 ************************************************************************/
// intMaskHi bits
#define	hwr328IntHiNMI							0x0080
#define	hwr328IntHiTimer1						0x0040
#define	hwr328IntHiSPIS						0x0020
#define	hwr328IntHiPen							0x0010
#define	hwr328IntHiIRQ6						0x0008
#define	hwr328IntHiIRQ3						0x0004
#define	hwr328IntHiIRQ2						0x0002
#define	hwr328IntHiIRQ1						0x0001

//NOTE: Due to a bug in early rev's of the DragonBall, the clear bits for
/// IRQ1,2,3,6 are different than the indicator bits
#define	hwr328IntHiIRQ6Clr					0x0001
#define	hwr328IntHiIRQ3Clr					0x0002
#define	hwr328IntHiIRQ2Clr					0x0004
#define	hwr328IntHiIRQ1Clr					0x0008

// intMaskLo bits
#define	hwr328IntLoInt7						0x8000
#define	hwr328IntLoInt6						0x4000
#define	hwr328IntLoInt5						0x2000
#define	hwr328IntLoInt4						0x1000
#define	hwr328IntLoInt3						0x0800
#define	hwr328IntLoInt2						0x0400
#define	hwr328IntLoInt1						0x0200
#define	hwr328IntLoInt0						0x0100
#define	hwr328IntLoAllKeys					0xFF00
#define	hwr328IntLoInt0Bit					8
#define	hwr328IntLoPWM							0x0080
#define	hwr328IntLoPWMBit						7
#define	hwr328IntLoKbd							0x0040
#define	hwr328IntLoLCDC						0x0020
#define	hwr328IntLoRTC							0x0010
#define	hwr328IntLoRTCBit						4
#define	hwr328IntLoWDT							0x0008
#define	hwr328IntLoUART						0x0004
#define	hwr328IntLoUARTBit					2
#define	hwr328IntLoTimer2						0x0002
#define	hwr328IntLoTimer2Bit					1
#define	hwr328IntLoSPIM						0x0001

// intControl bits
// NOTE: These are different (at least in version 0G58E) that documented in the
//  User's manual
#define	hwr328IntCtlEdge1						0x0800	
#define	hwr328IntCtlEdge2						0x0400
#define	hwr328IntCtlEdge3						0x0200
#define	hwr328IntCtlEdge6						0x0100
#define	hwr328IntCtlPol1						0x8000	
#define	hwr328IntCtlPol2						0x4000
#define	hwr328IntCtlPol3						0x2000
#define	hwr328IntCtlPol6						0x1000


/************************************************************************
 * Timer bits
 ************************************************************************/
#define	hwr328TmrControlUnused						0xFE00
#define	hwr328TmrControlFreeRun						0x0100
#define	hwr328TmrControlCaptureEdgeMask			0x00C0
	#define	hwr328TmrControlCaptureEdgeNone		0x0000
	#define	hwr328TmrControlCaptureEdgeRising	0x0040
	#define	hwr328TmrControlCaptureEdgeFalling	0x0080
	#define	hwr328TmrControlCaptureEdgeBoth		0x00C0
#define	hwr328TmrControlOutputModeToggle			0x0020
#define	hwr328TmrControlEnInterrupt				0x0010
#define	hwr328TmrControlClkSrcMask					0x000E
	#define	hwr328TmrControlClkSrcStop				0x0000
	#define	hwr328TmrControlClkSrcSys				0x0002
	#define	hwr328TmrControlClkSrcSysBy16			0x0004
	#define	hwr328TmrControlClkSrcTIN				0x0006
	#define	hwr328TmrControlClkSrc32KHz			0x0008
#define	hwr328TmrControlEnable						0x0001

#define	hwr328TmrStatusCapture						0x0002
#define	hwr328TmrStatusCaptureBit					1
#define	hwr328TmrStatusCompare						0x0001
#define	hwr328TmrStatusCompareBit					0


/************************************************************************
 * 68328 Serial Port Register Bits
 ************************************************************************/
// uControl Register
#define	hwr328UControlUARTEnable			0x8000
#define	hwr328UControlRxEnable				0x4000
#define	hwr328UControlTxEnable				0x2000
#define	hwr328UControlRxClock1x				0x1000
#define	hwr328UControlParityEn				0x0800
#define	hwr328UControlParityOdd				0x0400
#define	hwr328UControlStopBits2				0x0200
#define	hwr328UControlDataBits8				0x0100
#define	hwr328UControlGPIODeltaEn			0x0080
#define	hwr328UControlCTSDeltaEn			0x0040
#define	hwr328UControlRxFullEn				0x0020
#define	hwr328UControlRxHalfEn				0x0010
#define	hwr328UControlRxRdyEn				0x0008
#define	hwr328UControlTxEmptyEn				0x0004
#define	hwr328UControlTxHalfEn				0x0002
#define	hwr328UControlTxAvailEn				0x0001

#define	hwr328UControlEnableAll			   (hwr328UControlUARTEnable |  	\
														 hwr328UControlRxEnable |		\
														 hwr328UControlTxEnable)

// uBaud Register
#define	hwr328UBaudGPIODelta					0x8000
#define	hwr328UBaudGPIOData					0x4000
#define	hwr328UBaudGPIODirOut				0x2000
#define	hwr328UBaudGPIOSrcBaudGen			0x1000
#define	hwr328UBaudBaudSrcGPIO				0x0800
#define	hwr328UBaudDivider					0x0700
#define	hwr328UBaudPrescaler					0x003F

#define	hwr328UBaudDivideBitOffset			8


// uReceive Register
#define	hwr328UReceiveFIFOFull				0x8000
#define	hwr328UReceiveFIFOHalf				0x4000
#define	hwr328UReceiveDataRdy				0x2000
#define	hwr328UReceiveDataRdyBit			13
#define	hwr328UReceiveOverrunErr			0x0800
#define	hwr328UReceiveOverrunErrBit		11
#define	hwr328UReceiveFrameErr				0x0400
#define	hwr328UReceiveFrameErrBit			10
#define	hwr328UReceiveBreakErr				0x0200
#define	hwr328UReceiveBreakErrBit			9
#define	hwr328UReceiveParityErr				0x0100
#define	hwr328UReceiveParityErrBit			8
#define	hwr328UReceiveData					0x00FF

#define	hwr328UReceiveErrsMask				(hwr328UReceiveOverrunErr |	\
														 hwr328UReceiveFrameErr |		\
														 hwr328UReceiveBreakErr |		\
														 hwr328UReceiveParityErr)

// uTransmit Register
#define	hwr328UTransmitFIFOEmpty			0x8000
#define	hwr328UTransmitFIFOHalf				0x4000
#define	hwr328UTransmitTxAvail				0x2000
#define	hwr328UTransmitSendBreak			0x1000
#define	hwr328UTransmitIgnoreCTS			0x0800
#define	hwr328UTransmitCTSStatus			0x0200
#define	hwr328UTransmitCTSDelta				0x0100
#define	hwr328UTransmitData					0x00FF

// uMisc Register
#define	hwr328UMiscClkSrcGPIO				0x4000
#define	hwr328UMiscForceParityErr			0x2000
#define	hwr328UMiscLoopback					0x1000
#define	hwr328UMiscReservedMask				0x8F00
#define	hwr328UMiscRTSThruFIFO				0x0080
#define	hwr328UMiscRTSOut						0x0040
#define	hwr328UMiscIRDAEn						0x0020
#define	hwr328UMiscLoopIRDA					0x0010
#define	hwr328UMiscUnused						0x000F



/************************************************************************
 * PWM Bits
 ************************************************************************/
// PWM register bits of interest
#define	hwr328PWMControlEnable				0x0010
#define	hwr328PWMControlEnableIRQ			0x4000
#define	hwr328PWMControlLoad					0x0100
#define	hwr328PWMIRQStatus					0x8000
// PWM divider chain output selectors
#define	hwr328PWMControlDivMask				0x0007
#define	hwr328PWMControlDivBy2				0x0
#define	hwr328PWMControlDivBy4				0x1
#define	hwr328PWMControlDivBy8				0x2
#define	hwr328PWMControlDivBy16				0x3
#define	hwr328PWMControlDivBy32				0x4
#define	hwr328PWMControlDivBy64				0x5
#define	hwr328PWMControlDivBy128			0x6
#define	hwr328PWMControlDivBy256			0x7


/************************************************************************
 * PLL Bits
 ************************************************************************/
#define	hwr328PLLControlDisable				0x0008
#define	hwr328PLLControlClkEn				0x0010

#define	hwr328PLLControlSysVCODiv2			0x0000
#define	hwr328PLLControlSysVCODiv4			0x0100
#define	hwr328PLLControlSysVCODiv8			0x0200
#define	hwr328PLLControlSysVCODiv16		0x0300
#define	hwr328PLLControlSysVCODiv1			0x0400

#define	hwr328PLLControlPixVCODiv2			0x0000  
#define	hwr328PLLControlPixVCODiv4			0x080
#define	hwr328PLLControlPixVCODiv8			0x1000
#define	hwr328PLLControlPixVCODiv16		0x1800
#define	hwr328PLLControlPixVCODiv1			0x2000

/************************************************************************
 * Real Time Clock (RTC) Bits
 ************************************************************************/

// RTC Control Register (RTCCTL) bits
#define hwr328RTCControlRTCEnable				0x80
#define hwr328RTCControlRefSelMask				0x20
#define hwr328RTCControlRefSel38400				0x20		// 38.4 kHz reference frequency
#define hwr328RTCControlRefSel32768				0x00		// 32.768 kHz reference frequency

// RTC Interrupt Enable Register (RTCIENR) bits
#define hwr328RTCIntEnableSec						0x10
#define hwr328RTCIntEnable24Hr					0x08
#define hwr328RTCIntEnableAlarm					0x04
#define hwr328RTCIntEnableMinute					0x02
#define hwr328RTCIntEnableStopWatch				0x01

// RTC Interrupt Status Register bits
#define hwr328RTCIntStatusSec						0x10
#define hwr328RTCIntStatus24Hr					0x08
#define hwr328RTCIntStatusAlarm					0x04
#define hwr328RTCIntStatusMinute					0x02
#define hwr328RTCIntStatusStopWatch				0x01

// RTC Alarm register (RTCALRM) masks and offsets
#define hwr328RTCAlarmSecondsMask				0x0000003fL
#define hwr328RTCAlarmSecondsOffset				0
#define hwr328RTCAlarmMinutesMask				0x003f0000L
#define hwr328RTCAlarmMinutesOffset				16
#define hwr328RTCAlarmHoursMask					0x1f000000L
#define hwr328RTCAlarmHoursOffset				24

// RTC hours/minutes/seconds register (RTCHMS) masks and offsets
#define hwr328RTCHourMinSecSecondsMask			0x0000003fL
#define hwr328RTCHourMinSecSecondsOffset		0
#define hwr328RTCHourMinSecMinutesMask			0x003f0000L
#define hwr328RTCHourMinSecMinutesOffset		16
#define hwr328RTCHourMinSecHoursMask			0x1f000000L
#define hwr328RTCHourMinSecHoursOffset			24


/************************************************************************
 * SPI Master bits
 ************************************************************************/
#define	hwr328SPIMControlRateMask			0xE000
	#define	hwr328SPIMControlRateDiv4			0x0000
	#define	hwr328SPIMControlRateDiv8			0x2000
	#define	hwr328SPIMControlRateDiv16			0x4000
	#define	hwr328SPIMControlRateDiv32			0x6000
	#define	hwr328SPIMControlRateDiv64			0x8000
	#define	hwr328SPIMControlRateDiv128		0xA000
	#define	hwr328SPIMControlRateDiv256		0xC000
	#define	hwr328SPIMControlRateDiv512		0xE000
#define	hwr328SPIMControlEnable				0x0200
#define	hwr328SPIMControlExchange			0x0100
#define	hwr328SPIMControlIntStatus			0x0080
#define	hwr328SPIMControlIntEnable			0x0040
#define	hwr328SPIMControlOppPhase			0x0020
#define	hwr328SPIMControlInvPolarity		0x0010
#define	hwr328SPIMControlBitsMask			0x000F


/************************************************************************
 * I/O Port C pins
 ************************************************************************/
#define	hwr328PortCMOClk						0x01
#define	hwr328PortCUDS							0x02
#define	hwr328PortCLDS							0x04
#define	hwr328PortCNMI							0x10
#define	hwr328PortCDTack						0x20
#define	hwr328PortCPcmciaWE					0x40


/************************************************************************
 * I/O Port G pins
 ************************************************************************/
#define	hwr328PortGUartTxD					0x01
#define	hwr328PortGUartRxD					0x02
#define	hwr328PortGPwmOut						0x04
#define	hwr328PortGTOut2						0x08
#define	hwr328PortGTIn2						0x10
#define	hwr328PortGTOut1						0x20
#define	hwr328PortGTIn1						0x40
#define	hwr328PortGRtcOut						0x80


/************************************************************************
 * I/O Port K pins
 ************************************************************************/
#define	hwr328PortKSpimTxD					0x01
#define	hwr328PortKSpimRxD					0x02
#define	hwr328PortKSpimClkO					0x04
#define	hwr328PortKSpisEn						0x08
#define	hwr328PortKSpisRxD					0x10
#define	hwr328PortKSpisClkI					0x20
#define	hwr328PortKPcmciaCE2					0x40
#define	hwr328PortKPcmciaCE1					0x80


/************************************************************************
 * I/O Port M pins
 ************************************************************************/
#define	hwr328PortMCTS							0x01
#define	hwr328PortMRTS							0x02
#define	hwr328PortMIRQ6						0x04
#define	hwr328PortMIRQ3						0x08
#define	hwr328PortMIRQ2						0x10
#define	hwr328PortMIRQ1						0x20
#define	hwr328PortMPenIRQ						0x40
#define	hwr328PortMUnused7					0x80


 


#endif // __M68328_H__

#endif // NON_PORTABLE











