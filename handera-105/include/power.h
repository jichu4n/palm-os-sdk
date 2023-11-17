/****************************************************************************
 *
 *      Copyright (c) 2001, TRG, All Rights Reserved
 *
 *---------------------------------------------------------------------------
 * FileName:
 *              Power.h
 *
 * Description:
 *              Power status.
 *
 *
 ****************************************************************************/

#ifndef __POWER_H__
#define __POWER_H__

/*------------------------------------------------------------------------
 * Bits returned by HwrDockStatus()
 *----------------------------------------------------------------------*/
#define	hwrDockStatusCharging              0x0008   // internal power cells are recharging

UInt16	HwrDockStatus(void)
           SYS_TRAP(sysTrapHwrDockStatus);

#endif //#ifndef __POWER_H__
