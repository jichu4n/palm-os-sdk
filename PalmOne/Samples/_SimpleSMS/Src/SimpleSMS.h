///** 
/// * \file   SimpleSMS.h
/// *
/// * Simple SMS
/// *
/// * Simple SMS Application. Basic messaging operations
/// *
/// * \license
/// * 
/// * Copyright (c) 2002 Handspring Inc., All Rights Reserved
/// *
/// * $Id:$
/// *
/// ***************************************************************/

// Form
#define resFormIDSimpleSMS			1000

#define resLabelServiceCenter       1001
#define resFieldServiceCenter       1002
#define resLabelAddress             1003
#define resFieldAddress             1004
#define resLabelMessage				1005
#define resFieldMessage				1006
#define resButtonSend				1007

#define resGadgetSignalLevel		1101
#define resGadgetMainBatteryLevel	1102
#define resGadgetSignalLevel2		1103
#define resGadgetMainBatteryLevel2	1104


#define resFormIDSimpleSMSNoSend	1200

#define resLabelReceiveOnly			1201

// Menu
#define resMenuIDSimpleSMS			1000

#define resMenuItemUnRegister		1001
#define resMenuItemSeperator		1002
#define	resMenuItemAbout			1003

#define resMenuItemEditUndo         2000	//  Command Key: U
#define resMenuItemEditCut          2001	//  Command Key: X
#define resMenuItemEditCopy         2002	//  Command Key: C
#define resMenuItemEditPaste        2003	//  Command Key: P
#define resMenuItemEditSelectAll    2004	//  Command Key: S
#define resMenuItemEditSeperator    2005
#define resMenuItemEditKeyboard     2006	//  Command Key: K
#define resMenuItemEditGraffitiHelp 2007	//  Command Key: G

// Alerts
#define resAlertNotTreo             1000
#define resAlertNotTreoOK           0

#define resAlertPhoneLibrary        1001
#define resAlertPhoneLibraryOK      0

#define resAlertPhoneNotReady       1002
#define resAlertPhoneNotReadyOK     0

#define resAlertCantRegisterWithSMSLibrary	  1003
#define resAlertCantRegisterWithSMSLibraryOK  0

#define resAlertNotRegistered       1004
#define resAlertNotRegisteredOK     0

#define resAlertIncomingMessage     1005
#define resAlertIncomingMessageOK   0

#define resAlertIncomingNBSMessage     1006
#define resAlertIncomingNBSMessageOK   0

#define resAlertSuccessfulTransfer	  1007
#define resAlertSuccessfulTransferOK  0

#define resAlertUnsuccessfulTransfer	1008
#define resAlertUnsuccessfulTransferOK	0

#define resAlertNoAddressInput			1009 // for the case when user tries to send a SMS but doesn't enter a phone number
#define resAlertNoAddressInputOK		0
