---------------------------------------------------------------------------
Name

  PmConnectorTest

---------------------------------------------------------------------------
Description

  This application demonstrates the PmConnector Library APIs. It shows how 
  to disable automatic detection of serial devices (not USB devices and 
  power adapters). I also shows how to enable/disable the power_out pin 
  that supplies power to peripherals that are not Athena compliant (the 
  pin is automatically enabled when an Athena compliant peripheral is used).

---------------------------------------------------------------------------
Builds With

  CodeWarrior
  Palm OS Developer Suite
  gcc

---------------------------------------------------------------------------
Devices

  Treo 650 maintenance releases and later Palm OS smartphones

---------------------------------------------------------------------------
Requirements

---------------------------------------------------------------------------
Libraries Used

  PmConnector Library

---------------------------------------------------------------------------
How to Run

  1. Disable Class Detection button: Disables detection of serial devices. 
          It does not disable detection of USB devices and power adapter. 
  2. Power On button: Enable power out pin on the Athena connector that 
          supplies power from Treo to an accessory. 
  3. Power Off button: Disable power out pin on the Athena connector that 
          supplies power from Treo to an accessory.

---------------------------------------------------------------------------
Note


---------------------------------------------------------------------------