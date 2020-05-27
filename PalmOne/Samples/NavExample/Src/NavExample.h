/***************************************************************
 *
 *  Project:
 *	  NavExample
 *
 * Copyright info:
 *	  Copyright (c) Handspring 2003 -- All Rights Reserved
 *
 *  FileName:
 *	  NavExample.h
 * 
 *  Description:
 *	  
 *	  
 *
 * History:
 *	  2003-01-17  SLV  Created
 ****************************************************************/

//#include <PalmOS.h>
#include <PalmTypes.h>
#include <HsKeyCommon.h>

#define appFileCreator			   'SV30'
#define appVersionNum              0x02
#define appPrefID                  0x00
#define appPrefVersionNum          0x01


#include "NavExampleRsc.h"



typedef struct
  {
	UInt32 type;
	DmResID id;
  }
  ResourceArrayElement;

typedef struct AppPrefs
  {
	Boolean showItem1;
	Boolean showItem2;
	Boolean showItem3;
	Boolean showItem4;
	Boolean showItem5;
	UInt32 type;
	Char name[33];
  } 
  AppPrefs;



#define min(x,y) (((x)>(y))?(y):(x))

#define GetObjectPtr(frmP,objId) \
		FrmGetObjectPtr(frmP, FrmGetObjectIndex (frmP, objId))

#define GetControlVal(frmP,ctlId) \
		CtlGetValue (GetObjectPtr (frmP, ctlId))

#define SetControlVal(frmP,ctlId,val) \
		CtlSetValue (GetObjectPtr (frmP, ctlId), val)

#define ObjectHasFocus(frmP,objId) \
		(FrmGetFocus (frmP) == FrmGetObjectIndex (frmP, objId))

