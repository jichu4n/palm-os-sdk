
---------------------------------------------------------------------------
Name

	HtmlLibTest68K

---------------------------------------------------------------------------
Description
	
	This sample code shows how to render text and image data using HTML library on
	GSM phones and T|X handheld.
	
Files:

  - HtmlLibTestMain.c : Main application. 
  - HtmlLibTest.c: Html render text and image data.
  - HTLFileUtil.c: File system utility calls.
  - smile.jpg: a image file sample. Need to be saved under the root of SD card.

  Major functions used:

  - HtmlLibAddTextData ();
  - HtmlLibAddImageData ();	
  - HtmlLibRenderData ();

---------------------------------------------------------------------------
Builds With

	CodeWarrior9
        Gcc

---------------------------------------------------------------------------
Devices

	Treo GSM Phones, Palm T|X handheld

---------------------------------------------------------------------------
Requirements
	
	
---------------------------------------------------------------------------
Libraries Used

	HTML Library

---------------------------------------------------------------------------
How to Run
	
	1. Copy \Rsc\smile.jpg to the root of SD card.
	2. Launch the HtmlLibTest application.
	3. Chooose either Test1 or Test2 pages, and tap on Render to see the result.
        4. Select "Find Text On Page" from the PAGE menu.
        5. Enter some text that is in this page and click on the "Find" button.

---------------------------------------------------------------------------
Note
	
	If you want to render bitmap image file, please use Treo 700p and above Treo devices.  

        There is a known issue with the HtmlLibResizeScreen API. When resizing 
        the redered page twice, the scroll bar appears twice.

---------------------------------------------------------------------------