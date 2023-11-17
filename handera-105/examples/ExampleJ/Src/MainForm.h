/******************************************************************************
 *
 * File: MainForm.h
 *
 * Project : Example J
 *
 *****************************************************************************/
#ifndef _MAINFORM_H_
#define _MAINFORM_H_

#include <PalmOS.h>

Boolean MainFormHandleEvent          (EventPtr eventP);
void    MainFormHandleVolumeMounted  (Boolean draw);
void    MainFormHandleVolumeUnmounted(Boolean draw);

#endif


