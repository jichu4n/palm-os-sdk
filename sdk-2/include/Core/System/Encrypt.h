/*******************************************************************
 * 							 Pilot Software  
 *
 *	Copyright (c) 1994-1997, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------
 * FileName:  
 *		Encrypt.h
 *
 * Description:
 *		Equates for encryption/digestion routines in pilot
 *
 * History:
 *		7/31/96	RM	- Created by Ron Marianetti   
 *
 *******************************************************************/

#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__





/************************************************************
 * Function Prototypes
 *************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Perform reversible encryption or decryption of 8 byte string in
//  srcP using 8 byte key keyP. Place 8 byte result in dstP.
Err	EncDES(BytePtr srcP, BytePtr keyP, BytePtr dstP, Boolean encrypt)
			SYS_TRAP(sysTrapEncDES);


// Digest a string of bytes and produce a 128 bit result using
//   the MD4 algorithm.
Err	EncDigestMD4(BytePtr strP, Word strLen, Byte digestP[16])
			SYS_TRAP(sysTrapEncDigestMD4);


// Digest a string of bytes and produce a 128 bit result using
//   the MD5 algorithm.
Err	EncDigestMD5(BytePtr strP, Word strLen, Byte digestP[16])
			SYS_TRAP(sysTrapEncDigestMD5);



#ifdef __cplusplus 
}
#endif



#endif	//__ENCRYPT_H__

