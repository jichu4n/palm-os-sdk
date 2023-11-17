/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: MailRsc.h
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file defines the ID of the resources used by the Mail 
 *   application.
 *
 * History:
 *		May 31, 1996	Created by Art Lamb
 *
 *****************************************************************************/

// List View
#define ListView							1000
#define ListCategoryTrigger			1003
#define ListCategoryList				1004
#define ListNewButton					1005
#define ListShowButton					1006
#define ListTable							1007
#define ListScrollBar					1008
//mgmg newTitle
#define ListTitleLabel					1001


// Message View
#define MessageView						1100
//mgmg new
#define MessageTitleLabel				1102

#define MessageAbbrHeaderButton		1103
#define MessageFullHeaderButton		1104
#define MessageField						1105		// DOLATER ??? - remove
#define MessageGadget					1105
#define MessageScrollBar				1106
#define MessageDoneButton				1107
#define MessageReplyButton				1108
#define MessageUnsendButton			1109
#define MessageEditButton				1110
#define MessageDeleteButton			1112
#define MessageUndeleteButton			1113
//#define MessageSmallFontButton		1113
//#define MessageLargeFontButton		1114
#define MessageHeaderGroup				1
//#define MessageFontGroup				2

#define MessagePrevMessageButton		1111
#define MessageNextMessageButton		1114


// Message Creation View
#define NewView							1200
#define NewSendButton					1202
#define NewCancelButton					1203
#define NewDetailsButton				1204
#define NewTable							1205
#define NewScrollBar						1206


// Message Edit View (full screen)
#define EditView							1300
#define EditField							1302
#define EditScrollBar					1303
#define EditDoneButton					1304
#define EditLookupButton				1305
#define EditSendButton					1306
//#define EditSmallFontButton			1307
//#define EditLargeFontButton			1308
//#define EditFontGroup					2


// Options Dialog
#define OptionsDialog					1400
#define OptionsSortByTrigger			1404
#define OptionsSortByList				1405
#define OptionsShowDates				1406
#define OptionsOkButton					1407
#define OptionsCancelButton			1408


// Details Dialog Box
#define DetailsDialog					1500
#define DetailsPriorityTrigger		1504
#define DetailsPriorityList			1505
#define DetailsBCCCheckbox				1507				
#define DetailsSignatureCheckbox		1509
#define DetailsReadCheckbox			1511
#define DetailsDeliveryCheckbox		1513
#define DetailsOkButton					1514
#define DetailsCancelButton			1515

// Preferences Dialog Box
#define PrefDialog						1600
#define PrefConfirmDeleteCheckbox	1602
#define PrefSignatureField				1604
#define PrefOkButton						1605
#define PrefCancelButton				1606
#define PrefDefaultMailTrigger			1608
#define PrefDefaultMailList				1609

// Reply Options Dialog Box
#define ReplyDialog						1700
#define ReplyToSenderPushButton		1703
#define ReplyToAllPushButton			1704
#define ReplyForwardPushButton		1705
#define ReplayIncludeTextCheckbox	1706
#define ReplyCommentTextCheckbox		1707
#define ReplyOkButton					1708
#define ReplyCancelButton				1709
#define ReplyToGroup						1

// Sync Options Dialog Box
#define SyncDialog						1800

//mgmg syncUnread
#define SyncUnreadPushButton			1801
#define SyncUnreadLabel			1803

#define SyncSettingsTrigger			1804
#define SyncSettingsList				1805
#define SyncAllPushButton				1806
#define SyncSendOnlyPushButton		1807
#define SyncFilterPushButton			1808
#define SyncAllLabel						1809
#define SyncSendOnlyLabel				1810
#define SyncRetrieveAllCheckbox		1811
#define SyncMessageContaingTrigger	1813
#define SyncMessageContaingList		1814
#define SyncToButton						1815
#define SyncToField						1816
#define SyncFromButton					1817
#define SyncFromField					1818
#define SyncSubjectButton				1819
#define SyncSubjectField				1820
#define SyncOkButton						1821
#define SyncCancelButton				1822
#define SyncTruncateButton				1823
#define SyncTypeGroup					1

// Truncate Options Dialog Box
#define TruncateDialog					1900
#define TruncateList						1903
#define TruncateOkButton				1904
#define TruncateCancelButton			1905

// Delete Message Alert
#define DeleteMessageAlert				2001
#define DeleteMessageYes				0
#define DeleteMessageNo           	1

// File Message Alert
#define FileMessageAlert				2002
#define FileMessageYes					0
#define FileMessageNo           		1
#define FileMessageCancel          	2

// No To Address Alert
#define NoAddressAlert					2003

// No To Address Alert
#define PurgeAlert						2004
#define PurgeYes							0
#define PurgeNo							1

// Save Draft Alert
#define SaveDraftAlert					2005
#define SaveDraftYes						0
#define SaveDraftNo						1
#define SaveDraftCancel					2

// Nothing To Purge Alert
#define NothingToPurgeAlert			2006

//mgmg new
#define RomIncompatibleAlert			2007
#define SaveEmptyAlert					2008

// Menus
#define ListViewMenu						1000
#define NoteViewMenu						1001

#define newMenuCmd						100
#define purgeMenuCmd						101

#define ListChangeFontMenuCmd			200
#define preferencesMenuCmd				201
#define hotSyncMenuCmd					202
#define aboutMenuCmd						203

#define messageNewMenuCmd				300
#define messageFileMenuCmd				301
#define messagePurgeMenuCmd			302

#define messageCopyCmd					400
#define messageSelectAllCmd			401


#define messageChangeFontCmd			500
#define messagePreferencesMenuCmd	501
#define messageHotSyncMenuCmd			502
#define messageAboutMenuCmd			503

#define newSaveDraftMenuCmd			600
#define newPurgeMenuCmd					601

#define newChangeFontMenuCmd			700
#define newPreferencesMenuCmd			701
#define newHotSyncMenuCmd				702
#define newLookupMenuCmd				703
#define newAboutMenuCmd					704

#define editChangeFontCmd				900
#define editLookupMenuCmd				901

// Strings
#define findHeaderStrId					100
#define toStrId							101
#define fromStrId							102
#define subjectStrId						103
#define ccStrId							104
#define dateStrId							105
#define bccStrId							106
#define newViewLabelsStrId				107
#define editViewTitlesStrId			108
#define listTitleStringId				109
#define replyPrefixStrId				110
#define forwardPrefixStrId				111
#define forwardMessageStrId			112
#define wroteOnStrId						113
#define mailToLookupTitleStrID		114
#define mailFromLookupTitleStrID		115
#define mailCCLookupTitleStrID		116
#define mailBCCLookupTitleStrID		117
#define mailLookupAddStrID				118
#define mailLookupFormatStrID			119
#define noSubjectStrID					120
#define filterIgnoreStrID				121
#define purgeTipText						122
#define noSubjectListViewStrID		123
#define noToListViewStrID				124
#define listTitleNotInboxStringId	125
#define MessageTitleStrID				126
#define listTitleSingularStringId	127
#define listTitleNotInboxSingularStrId	128
#define listTitleShortStrID			129
#define mailAddlLookupFormatStrID	130

//BITMAPS
#define readUrgentIconID				1000
#define readIconID						1001
#define urgentIconID						1002
