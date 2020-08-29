/**************************************************************************************************
 *
 * Copyright (c) 2007 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 *************************************************************************************************/
/** @ingroup IRCommunication
 *
**/

/** @file Common.h
 * This file contains utility macros and functions for the IR Communication sample applications
**/

/** @code
 *
**/
/*@{*/


/**************************************************************************************************
 *
 *	File:			Common.h
 *
 *	Description:	Some useful macros and functions
 *
 *	Version:		1.0 - Initial Revision (03/04/03)
 *
 *************************************************************************************************/

#ifndef COMMON_H_
#define COMMON_H_

/**************************************************************************************************
 *
 *	Defines
 *
 *************************************************************************************************/

#define FrmGetPtr(formP, id)			FrmGetObjectPtr( formP, FrmGetObjectIndex( formP, id ) )

#define FrmHideID(formP, id)			FrmHideObject( formP, FrmGetObjectIndex( formP, id ) )

#define FrmShowID(formP, id)			FrmShowObject( formP, FrmGetObjectIndex( formP, id ) )

#define FrmGetPosition( formP, id, xP, yP )														  \
										FrmGetObjectPosition( formP,							  \
															  FrmGetObjectIndex( formP, id ),	  \
															  xP,								  \
															  yP );

#define FrmGetBounds( formP, id, boundsP )														  \
										FrmGetObjectBounds( formP,								  \
															FrmGetObjectIndex( formP, id ),		  \
															boundsP );

#define FrmSetBounds( formP, id, boundsP )														  \
										FrmSetObjectBounds( formP,								  \
															FrmGetObjectIndex( formP, id ),		  \
															boundsP );

#define MisalignedReadBEUInt16( pointer, offset )												  \
										(   ( (UInt16)( *(   ( (UInt8 *)( pointer ) )			  \
									                       + offset								  \
													       + sizeof( UInt8 ) ) ) )				  \
									      |	( ( (UInt16)( *(   ( (UInt8 *)( pointer ) )			  \
														     + offset ) ) ) << 8 ) )

#define MisalignedReadBEUInt32( pointer, offset )												  \
										(   ( (UInt32)( MisalignedReadBEUInt16(					  \
																(   ( (UInt8 *)( pointer ) )	  \
										                          + sizeof( UInt16 ) ),			  \
																offset ) ) )					  \
										  |	(    ( (UInt32)( MisalignedReadBEUInt16( pointer,	  \
																				  offset ) ) )	  \
											  << 16 ) )

#define Swap16( n )						(   ( ( ( (UInt16)( n ) ) << 8 ) & 0xFF00 ) 			  \
									      | ( ( ( (UInt16)( n ) ) >> 8 ) & 0x00FF ) )
					

#define Swap32( n )						(   ( ( ( (UInt32)( n ) ) << 24 ) & 0xFF000000 ) 		  \
										  |	( ( ( (UInt32)( n ) ) <<  8 ) & 0x00FF0000 )		  \
										  | ( ( ( (UInt32)( n ) ) >>  8 ) & 0x0000FF00 ) 		  \
										  | ( ( ( (UInt32)( n ) ) >> 24 ) & 0x000000FF ) )
										

#define COMMON_CHAR_NULL				'\0'

#define COMMON_SHIFT_8					( 8 )

#define COMMON_MASK_8_0XFF				( 0xFF )


// IrDA middleware states
#define COMMON_STATE_START				( 0 )
#define COMMON_STATE_FRAME_START		( 1 )
#define COMMON_STATE_XBOF				( 2 )
#define COMMON_STATE_FRAME_END			( 3 )
#define COMMON_STATE_PAYLOAD			( 4 )
#define COMMON_STATE_IR_ESC				( 5 )

// IrDA codes
#define COMMON_IRDA_BOF					( 0xC0 )
#define COMMON_IRDA_EXTRA_BOF			( 0xFF )
#define COMMON_IRDA_EOF					( 0xC1 )
#define COMMON_IRDA_ESC					( 0x7D )
#define COMMON_IRDA_ESC_XOR				( 0x20 )

// Size in bytes
#define COMMON_SIZEOF_NULL				( 1 )

#define COMMON_IRDA_SIZEOF_BOF			( 1 )
#define COMMON_IRDA_SIZEOF_EOF			( 1 )
#define COMMON_IRDA_SIZEOF_CRC			( 2 )
#define COMMON_IRDA_SIZEOF_ESC			( 1 )

#define COMMON_IRDA_FRAME_SIZE_MAX		( 1024 )	//bytes for the twice the max IrDA frame size


#define COMMON_IRDA_ADDR_BROADCAST_CMD	( 0xFF )
#define COMMON_IRDA_CONTROL_TEST_CMD	( 0xF3 )

#define COMMON_INIT_FCS					( 0xFFFF )  /* Initial FCS value */
#define COMMON_GOOD_FCS					( 0xF0B8 )  /* Good final FCS value */

#define COMMON_BUFFER_LEN				( 81 )
#define COMMON_BUFFER_HEX_LEN			( 241 )						


/**************************************************************************************************
 *
 * Function Prototypes
 *
 *************************************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

Err SetFieldTextFromStr( FieldType		*fieldPtr,
						 Char			*stringPtr,
						 Boolean		 redraw );


#ifdef __cplusplus
}
#endif

#endif COMMON_H_

/* EOF *******************************************************************************************/

/*@}*/
