
---------------------------------------------------------------------------
Name

	NavExample

---------------------------------------------------------------------------
Description
	
	1. This sample code shows how the 5-way navigation key works programmatically
	   This sample code demonstrates how to handle the following key events:
		- vchrRockerUp
		- pageUpChr
		- vchrRockerDown
		- pageDownChr
		- vchrRockerCenter
	2. This sample code also illustrates how to draw a 3-column table on a form. Refer
	   to TableFrmEventHandler() routinee for more information
	3. This sample code shows how to use Prefeneces Manager to set (PrvSetPrefs() routine)
	   and retrieve (PrvGetPrefs() routine) application specific preferences. 
	   You use application preferences to store state specific to your application that 
	   should persist across invocations of your application.
	
Files:

  - NavExample.c : Main application. Does all the work.

 

---------------------------------------------------------------------------
Builds With

	CodeWarrior9
	GCC command line (Please use GCC_makefile to build)
	PODS (Please use PODS_makefile to build)

---------------------------------------------------------------------------
Devices

	Treo devices

---------------------------------------------------------------------------
Requirements
	
	
---------------------------------------------------------------------------
Libraries Used
	Preference Manager 

---------------------------------------------------------------------------
How to Run
	
	To test 5-way navigation feature:
	1. Launch the 'NavExample' application
	2. On the main page, the focus should be on the list. 
	3. Navigate to the drop-down list (pop trigger) by tapping on Left or Right key on 5-way 
	   navigation key, press center key on 5-way navigation key, you can choose either All, 
	   appl, and ovly options.The list should include all database under each option
	4. Choose one database by using Up and Down key
	5. Tap on Open button will show you a message saying "This is where we would open the database..."
	   This is just an example to show developers that they can plug their database code in there
	
	To test table feature:
	1. Go to main form
	2. Tap on Table button, you should see a new form with 3-column. (Note, the line number is
	   the 1st column)
	3. Tap on anywhere on the screen, you can start typing. If you enter text longer than one line, 
	   you should see the line extended to 2 or 3 rows but stay in the same column.

	To test Preference Manager feature
	1. Launch Menu and select Preference option
	2. Check the preference by checking or unchecking some items. (Items here is just an example, they
	   are not associated with any form or list within the sample code)
	3. Close the sample code and launch it again
	4. Go to Preference option again, you should see the same setting as you did in step 2.

   
---------------------------------------------------------------------------
Note
	
	There are 3 forms in this sample code:
	1. Main form, listing selected database
	2. Table form
	3. Preference form

---------------------------------------------------------------------------