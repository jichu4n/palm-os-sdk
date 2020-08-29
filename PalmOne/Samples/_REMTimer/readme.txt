---------------------------------------------------------------------------
Name

  _REMTimer

---------------------------------------------------------------------------
Description

  _REMTimer demonstrates how to make a device go into deferred sleep mode, 
  REM sleep mode and deep sleep mode.

---------------------------------------------------------------------------
Builds With

  CodeWarrior
  Gcc

---------------------------------------------------------------------------
Devices

  All Treo smartphones

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used


---------------------------------------------------------------------------
How to Run

1. Launch the application
2. Tap on 'sleep' button
   * Timer should start but device will not go to sleep (sleep deferred)
3. Tap anywhere on the screen to stop the timer
4. Tap on 'REM sleep' button
   * Timer should start and device should go to sleep after the count of
     3 (device is in REM sleep)
5. Press the power button to turn device back on
   * An alert should pop up, timer should indicate that it has been 
     running while device is on, and the defer count should increase 
     (device is kept in REM sleep)
6. Tap anywhere on the screen to quit the timer screen
7. Tap on 'off' button
   * Timer should start and device should go to sleep after the count of
     3 (device should go to deep sleep)
8. Press the power button to turn device back on
   * An alert should pop up, timer should indicate that it has been 
     running while device is on, but the defer count should be 0 (our
     application is not keeping the device in REM sleep)

---------------------------------------------------------------------------
Note


---------------------------------------------------------------------------
