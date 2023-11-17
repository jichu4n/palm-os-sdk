/****************************************************************************
 *
 *      Copyright (c) 2001, TRG, All Rights Reserved
 *
 *---------------------------------------------------------------------------
 * FileName:
 *              Audio.h
 *
 * Description:
 *              Audio API definitions.
 *
 *
 ****************************************************************************/

#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Trg.h"

#define TRGAudioFtrNum           5

/*--------------------------------------------------------------------------
 * Audio result codes
 * (oemErrorClass is reserved for OEM/Licensees).
 *-------------------------------------------------------------------------*/
#define audioErrorClass               (oemErrorClass | 0x500)

#define audioErrUnimplemented         (audioErrorClass | 0)    // function not implemented (on this hardware)
#define audioErrBadParam              (audioErrorClass | 1)    // invalid parameter
#define audioErrInvalidData           (audioErrorClass | 2)    // bad wave data
#define audioErrUnsupportedFormat     (audioErrorClass | 3)    // unsupported play/record format

/*---------------------------------------------------------------------------
 * Extension Installed
 *--------------------------------------------------------------------------*/
#define _TRGAudioFeaturePresent(version) \
        (Boolean)(FtrGet(TRGSysFtrID, TRGAudioFtrNum, version) == 0)


/*---------------------------------------------------------------------------
 * Format structure for playing and recording PWM data
 *--------------------------------------------------------------------------*/
typedef struct AudioFormatType 
{
    UInt16 samplesPerSecond; // TRGpro < 33kHz, HandEra 330 <= 44kHz
    UInt16 bitsPerSample;    // only 8 bit is supported
    UInt16 channels;         // only 1 channel is supported
    UInt32 data_size;        // only needed for AudioOpenRawData() in play mode.
    UInt8  reserved[4];
} AudioFormatType;

/*---------------------------------------------------------------------------
 * enum for audioModeType
 *--------------------------------------------------------------------------*/
typedef enum AudioModeType
{
    audioPlayMode,
    audioRecordMode
} AudioModeType;

/*---------------------------------------------------------------------------
 * Defines for AudGetSupportedFeatures features bitmap
 *--------------------------------------------------------------------------*/
#define audioFtrPlayWave    0x0001
#define audioFtrAdjVolume   0x0002
#define audioFtrDTMF        0x0004
#define audioFtrRecordWave  0x0008

/*---------------------------------------------------------------------------
 * min and max volume levels
 *--------------------------------------------------------------------------*/
#define audioVolumeMax 255
#define audioVolumeMin 0

/*---------------------------------------------------------------------------
 * wave playing progress events
 *--------------------------------------------------------------------------*/
#define audioProgressEvent  ((eventsEnum)(firstUserEvent-1))

// progress is stored in the following fields of a generic eventType
// datum[0]: percent complete
// datum[1]: minutes
// datum[2]: seconds
// datum[3]: 1/100ths of second
typedef struct AudioProgressType
{
    eventsEnum     eType;
    Boolean        reserved;
    UInt8          percent;
    Int16          minutes;
    Int16          seconds;
} AudioProgressType;


#define  trgMinAudFunction               0x0500
#define  trgAudGetSupportedFeatures      (trgMinAudFunction +  0)
#define  trgAudGetMasterVolume           (trgMinAudFunction +  1)
#define  trgAudSetMasterVolume           (trgMinAudFunction +  2)
#define  trgAudGetMute                   (trgMinAudFunction +  3)
#define  trgAudSetMute                   (trgMinAudFunction +  4)
#define  trgAudPlayDTMFChar              (trgMinAudFunction +  5)
#define  trgAudPlayDTMFStr               (trgMinAudFunction +  6)
#define  trgAudPlayData                  (trgMinAudFunction +  7)
#define  trgAudRecordData                (trgMinAudFunction +  8)
#define  trgAudPause                     (trgMinAudFunction +  9)
#define  trgAudOpenWaveOld               (trgMinAudFunction + 10)
#define  trgAudCloseWave                 (trgMinAudFunction + 11)
#define  trgAudVolumeDlg                 (trgMinAudFunction + 12)
#define  trgAudSeek                      (trgMinAudFunction + 13)
#define  trgAudSeekPercent               (trgMinAudFunction + 14)
#define  trgAudTell                      (trgMinAudFunction + 15)
#define  trgAudTellPercent               (trgMinAudFunction + 16)
#define  trgAudOpenRawDataOld            (trgMinAudFunction + 17)
#define  trgAudCloseRawData              (trgMinAudFunction + 18)
#define  trgAudCreateWave                (trgMinAudFunction + 19)
#define  trgAudCreateRawData             (trgMinAudFunction + 20)
#define  trgAudOpenWave                  (trgMinAudFunction + 21)
#define  trgAudOpenRawData               (trgMinAudFunction + 22)
#define  trgMaxAudFunction               (trgMinAudFunction + 22)

/********************************************************************
 *              Audio API Prototypes
 ********************************************************************/

/*---------------------------------------------------------------------------
 * function prototypes for callbacks
 *--------------------------------------------------------------------------*/
typedef Err	(*AudioReadProcPtr) 
				(void *dataP, UInt32 offset, UInt32 *sizeP, void *userDataP);

typedef Err (*AudioWriteProcPtr)
                (void *dataP, UInt32 offset, UInt32 *sizeP, void *userDataP);

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
 * get features support on this hardware
 *--------------------------------------------------------------------------*/
extern Err AudioGetSupportedFeatures(UInt32 *features)
                    EXT_TRAP(trgAudGetSupportedFeatures);

/*---------------------------------------------------------------------------
 * Display volume dlg (pass NULL for default title)
 *--------------------------------------------------------------------------*/
extern Err AudioVolumeDlg(Char *title)
                    EXT_TRAP(trgAudVolumeDlg);

/*---------------------------------------------------------------------------
 * Get master volume (0-255) 
 *--------------------------------------------------------------------------*/
extern Err AudioGetMasterVolume(UInt8 *volume)
                    EXT_TRAP(trgAudGetMasterVolume);

/*---------------------------------------------------------------------------
 * Set master volume (0-255)
 *--------------------------------------------------------------------------*/
extern Err AudioSetMasterVolume(UInt8 volume)
                    EXT_TRAP(trgAudSetMasterVolume);

/*---------------------------------------------------------------------------
 * Get mute status
 *--------------------------------------------------------------------------*/
extern Err AudioGetMute(Boolean *mute)
                    EXT_TRAP(trgAudGetMute);

/*---------------------------------------------------------------------------
 * Set mute status
 *--------------------------------------------------------------------------*/
extern Err AudioSetMute(Boolean mute)
                    EXT_TRAP(trgAudSetMute);

/*---------------------------------------------------------------------------
 * Play a DTMF tone
 *--------------------------------------------------------------------------*/
extern Err AudioPlayDTMFChar(char ascChar, Int16 toneLength)
                    EXT_TRAP(trgAudPlayDTMFChar);

/*---------------------------------------------------------------------------
 * Play a DTMF sequence
 *--------------------------------------------------------------------------*/
extern Err AudioPlayDTMFStr(char *ascStr, Int16 toneLength, Int16 toneGap)
                    EXT_TRAP(trgAudPlayDTMFStr);


/*---------------------------------------------------------------------------
 * Open a wave file and write out the header information and prepare to
 * record new data
 * NOTE: only one wave file may be open at a time
 *--------------------------------------------------------------------------*/
extern Err AudioCreateWave(AudioFormatType  *dataFormat,
                         AudioWriteProcPtr  writeData,
                         void              *userData)
                    EXT_TRAP(trgAudCreateWave);

/*---------------------------------------------------------------------------
 * Open a wave file and read in the header information and prepare to play
 * the data
 * NOTE: only one wave file may be open at a time
 *--------------------------------------------------------------------------*/
extern Err AudioOpenWave(AudioFormatType  *dataFormat,
                         AudioReadProcPtr  getData,
                         void             *userData)
                    EXT_TRAP(trgAudOpenWave);

/*---------------------------------------------------------------------------
 * Close the file and update the header information if needed.
 *--------------------------------------------------------------------------*/
extern Err AudioCloseWave(void)
                    EXT_TRAP(trgAudCloseWave);

/*---------------------------------------------------------------------------
 * Play PWM/Wave data
 *--------------------------------------------------------------------------*/
extern Err AudioPlayData(void)
                    EXT_TRAP(trgAudPlayData);

/*---------------------------------------------------------------------------
 * Record PWM data
 *--------------------------------------------------------------------------*/
extern Err AudioRecordData(void)
                    EXT_TRAP(trgAudRecordData);

/*---------------------------------------------------------------------------
 * Stop/Pause play or record
 *--------------------------------------------------------------------------*/
extern Err AudioPause(void)
                    EXT_TRAP(trgAudPause);

extern Err AudioSeek(UInt32 tenthsOfSeconds)
                    EXT_TRAP(trgAudSeek);
                    
extern Err AudioSeekPercent(UInt16 percent)
                    EXT_TRAP(trgAudSeekPercent);

extern Err AudioTell(UInt32 *tenthsOfSeconds)
                    EXT_TRAP(trgAudTell);
                    
extern Err AudioTellPercent(UInt16 *percent)
                    EXT_TRAP(trgAudTellPercent);
                    
extern Err AudioCreateRawData(AudioFormatType  *dataFormat,
                              AudioWriteProcPtr writeData,
                              void             *userData)
                    EXT_TRAP(trgAudCreateRawData);

extern Err AudioOpenRawData(AudioFormatType  *dataFormat,
                            AudioReadProcPtr  getData,
                            void             *userData)
                    EXT_TRAP(trgAudOpenRawData);

extern Err AudioCloseRawData(void)
                    EXT_TRAP(trgAudCloseRawData);

extern Err AudioOpenWaveOld(AudioModeType mode, AudioFormatType *dataFormat,
                            AudioReadProcPtr getData, AudioWriteProcPtr writeData,
                            void *userData)
                    EXT_TRAP(trgAudOpenWaveOld);

extern Err AudioOpenRawDataOld(AudioModeType mode, AudioFormatType *dataFormat,
                            AudioReadProcPtr getData, AudioWriteProcPtr writeData,
                            void *userData)
                    EXT_TRAP(trgAudOpenRawDataOld);

#ifdef __cplusplus 
}
#endif


#endif  // __AUDIO_H__
