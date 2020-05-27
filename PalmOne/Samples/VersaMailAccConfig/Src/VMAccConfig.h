/**************************************************************************************************
 *
 * Copyright (c) 2004-2007 Palm,Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/

/** @defgroup VersaMailAccConfig VersaMailAccConfig Sample Code Library
 *
 *  Sample code for the VersaMail Account Configuration Applications 
 *  
 *
 *
 * FileName:
 *	 VMAccConfig.h
 *   
 * @version Version 1.0 	- Initial Revision (08/21/03)
 * @version Version 1.1		- Added "About" Box and 5-way nav support (2/20/07), compiles under PODS and GCC
 * 
 *
 * @{
 * @}
 */

/** @ingroup VersaMailAccConfig
 *
 */
 
/**
* 																		  																  
* @file VMAccConfig.h														  												  
*																		  
* @brief Sample code for the VersaMail Account Configuration Applications 
*        
*																		
*/


 
#ifndef VERSASET_H_
#define VERSASET_H_

/*********************************************************************
 * Internal Structures
 *********************************************************************/


/*********************************************************************
 * Global variables
 *********************************************************************/


/*********************************************************************
 * Internal Constants
 *********************************************************************/

#define appFileCreator			'STRT'
#define appName					"VMAccConfig"
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01

#define	INCOMINGSERVER			0x00
#define	USERNAME				0x01
#define	USEENCRYPTEDPASSWORD	0x02
#define	PASSWORD				0x03
#define	EMAILADDRESS			0x04
#define	OUTGOINGSERVER			0x05
#define	TITLE					0x06
#define	INCOMINGPORT			0x07
#define	ROOTMAILBOX				0x08
#define	OUTGOINGPORT			0x09
#define	REPLYTO					0x0A
#define	APN						0x0B
#define	USESMTP					0x0C
#define SAMCREATORID            0x0D
#define SAMTYPEID               0x0E
#define USEINCOMINGSSL			0x0F
#define USEOUTGOINGSSL			0x10

#define	ACCSLOT					0x00

#endif /* VERSASET_H_ */
