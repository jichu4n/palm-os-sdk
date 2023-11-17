/****************************************************************************
 * File        : Wave.h
 * Date        : 8-30-00
 * Description : 
 ****************************************************************************/
#ifndef _WAVE_H_
#define _WAVE_H_

/*------------------------------------------------------------------------
 * Hard code to first volume for this example, actual apps need to iterate
 * through all the volumes.
 *----------------------------------------------------------------------*/
#define VOLUME_REF_NUM      1       

void WaveInit  (void);
Err  WaveOpen  (char *filename);
Err  WaveCreate(char *filename);
void WaveClose (void);
Err  WavePlay  (void);
Err  WavePause (void);
Err  WaveRecord(void);
Err  WaveStop  (void);
Err  WaveSeek  (UInt16 percent);

#endif


