/*
 * gData.cpp
 *
 *  Created on: Mar 4, 2021
 *      Author: cjgree13
 */
#include "gData.h"

bool MidiGlobalData::SetMicInput(bool input)
{
	bool lockStatus = micLock.try_lock();
	if(lockStatus)
	{
		micInput = input;
		micLock.unlock();
	}
	return lockStatus;
}

bool MidiGlobalData::GetMicInput(bool * input)
{
	bool lockStatus = micLock.try_lock();
	if(lockStatus)
	{
		*input = micInput;
		micLock.unlock();
	}
	return lockStatus;
}

bool MidiGlobalData::SetRecording(bool rec)
{
	bool lockStatus = recLock.try_lock();
	if(lockStatus)
	{
		recording = rec;
		recLock.unlock();
	}
	return lockStatus;
}

bool MidiGlobalData::GetRecording(bool * rec)
{
	bool lockStatus = recLock.try_lock();
	if(lockStatus)
	{
		*rec = recording;
		recLock.unlock();
	}
	return lockStatus;
}

bool MidiGlobalData::SetDrumVol(double vol, int drumNum)
{
	bool lockStatus = drumLock.try_lock();
	if(lockStatus)
	{
		switch(drumNum)
		{
			case 1:
				drum1vol = vol;
				break;
			case 2:
				drum2vol = vol;
				break;
			case 3:
				drum3vol = vol;
				break;
			case 4:
				drum4vol = vol;
				break;
		}
		drumLock.lock();
	}
	return lockStatus;
}

bool MidiGlobalData::GetDrumVol(int drumNum, double * vol)
{
	bool lockStatus = drumLock.try_lock();
	if (lockStatus)
	{
		switch(drumNum)
		{
			case 1:
				*vol = drum1vol;
				break;
			case 2:
				*vol = drum2vol;
				break;
			case 3:
				*vol = drum3vol;
				break;
			case 4:
				*vol = drum4vol;
				break;
			default:
				return 0;
		}
		drumLock.lock();
	}
	return lockStatus;
}

