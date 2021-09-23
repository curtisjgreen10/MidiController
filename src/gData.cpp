/*
 * gData.cpp
 *
 *  Created on: Mar 4, 2021
 *      Author: cjgree13
 */
#include "gData.h"
#include <mutex>

std::mutex micLock;
std::mutex recLock;
std::mutex drumLock;

namespace MidiGlobalData
{
	MixQueue* queue;
	Timer *timer;
	Audio *audioCtrl;
	bool micInput;		/*!< true if mic is to be routed to output */
	double drum1vol;		/*!< current volume of drum 1 */
	double drum2vol;		/*!< current volume of drum 2 */
	double drum3vol;		/*!< current volume of drum 3 */
	double drum4vol;		/*!< current volume of drum 4 */
//	double cymbal1vol;	/*!< current volume of cymbal 1 */
//	double cymbal2vol;	/*!< current volume of cymbal 2 */
//	double cymbal3vol;	/*!< current volume of cymbal 3 */
//	double cymbal4vol;	/*!< current volume of cymbal 4 */
	bool recording; /*!< true if software is recording */



	bool SetMicInput(bool input)
	{
		bool lockStatus = micLock.try_lock();
		if(lockStatus)
		{
			micInput = input;
			micLock.unlock();
		}
		return lockStatus;
	}

	bool GetMicInput()
	{
		micLock.lock();
		return micInput;
		micLock.unlock();
	}

	bool SetRecording(bool rec)
	{
		bool lockStatus = recLock.try_lock();
		if(lockStatus)
		{
			micInput = rec;
			recLock.unlock();
		}
		return lockStatus;
	}

	bool GetRecording()
	{
		recLock.lock();
		return recording;
		recLock.unlock();
	}

	bool SetDrumVol(double vol, int drumNum)
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

	double GetDrumVol(int drumNum)
	{
		drumLock.lock();
		switch(drumNum)
		{
			case 1:
				return drum1vol;
				break;
			case 2:
				return drum2vol;
				break;
			case 3:
				return drum3vol;
				break;
			case 4:
				return drum4vol;
				break;
			default:
				return 0;
		}
		drumLock.lock();
	}
}
