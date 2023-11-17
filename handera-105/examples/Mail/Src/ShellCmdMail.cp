/******************************************************************************
 *
 * Copyright (c) 1996-2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: ShellCmdMail.cp
 *
 * Release: Palm OS SDK 4.0 (63220)
 *
 * Description:
 *	  This file is where customized console shell comands are kept.
 * Currently this is a stub which the emulator calls when it doesn't
 * recognize a command.  The code here shows how to add a custom command.
 * The custom command is useless, but it does provide a template to add commands.
 * An application programmer can copy this file to their application 
 * folder and then customize it to MemHandle as many commands as they wish.
 *
 * History:
 *		Feb 26, 1996	Created by Roger Flores
 *
 *****************************************************************************/

#if 0
#include <PalmOS.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ShellCmd.h"

typedef void ShellCmdFuncType (int argc, Char * argv[]);
typedef ShellCmdFuncType * ShellCmdFuncPtr;
typedef struct {
	Char *				longName;
	Char *				shortName;
	ShellCmdFuncPtr	func;
} ShellCmdInfoType;
#endif

/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1996 -- All Rights Reserved
 *
 * PROJECT:  Pilot 2.0
 *
 * FILE:     ShellCmdMail.cp
 *
 * AUTHOR:	 Art Lamb: May 30, 1996
 *
 * DECLARER: Mail
 *
 * DESCRIPTION:
 *	  This file contains the console commands for Mail application.
 *
 ***********************************************************************/

// Pilot Includes 
#include <PalmOS.h>

// C library Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// DB includes
#include "DateTime.h"
#include "MailDB.h"

#include "ShellCmd.h"


typedef void ShellCmdFuncType (int argc, Char * argv[]);
typedef ShellCmdFuncType * ShellCmdFuncPtr;
typedef struct {
	const Char *	longName;
	const Char *	shortName;
	ShellCmdFuncPtr	func;
} ShellCmdInfoType;

#pragma pcrelstrings off

// DOLATER ??? - Put these in one of the include files
#define mailDBType				'DATA'
#define sysFileCMail				'mail'


/**********************************************************************
 * find an open appointment database
 *
 * Parameters: none
 ***********************************************************************/
static DmOpenRef FindOpenedMailDatabases (void)
{
	DmOpenRef	dbP=0;
	LocalID		dbID;
	UInt16			mode;
	UInt16			cardNo;
	UInt32			dbType;
	UInt32			dbCreator;
	
	do {
		dbP = DmNextOpenDatabase(dbP);
		if (!dbP) break;

		Err err = DmOpenDatabaseInfo(dbP, &dbID, NULL, &mode, &cardNo, NULL);
		if (err) 
			{
			ShlInsertText("\n#ERROR getting info");
			}
		else
			{
			DmDatabaseInfo(cardNo, dbID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
				&dbType, &dbCreator);

			if ( (dbType == mailDBType) && (dbCreator == sysFileCMail) )
				return (dbP);
			}
		} while (1);


	return(0);
}


/**********************************************************************
 * Set the appointment application chunk to the defaults.
 *
 * MailSetInfoDefaults <access ptr>
 ***********************************************************************/
static void DoMailSetInfoDefaults(int argc, Char * argv[])
{
	Boolean		usageErr = false;
	DmOpenRef	dbP=0;
	char		text[256];
	int		i;

	for (i=1; i<argc; i++) {
		if (!dbP)
			sscanf(argv[i], "%lx", &dbP);
		else
			usageErr = true;
		
		}

	if (!dbP) usageErr = true;

	if (usageErr) {
		sprintf(text, "Syntax: %s <access ptr>\n", argv[0]);
		ShlInsertText(text);
		return;
		}

	Err err = MailAppInfoInit (dbP);
	if (err) {
		ShlInsertText("\n##ERROR Setting defaults\n");
		}
	else {
		ShlInsertText("Success!\n");
		}
}



/**********************************************************************
 * Test the newRecord function.
 *
 * DoMailTestNewRecord <access ptr>
 ***********************************************************************/ 
static void DoMailNewRecords(int argc, Char * argv[])
{
	Boolean		usageErr = false;
	DmOpenRef	dbP=0;
	UInt16 		index;
	char		text[256];
	int			i;

#ifdef __MWERKS__
#pragma const_strings off
#endif

	MailDBRecordType	testRecord1 = {
		94, 1, 28,
		{14, 30},
		{0, 1, 0, 0, priorityNormal, 0, 0 },
		"Dinner Plans",
		"mary@aol.com",
		"johndoe@aol.com",
		"",
		"",
		"",
		"",
		"John,\nWe are meeting at Dan and Leslie's place around 8:00 PM.  Don't forget "
		"to bring the birthday present for Dan.  See you there."
	};

	MailDBRecordType	testRecord2 = {
		94, 1, 28,
		{0, 0},
		{0, 1, 0, 0, priorityNormal, 0, 0 },
		"Meeting notes",
		"david@palm.com",
		"david@palm.com",
		"",
		"",
		"",
		"",
		"no body text"
	};

	MailDBRecordType	testRecord3 = {
		94, 1, 27,
		{0, 0},
		{0, 1, 0, 0, priorityNormal, 0, 0 },
		"Little brother",
		"Gayle Stoltz",
		"Gayle Stoltz",
		"",
		"",
		"",
		"",
		"no body text"
	};

	MailDBRecordType	testRecord4 = {
		94, 1, 27,
		{0, 0},
		{0, 1, 0, 0, priorityNormal, 0, 0 },
		"Press Release",
		"connie@wilsontec",
		"connie@wilsontec",
		"",
		"",
		"",
		"",
		"no body text"
	};

	MailDBRecordType	testRecord5 = {
		94, 1, 26,
		{0, 0},
		{0, 1, 0, 0, priorityNormal, 0, 0 },
		"IR Beaming",
		"michael@3com.com",
		"michael@3com.com",
		"",
		"",
		"",
		"",
		"no body text"
	};

/*
	MailDBRecordType	testRecord1 = {
		92, 5, 30,
		{0, 0},
		{1, 0, 0, 0, priorityHigh, sentCC, 0 },
		"Test Mail Message",
		"joe@palm.com (Joe Sipher)",
		"monty@palm.com (Monty Boyer)\nart@palm.com (Art Lamb)",
		"roger@palm.com (Roger Flores)",
		"mcauwet@palm (Marian Cauwet)",
		"replyTo@alo.com",
		"art@palm.com (Art Lamb)",
		"This is a test mail message\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12"
	};
	
	MailDBRecordType	testRecord2 = {
		92, 6, 3,
		{0, 0},
		{0, 1, 0, 0, priorityHigh, 0, 0 },
		"Message Two",
		"alamb@aol.com",
		"Engineering@palm.com",
		"monty@palm.com (Monty Boyer)\njoe@palm.com (Joe Sipher)",
		"",
		"",
		"",
		"This is test message two."
	};
	
	MailDBRecordType	testRecord3 = {
		92, 6, 3,
		{0, 0},
		{0, 0, 1, 0, 1, 0, 0 },
		"Message Three",
		"Supervisor",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message three."
	};
	
	MailDBRecordType	testRecord4 = {
		92, 6, 3,
		{0, 0},
		{0, 0, 0, 1, 1, 0, 0 },
		"Message Four",
		"joe (Joe Sipher)",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message four."
	};
	
	MailDBRecordType	testRecord5 = {
		92, 6, 4,
		{0, 0},
		{0, 0, 0, 0, 1, 0, 0 },
		"Message Five",
		"Art",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message five."
	};
	
	MailDBRecordType	testRecord6 = {
		92, 6, 4,
		{0, 0},
		{0, 0, 0, 0, 1, 0, 0 },
		"Message Six",
		"Art",
		"Engineering@palm.com",
		"Company",
		"",
		"",
		"",
		"This is test message six."
	};
	
	MailDBRecordType	testRecord7 = {
		92, 6, 5,
		{0, 0},
		{0, 0, 0, 0, 1, 0, 0 },
		"Message Seven",
		"Art",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message seven."
	};
	
	MailDBRecordType	testRecord8 = {
		92, 6, 5,
		{0, 0},
		{0, 0, 0, 0, 1, 0, 0 },
		"Message Eight",
		"Art",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message eight."
	};
	
	MailDBRecordType	testRecord9 = {
		92, 6, 10,
		{0, 0},
		{0, 0, 0, 0, 1, 0, 0 },
		"Message Nine",
		"Art",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message nine."
	};
	
	MailDBRecordType	testRecord10 = {
		92, 6, 10,
		{0, 0},
		{0, 0, 0, 0, 1, 0, 0 },
		"Message Ten",
		"Art",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message ten."
	};
	
	MailDBRecordType	testRecord11 = {
		92, 6, 10,
		{0, 0},
		{0, 0, 0, 0, 1, 0, 0 },
		"Message Eleven",
		"Art",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message eleven."
	};
	
	MailDBRecordType	testRecord12 = {
		92, 6, 20,
		{0, 0},
		{0, 0, 0, 0, 1, 0, 0 },
		"Message Twelve",
		"Art",
		"Engineering@palm.com",
		"",
		"",
		"",
		"",
		"This is test message Twelve."
	};
*/

#ifdef __MWERKS__
#pragma const_strings reset
#endif
	
	for (i=1; i<argc; i++) {
		if (!dbP)
			sscanf(argv[i], "%lx", &dbP);
		else
			usageErr = true;
		
		}

	if (!dbP)
		dbP = FindOpenedMailDatabases ();
		
	if (!dbP)
		{
		ShlInsertText("Could not find an open mail database\n");
		return;
		}


	if (usageErr) {
		sprintf(text, "Syntax: %s <access ptr>\n", argv[0]);  
		ShlInsertText(text);
		return;
		}

	MailNewRecord(dbP, &testRecord1, &index);
	MailNewRecord(dbP, &testRecord2, &index);
	MailNewRecord(dbP, &testRecord3, &index);
	MailNewRecord(dbP, &testRecord4, &index);
	MailNewRecord(dbP, &testRecord5, &index);
//	MailNewRecord(dbP, &testRecord6, &index);
//	MailNewRecord(dbP, &testRecord7, &index);
//	MailNewRecord(dbP, &testRecord8, &index);
//	MailNewRecord(dbP, &testRecord9, &index);
//	MailNewRecord(dbP, &testRecord10, &index);
//	MailNewRecord(dbP, &testRecord11, &index);
//	MailNewRecord(dbP, &testRecord12, &index);


	ShlInsertText("New records added\n");
}


/**********************************************************************
 *	Function:		PrintMailRecord
 *
 *	Description:	Print the details of an appt record.  Includes
 *						deleted records, and the
 *						deleted, dirty, secret, & busy flags.
 *
 * Usage:			PrintMailRecord (dbP, index)
 *
 *	Revision History:
 *
 *		Name		Date		Description
 *		----		----		-----------
 *		kcr		10/23/95	display deleted records, sync-status flags
 *
 ***********************************************************************/
static void PrintMailRecord (DmOpenRef dbP, UInt16 index)
{
	char 					text[256];
	LocalID				chunk;
	UInt16					attr;
	UInt32					uniqueID;
	MemHandle				recordH;
	MailDBRecordType	record;



	DmRecordInfo (dbP, index, &attr, &uniqueID, &chunk);

	// Print record index.
	sprintf (text, "\nIndex:   %d", index);
	ShlInsertText (text);

	// Print the unique id
	sprintf (text, ",  unique id: %ld", uniqueID);
	ShlInsertText (text);
	
	if ((attr & dmRecAttrDelete) &&
		 chunk)
		ShlInsertText ("\tArchived");
	else if (attr & dmRecAttrDelete)
		ShlInsertText ("\tDeleted");
	if (attr & dmRecAttrDirty)
		ShlInsertText ("\tDirty");
	if (attr & dmRecAttrBusy)
		ShlInsertText ("\tBusy");
	if (attr & dmRecAttrSecret)
		ShlInsertText ("\tSecret");

	if (attr & dmRecAttrDelete)
		return;

	// Print the category
	sprintf (text, "\tCategory: %d", attr & dmRecAttrCategoryMask);
	ShlInsertText (text);


	if (MailGetRecord(dbP, index, &record, &recordH) != 0)
		{
		ShlInsertText("Error!");
		return;
		}

	// Print date
	sprintf(text, "\nDate:    %d/%d/%d", record.date.month,
		record.date.day, record.date.year+4);
	ShlInsertText(text);
	

	// Print the flags
	ShlInsertText ("\nFlags:   ");
	sprintf (text, "Priority: %d   ", record.flags.priority);
	ShlInsertText (text);

	if (record.flags.read)
		ShlInsertText ("Read  ");

	if (record.flags.signature)
		ShlInsertText ("Signature  ");

	if (record.flags.confirmRead)
		ShlInsertText ("Confirm Read  ");

	if (record.flags.confirmDelivery)
		ShlInsertText ("Confirm Delivery  ");
		

	if (*record.subject)
		{
		ShlInsertText ("\nSubject: ");
		ShlInsertText (record.subject);
		}
	
	if (*record.from)
		{
		ShlInsertText ("\nFrom:    ");
		ShlInsertText (record.from);
		}
	
	if (*record.to)
		{
		ShlInsertText ("\nTo:      ");
		ShlInsertText (record.to);
		}
	
	if (*record.cc)
		{
		ShlInsertText ("\nCC:      ");
		ShlInsertText (record.cc);
		}
	
	if (*record.bcc)
		{
		ShlInsertText ("\nBCC:      ");
		ShlInsertText (record.bcc);
		}
	
	if (*record.body)
		{
		ShlInsertText ("\nBody:    ");
		ShlInsertText (record.body);
		}

	ShlInsertText ("\n");


	MemHandleUnlock ((MemHandle)recordH);
}


/**********************************************************************
 * Get an record and print in Address format.
 *
 * DoMailGetRecord <access ptr> <index>
 ***********************************************************************/
static void DoMailGetRecord(int argc, Char * argv[])
{
	Boolean		usageErr = false;
	DmOpenRef	dbP=0;
	char			text[256];
	UInt16 index = 0;
	int			i;
	
	
	for (i=1; i<argc; i++) {
		if (!dbP)
			sscanf(argv[i], "%lx", &dbP);
		else
		if (!index)
			sscanf(argv[i], "%d", &index);
		else
			usageErr = true;
		
		}

	if (!dbP) usageErr = true;

	if (usageErr) {
		sprintf(text, "Syntax: %s <access ptr> <index>\n", argv[0]);  
		ShlInsertText(text);
		return;
		}

	PrintMailRecord (dbP, index);
}


/**********************************************************************
 * Get an record and print in Address format.
 *
 * DoMailGetRecord <access ptr> <index>
 *
 ***********************************************************************/
static void DoMailGetAll(int argc, Char * argv[])
{
	int					i;
	UInt16 					index = 0;
	char					text[256];
	Boolean				usageErr = false;
	DmOpenRef			dbP=0;
		
	if ( argc > 1 && !strcmp(argv[1], "?") )
		goto Help;

	for (i=1; i<argc; i++) {
		if (!dbP)
			sscanf(argv[i], "%lx", &dbP);
		else
			usageErr = true;
		}

	if (usageErr) {
		goto SyntaxOnly;
		}

	if (!dbP)
		dbP = FindOpenedMailDatabases ();
		
	if (!dbP)
		{
		ShlInsertText("Could not find an open appointment database\n");
		return;
		}


	for (index = 0; index < DmNumRecords(dbP); index++)
		PrintMailRecord (dbP, index);
	ShlInsertText("\n");

	return;


Help:
	ShlInsertText("\nPrint all records in the appointment database\n");
	ShlInsertText("and their sync flags.\n");

SyntaxOnly:
	sprintf(text, "Syntax: %s [access ptr]\n", argv[0]);  
	ShlInsertText(text);
}





/**********************************************************************
 * Test changing an appointment record.
 *
 * DoMailChangeRecord <access ptr> <index>
 ***********************************************************************/
static void DoMailChangeRecord(int argc, Char * argv[])
{
	Int16							i;
	Char							text[256];
	UInt16 							result;
	UInt16 							index = 0;
	Boolean						usageErr = false;
	MemHandle						recordH;
	DmOpenRef					dbP = 0;
	MailDBRecordType			testRecord;
	MailChangedFieldsType	changedFields = { 0,0,0,0,0,0,0,};
	
	
	* (int *) &changedFields = 0;
	for (i=1; i<argc; i++) {
		if (!dbP)
			sscanf(argv[i], "%lx", &dbP);
		else
		if (!index)
			sscanf(argv[i], "%d", &index);
		else
			usageErr = true;
		
		}

	if (!dbP) usageErr = true;

	if (usageErr) {
		sprintf(text, "Syntax: %s <access ptr> <index>\n", argv[0]);  
		ShlInsertText(text);
		return;
		}


	if (MailGetRecord(dbP, index, &testRecord, &recordH) != 0)
		{
		ShlInsertText("Error!");
		return;
		}
	
	testRecord.date.month = 12;
	changedFields.date = true;
	
	MemHandleUnlock ((MemHandle)recordH);
	
	result = MailChangeRecord (dbP, &index, &testRecord, changedFields);

	sprintf(text, "%d\n", result);
	ShlInsertText(text);
}


/**********************************************************************
 * Sort the Mail Database
 *
 * DoMailSync
 ***********************************************************************/
static void DoMailSync (int argc, Char * argv[])
{
	int					i;
	UInt16 					index = 0;
	char					text[256];
	Boolean				usageErr = false;
	DmOpenRef			dbP=0;
		
	if ( argc > 1 && !strcmp(argv[1], "?") )
		goto Help;


	for (i=1; i<argc; i++) {
		if (!dbP)
			sscanf(argv[i], "%lx", &dbP);
		else
			usageErr = true;
		}

	if (usageErr) {
		goto SyntaxOnly;
		}

	if (!dbP)
		dbP = FindOpenedMailDatabases ();
		
	if (!dbP)
		{
		ShlInsertText("Could not find an open appointment database\n");
		return;
		}


	// Send a sync notification to the app.  This only works when the
	// app is running in Emulation mode.
	MailSort (dbP);
	ShlInsertText ("\n");

	return;


Help:
	ShlInsertText("\nSend a sync notification to the app (really just sorts it).\n");

SyntaxOnly:
	sprintf(text, "Syntax: %s [access ptr]\n", argv[0]);  
	ShlInsertText(text);
}


/**********************************************************************
 * Add default date record to the database.
 *
 * DoMailDefaultData <access ptr>
 ***********************************************************************/ 
static void DoMailDefaultData(int argc, Char * argv[])
{
	Boolean		usageErr = false;
	DmOpenRef	dbP=0;
	UInt16 		index;
	char		text[256];
	int			i;

#if 0 // DOLATER -- Need to fix this up for LOCALEs.
#if LANGUAGE == LANGUAGE_FRENCH
	MailDBRecordType	testRecord1 = {
		93, 5, 19,
		{11, 0},
		{0, 0, 0, 0, priorityNormal, sentTo, 0 },
		"Bienvenue",
		"eurosupport@usr.com (U.S. Robotics)",
		"Utilisateur de Courrier de PalmPilot",
		"",
		"",
		"",
		"Utilisateur de Courrier de PalmPilot",
	"Bienvenue dans Courrier de PalmPilot!\n" 
			"\n"
	"Pour que vos messages de système de messagerie de "
	"bureau apparaissent ici, vous devez configurer "
	"HotSync sur votre bureau comme suit.\n"
			"\n"
	"1. Installez PalmPilot Desktop à partir "
	"du CD ou des disquettes de "
	"distribution.\n"
			"\n"
	"2. Sous Windows 95/NT 4.0 PC, "
	"cliquez sur l'icône de HotSync "
	"dans la barre des tâches "
	"et sélectionnez Personnaliser. Sous "
	"Windows 3.1x, cliquez deux fois "
	"sur l'icône Personnaliser du groupe "
	"de programmes de PalmPilot 2.0.\n"
			"\n"
	"3. Sélectionnez la conduite Courrier "
	"et cliquez sur le bouton Changer.\n"
			"\n"
	"4. Cochez la case Activer Courrier "
	"de PalmPilot.\n"
			"\n"
	"5. Sélectionnez le système de "
	"messagerie de bureau dans le "
	"menu déroulant Synchroniser avec.\n"
			"\n"
	"6. Entrez le nom de l'utilisateur et le "
	"Mot de passe utilisé pour vous "
	"connecter au système de messagerie "
	"de bureau.\n"
			"\n"
	"7. Cliquez sur le bouton d'aide pour "
	"connaître le paramétrage "
	"spécifique à votre système de "
	"messagerie exigé pour la "
	"configuration.\n"
			"\n"
	"8. Cliquez sur OK.\n"
			"\n"
	"9. Cliquez sur Terminé.\n"
			"\n"
	"A la prochaine synchronisation, les messages de votre boîte de réception de bureau "
	"appaîtront ici. Lisez-les, répondez, transférez-les, supprimez-les ou créez-en de nouveaux "
	"A la prochaine synchronisation, tous les résultats de ces actions apparaîtront "
	"ici et sur le système de messagerie de bureau.\n"
			"\n"
	"Profitez et appréciez Courrier de PalmPilot!\n"
	};
#elif LANGUAGE == LANGUAGE_GERMAN
	MailDBRecordType	testRecord1 = {
		93, 5, 19,
		{11, 0},
		{0, 0, 0, 0, priorityNormal, sentTo, 0 },
		"Begrüßung",
		"germantechsup@usr.com (U.S. Robotics)",
		"PalmPilot Mail-Benutzer",
		"",
		"",
		"",
		"PalmPilot Mail-Benutzer",
	"Willkommen bei PalmPilot Mail!\n"
			"\n"
	"Damit Ihre Desktop E-Mails hier erscheinen können, müssen Sie die HotSync-Anwendung "
	"auf Ihrem Desktop wie folgt konfigurieren:\n"
			"\n"
	"Installieren Sie PalmPilot Desktop von der mitgelieferten CD bzw. von Disketten.\n"
			"\n"
	"Unter Windows 95/NT 4.0 PC klicken Sie im Systemfach (rechts unten) auf das "
	"HotSync-Symbol, und wählen Sie  \"Benutzerdefiniert\".\n"
			"\n"
	"Unter Windows 3.1x doppelklicken Sie auf das Symbol \"Benutzerdefiniert\" in der "
	"Programmgruppe \"PalmPilot 2.0.\"\n"
			"\n"
	"Wählen Sie das Conduit \"Mail\", und klicken Sie auf \"Ändern\".\n"
			"\n"
	"Klicken Sie auf \"PalmPilot aktivieren\".\n"
			"\n"
	"Wählen Sie unter \"Synchronisieren mit\" Ihr Desktop E-Mail-System aus.\n"
			"\n"
	"Geben Sie den Benutzernamen und das Kennwort ein, mit denen Sie sich gewöhnlich "
	"bei Ihrem Desktop E-Mail-System anmelden.\n"
			"\n"
	"Klicken Sie auf \"Hilfe\", um weitere Einstellungen anzuzeigen, die Sie eventuell "
	"auf Ihrem Desktop E-Mail-System konfigurieren müssen.\n"
			"\n"
	"Klicken Sie auf \"OK\".\n"
			"\n"
	"Klicken Sie auf \"Fertig\".\n"
			"\n"
	"Nach dem nächsten HotSync-Vorgang werden Sie Ihre Nachrichten aus dem Desktop-Ordner "
	"\"Posteingang\" hier sehen.  Sie können nach Belieben E-Mails verfassen, lesen, "
	"beantworten und senden bzw. löschen.  Wenn Sie das nächste Mal einen HotSync "
	"ausführen, werden all diese Aktionen sowohl hier als auch auf Ihrem Desktop "
	"E-Mail-System angezeigt.\n"
			"\n"
	"Viel Spaß mit PalmPilot Mail!\n"
	};
#elif (LANGUAGE == LANGUAGE_ENGLISH) && (VENDOR == VENDOR_IBM)
	MailDBRecordType	testRecord1 = {
		93, 6, 20,
		{15, 0},
		{0, 0, 0, 0, priorityNormal, sentTo, 0 },
		"Welcome",
		"WorkPad@us.ibm.com (IBM Corp.)",
		"WorkPad Mail User",
		"",
		"",
		"",
		"WorkPad Mail User",
	"Welcome to WorkPad Mail!\n" 
	"\n"
	"In order for your desktop e-mail messages to appear here, you must "
	"configure HotSync on your desktop as follows.\n"
	"\n"
	"1. Install WorkPad Desktop from \n"
	"     the CD provided.\n"
	"2. On your Windows 95/NT 4.0 PC, \n"
	"     click the HotSync system tray \n"
	"     icon and select Custom.\n"
	"3. Select the Mail conduit and\n"
	"     click the Change button. \n"
	"4. Click on the Activate \n"
	"     WorkPad Mail checkbox.\n"
	"5. Select the desktop e-mail \n"
	"     system you use in the\n"
	"     Synchronize with drop-winDown\n"
	"     menu.\n"
	"6. Enter the User Name and\n"
	"     Password you use to log into\n"
	"     your desktop e-mail system.\n"
	"7. Click the Help button to\n"
	"     learn about other settings\n"
	"     specific to your desktop\n"
	"     e-mail system that you may\n"
	"     need to configure.\n"
	"8. Click OK.\n"
	"9. Click Done.\n"
	"\n"
	"When you synchronize, the messages from your desktop inbox will "
	"appear right here.  Read, reply, forward, delete, or create new messages.  "
	"\n"
	"The next time you synchronize, all those actions will be reflected both "
	"here on your IBM WorkPad and on your desktop e-mail system.\n"
	"\n"
	"Enjoy WorkPad Mail!"
	};
#else
	MailDBRecordType	testRecord1 = {
		94, 3, 1,
		{9, 0},
		{0, 0, 0, 0, priorityNormal, sentTo, 0 },
		"Welcome",
		"support@palm.com(3Com Corporation)",
		"Palm Mail User",
		"",
		"",
		"",
		"Palm Mail User",
		"Welcome to Palm Mail!\n"
		"\n"
		"In order for your desktop e-mail messages to appear here, you must configure HotSync on your desktop as follows.\n"
		"\n"
		"1. Install Palm Desktop organizer software on the provided CD or diskettes.\n"
		"\n"
		"2. Click Yes to the screen that asks if you want to configure your Palm Mail settings.\n"
		"\n"
		"3. Follow the steps in the Mail Setup Wizard to configure your Palm Mail.\n"
		"\n"
		"4. You can return to Mail Setup at anytime through the Start button on your Windows 95/NT desktop.  Click the Start button, highlight Programs, go to the Palm program group and highlight Mail Setup.\n"
		"\n"
		"When you synchronize, the messages from your desktop inbox will appear right here.  Read, reply, forward, delete, or create new messages.  The next time you synchronize, all those actions will be reflected both here and on your desktop e-mail system.\n"
		"\n"
		"Enjoy Palm Mail!"
	};
#endif
#endif

#if __MWERKS__
#pragma const_strings off
#endif 

	MailDBRecordType	testRecord1 = {
		94, 3, 1,
		{9, 0},
		{0, 0, 0, 0, priorityNormal, sentTo, 0 },
		"Welcome",
		"support@palm.com(3Com Corporation)",
		"Palm Mail User",
		"",
		"",
		"",
		"Palm Mail User",
		"Welcome to Palm Mail!\n"
		"\n"
		"In order for your desktop e-mail messages to appear here, you must configure HotSync on your desktop as follows.\n"
		"\n"
		"1. Install Palm Desktop organizer software on the provided CD or diskettes.\n"
		"\n"
		"2. Click Yes to the screen that asks if you want to configure your Palm Mail settings.\n"
		"\n"
		"3. Follow the steps in the Mail Setup Wizard to configure your Palm Mail.\n"
		"\n"
		"4. You can return to Mail Setup at anytime through the Start button on your Windows 95/NT desktop.  Click the Start button, highlight Programs, go to the Palm program group and highlight Mail Setup.\n"
		"\n"
		"When you synchronize, the messages from your desktop inbox will appear right here.  Read, reply, forward, delete, or create new messages.  The next time you synchronize, all those actions will be reflected both here and on your desktop e-mail system.\n"
		"\n"
		"Enjoy Palm Mail!"
	};
	
#if __MWERKS__
#pragma const_strings reset
#endif 

	for (i=1; i<argc; i++) {
		if (!dbP)
			sscanf(argv[i], "%lx", &dbP);
		else
			usageErr = true;
		
		}

	if (!dbP)
		dbP = FindOpenedMailDatabases ();
		
	if (!dbP)
		{
		ShlInsertText("Could not find an open mail database\n");
		return;
		}


	if (usageErr) {
		sprintf(text, "Syntax: %s <access ptr>\n", argv[0]);  
		ShlInsertText(text);
		return;
		}

	MailNewRecord(dbP, &testRecord1, &index);


	ShlInsertText("New records added\n");
}


/**********************************************************************
 * Support for Mail commands
 *
 * returns 0 if successfully found command
 ***********************************************************************/
int ShlDoAppCmd(int argc, Char * argv[])
{
	int i;
	ShellCmdInfoType cmd [] ={ 	
		"MailAppInfoInit", 			"msi",  	DoMailSetInfoDefaults,
		"MailNew",						"mnr",	DoMailNewRecords,
		"MailGetRecord",				"mgr",	DoMailGetRecord,
		"MailGetAll",					"md",		DoMailGetAll,
		"MailChangeRecord",			"mcr",	DoMailChangeRecord,
		"MailSync",						"ms",		DoMailSync,
		"MailDefaultData",			"mdd",	DoMailDefaultData,
		};
			
	for (i = 0; i < sizeof (cmd) / sizeof (ShellCmdInfoType); i++)
		{
		if ( (!ShlStrCmpi(argv[0], cmd[i].longName)) ||
			  (!ShlStrCmpi(argv[0], cmd[i].shortName)))
			{
			cmd[i].func (argc, argv);
			return 0;
			}
		}
	return 1;
}



#ifdef OLDWAY
/***********************************************************************
 *
 *	Copyright (c) Palm Computing 1996 -- All Rights Reserved
 *
 * PROJECT:  Pilot
 * FILE:     ShellCmdUser.c
 * AUTHOR:	 Roger Flores: Feb 26, 1996
 *
 * DESCRIPTION:
 *	  This file is where customized console shell comands are kept.
 * Currently this is a stub which the emulator calls when it doesn't
 * recognize a command.  The code here shows how to add a custom command.
 * The custom command is useless, but it does provide a template to add commands.
 * An application programmer can copy this file to their application 
 * folder and then customize it to MemHandle as many commands as they wish.
 *
 **********************************************************************/

#include <Pilot.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ShellCmd.h"

typedef void ShellCmdFuncType (int argc, Char * argv[]);
typedef ShellCmdFuncType * ShellCmdFuncPtr;
typedef struct {
	Char *				longName;
	Char *				shortName;
	ShellCmdFuncPtr	func;
} ShellCmdInfoType;



/***********************************************************************
 *
 * FUNCTION:    DoAppCommand
 *
 * DESCRIPTION: Execute a user defined command for the appliation. 
 *					 (It currently adds one to a number as an example)
 *
 *					 Syntax: UserCommand num
 *
 * PARAMETERS:  argc - number of arguments
 *              argv - argument list
 *
 * RETURNED:    false if the command was a ui command.
 *
 ***********************************************************************/
static void DoAppCommand (int argc, Char * argv[])
{
	int i;
	char text[256];
	Int16 num = 0;
	Boolean usageErr = false;

	// Parse the arguments passed.
	if ( argc > 1 && !strcmp(argv[1], "?") )
		goto Help;

	if (argc < 2) usageErr = true;
	for (i=1; i<argc; i++)
		{
		if (isxdigit(argv[i][0]))
			sscanf(argv[i], "%d", &num);
		else
			usageErr = true;
		}

	if (usageErr) {
		goto SyntaxOnly;
		}

	// Perform the command here
	sprintf(text, "\n%d + 1 = %d\n", num, num + 1);
	ShlInsertText(text);

	return;


	// Dislay help and or syntax.
Help:
	ShlInsertText("\nExecute a user defined command. (adds one to a number)\n"
		"You can customize this in the ShellCmdApp.cp file\n");
SyntaxOnly:
	sprintf(text, "\nSyntax: %s [number]\n", argv[0]);
	ShlInsertText(text);
	return;
}


/***********************************************************************
 *
 * FUNCTION:    ShlDoAppCmd
 *
 * DESCRIPTION: This routine check if a command is a user defined shell 
 *					 command for the application.  If so is the command is executed.
 *
 * PARAMETERS:  argc - number of arguments
 *              argv - argument list
 *
 * RETURNED:    false if the command was handled.
 *
 ***********************************************************************/
int ShlDoAppCmd (int argc, Char * argv[])
{
	int i;
	ShellCmdInfoType cmd [] ={ 
		// Each command should have a customized line below	
		"UserComand", 		"uc",  	DoAppCommand,
		};
			
	for (i = 0; i < sizeof (cmd) / sizeof (ShellCmdInfoType); i++)
		{
		if ( (!ShlStrCmpi(argv[0], cmd[i].longName)) ||
			  (!ShlStrCmpi(argv[0], cmd[i].shortName)))
			{
			cmd[i].func (argc, argv);
			return 0;
			}
		}
	return 1;

}

#endif
