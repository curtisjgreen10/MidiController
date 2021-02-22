/***********************************************************************
 * testaudio.h
 *  
 *    Audio Library
 *
 *
 * Toby Opferman Copyright (c) 2003
 *
 ***********************************************************************/


#ifndef __WAVELIB_H__
#define __WAVELIB_H__


typedef PVOID HWAVELIB;




HWAVELIB WaveLib_Init(PCHAR pAudioFile, BOOL bPause);
void WaveLib_UnInit(HWAVELIB hWaveLib);
void WaveLib_Pause(HWAVELIB hWaveLib, BOOL bPause);


#endif




