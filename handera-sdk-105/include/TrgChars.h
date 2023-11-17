/*******************************************************************
 *
 * Copyright (c) 2001 TRG, All Rights Reserved
 *
 * FileName:
 *		TRGChars.h
 *
 * Description:
 *	Extra character and keyboard bitmap definitions used
 *      on TRG devices.
 *
 * History:
 *      11-21-00  -- original version
 *
 ********************************************************************/
#ifndef __TRG_CHARS_H__
#define __TRG_CHARS_H__

#define TRGKeyFtrNum            20

// this feature provides a bitmap of the extra keys that are available
// on this unit that may be returned from KeyCurrentState()
#define _TRGKeyFeaturePresent(keymapPtr) \
        (Boolean)(FtrGet(TRGSysFtrID, TRGKeyFtrNum, keymapPtr) == 0)

/*-------------------------------------------------------------------
 * Additional definition of bit field returned from KeyCurrentState,
 * see KeyMgr.h
 *------------------------------------------------------------------*/
#define keyBitJogUp     0x1000
#define keyBitJogDown   0x2000
#define keyBitJogPush   0x4000
#define keyBitAux       0x8000

/*-------------------------------------------------------------------
 * Additional character definitions corresponding to additional hard
 * buttons on StarKist.  See Chars.h
 *------------------------------------------------------------------*/
#define vchrTrgJogUp               (vchrCFlashMin + 4)
#define vchrTrgJogDown             (vchrCFlashMin + 5)
#define vchrTrgJogPush             (vchrCFlashMin + 6)
#define vchrTrgAux                 (vchrCFlashMin + 7)

/*-------------------------------------------------------------------
 * Additional virtual characters for extra silk buttons on StarKist
 * See Chars.h
 *------------------------------------------------------------------*/
#define vchrTRGVolume              (vchrCFlashMin + 3)
#define vchrSilkToggle             (vchrCFlashMin + 10)


#endif  // #ifndef __TRG_CHARS_H__

