/******************************************************************************
 *
 * File: wave.c
 *
 * Project : Example 7
 *
 *****************************************************************************/

#include <PalmOS.h>
#include <vfsmgr.h>
#include "Audio.h"
#include "Wave.h"

static Err WriteCallback(void *dataP, UInt32 offset, UInt32 *sizeP, void *userDataP);
static Err ReadCallback (void *dataP, UInt32 offset, UInt32 *sizeP, void *userDataP);

static FileRef         fd;
static AudioFormatType format;    //format of current wave file.
static Boolean         wave_open;

/*--------------------------------------------------------------------------
 * Function    : WaveInit()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
void WaveInit(void)
{
    wave_open = false;
}

/*--------------------------------------------------------------------------
 * Function    : WaveOpen()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
Err WaveOpen(char *filename)
{
    Err retval;
    
    if ((retval = VFSFileOpen(VOLUME_REF_NUM, filename, vfsModeRead, &fd)) != errNone)
        return(retval);

    retval = AudioOpenWave(&format, ReadCallback, &fd);

    if (retval != 0)
        VFSFileClose(fd);
    else
        wave_open = true;

    return(retval);    
}

/*--------------------------------------------------------------------------
 * Function    : WaveClose()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
void WaveClose(void)
{
    if (!wave_open)
        return;

    VFSFileClose(fd);
    AudioCloseWave();
    wave_open = false;
}    

/*--------------------------------------------------------------------------
 * Function    : WaveCreate()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
Err WaveCreate(char *filename)
{
    Err             retval;
    AudioFormatType record_format;    //format of current wave file.

/*------------------------------------------------------------------------
 * Currently, this is the only format that is supported.
 *----------------------------------------------------------------------*/
    record_format.samplesPerSecond = 8000;
    record_format.bitsPerSample    = 8;
    record_format.channels         = 1;

    if ((retval = VFSFileOpen(VOLUME_REF_NUM, filename, vfsModeReadWrite | vfsModeCreate | vfsModeTruncate, &fd)) != errNone)
        return(retval);

    retval = AudioCreateWave(&record_format, WriteCallback, &fd);

    if (retval != 0)
        VFSFileClose(fd);
    else
        wave_open = true;

    return(retval);    
}


/*--------------------------------------------------------------------------
 * Function    : WavePlay()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
Err WavePlay(void)
{
    return(AudioPlayData());
}

/*--------------------------------------------------------------------------
 * Function    : WavePause()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
Err WavePause(void)
{
    return(AudioPause());
}


/*--------------------------------------------------------------------------
 * Function    : WaveRecord()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
Err WaveRecord(void)
{
    return(AudioRecordData());
}

/*--------------------------------------------------------------------------
 * Function    : WaveStop()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
Err WaveStop(void)
{
    Err retval;
    
    if ((retval = AudioPause()) == errNone)
        return(WaveSeek(0));
    return(retval);    
}

/*--------------------------------------------------------------------------
 * Function    : WaveSeek()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
Err WaveSeek(UInt16 percent)
{
    return(AudioSeekPercent(percent));
}    


/*--------------------------------------------------------------------------
 * Function    : WriteCallback()
 * Description : 
 * Params      : userDataP - points to file handle
 * Returns     : 
 *--------------------------------------------------------------------------*/
static Err WriteCallback(void *dataP, UInt32 offset, UInt32 *sizeP, void *userDataP)
{
    Err    retval = 0;

    retval = VFSFileSeek(*((FileRef *)userDataP), vfsOriginBeginning, offset);

    if ((retval == errNone) || (retval == vfsErrFileEOF))
        retval = VFSFileWrite(*((FileRef *)userDataP), *sizeP, dataP, sizeP);

    return(retval);
}    

/*--------------------------------------------------------------------------
 * Function    : ReadCallback()
 * Description : 
 * Params      : 
 * Returns     : 
 *--------------------------------------------------------------------------*/
static Err ReadCallback(void *dataP, UInt32 offset, UInt32 *sizeP, void *userDataP)
{
    Err    retval = 0;

    if ((retval = VFSFileSeek(*((FileRef *)userDataP), vfsOriginBeginning, offset)) != errNone)
        return(retval);

    retval = VFSFileRead(*((FileRef *)userDataP), *sizeP, dataP, sizeP);

    return(retval);
}    




