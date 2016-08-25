/*******************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * FileName:  
 *		Password.h
 *
 * Description:
 *		Password include file
 *
 * History:
 *		4/1/95 - created by Roger Flores
 *
 *******************************************************************/

 #ifndef __PASSWORD_H__
 #define __PASSWORD_H__



#define pwdLength						32
#define pwdEncryptionKeyLength	64

Boolean 	PwdExists()
				SYS_TRAP(sysTrapPwdExists);
				
Boolean 	PwdVerify(CharPtr string)
				SYS_TRAP(sysTrapPwdVerify);
				
void 		PwdSet(CharPtr oldPassword, CharPtr newPassword)
				SYS_TRAP(sysTrapPwdSet);
				
void 		PwdRemove(void)
				SYS_TRAP(sysTrapPwdRemove);
				
				
#endif // __PASSWORD_H__





