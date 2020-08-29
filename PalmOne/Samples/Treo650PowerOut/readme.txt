---------------------------------------------------------------------------
Name

  Treo650PowerOut

---------------------------------------------------------------------------
Description

  This sample code shows how to power on/off the Multi-connector POWER_OUT 
  pin on Treo 650.

  Features:
  - Power On the POWER_OUT pin (Treo 650 Only).
  - Power Off the POWER_OUT pin (Treo 650 Only).

  Files:
  - Treo650PowerOut: Shows how to power on/off the POWER_OUT pin on Treo 650.

---------------------------------------------------------------------------
Builds With

  CodeWarrior
  Gcc

---------------------------------------------------------------------------
Devices

  Treo 650

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used


---------------------------------------------------------------------------
How to Run

  1. Power On the POWER_OUT pin by tapping on the Power On POWER_OUT button
  2. Power Off the POWER_OUT pin by tapping on the Power Off POWER_OUT
     button

---------------------------------------------------------------------------
Note

  WARNING: This sample code uses an ARMlet to power on/off the POWER_OUT 
  pin on Treo 650. This is not officially supported by Palm. Please use 
  this at your own risk. Ideally, you should be using the PmConnectorLib, 
  if available.

Recommendations:

Here is what needs to be done in the application:
#if (CPU_TYPE != CPU_68K) || (defined BUILDING_PMCONNECTOR_LIB)
	#define PMCONNECTOR_LIB_TRAP(trapNum)
#else
	#include <LibTraps.h>
	#define PMCONNECTOR_LIB_TRAP(trapNum) SYS_TRAP(trapNum) #endif

#define kPmConnectorLibType		  			sysFileTLibrary
#define kPmConnectorLibCreator	      		'PmAt'
#define kPmConnectorLibName		  			"PmConnector"


#define kPmConnectorLibTrapOpen				sysLibTrapOpen
#define kPmConnectorLibTrapClose			sysLibTrapClose

#define kPmConnectorLibCtrlPowerOn			0x01
#define kPmConnectorLibCtrlPowerOff			0x02

Err
PmConnectorLibOpen (UInt16 refNum)
      PMCONNECTOR_LIB_TRAP (kPmConnectorLibTrapOpen);

Err
PmConnectorLibClose (UInt16 refNum)
      PMCONNECTOR_LIB_TRAP (kPmConnectorLibTrapClose);

Err
PmConnectorLibControl (UInt16 refNum, UInt16 cmdId, void *parmP)
      PMCONNECTOR_LIB_TRAP (sysLibTrapCustom + 1);

// Send Power to the Power_out pin
{
	// Try to find the library
	err = SysLibFind( kPmConnectorLibName, &refNum );	
	if ( err != errNone )
		err = SysLibLoad( kPmConnectorLibType, kPmConnectorLibCreator,
                          &refNum );

	if ( err )
	{
		// Use the Armlet to Power on the POWER_OUT Pin (See Sample code for more details)
	}
	else
	{
		PmConnectorLibOpen( refNum );
		PmConnectorLibControl( refNum, kPmConnectorLibCtrlPowerOn, (void *)NULL);						
		PmConnectorLibClose( refNum );				
	}
}

---------------------------------------------------------------------------





