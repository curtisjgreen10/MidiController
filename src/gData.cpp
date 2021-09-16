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



	void SetMicInput(bool input)
	{
		micLock.lock();
		micInput = input;
		micLock.unlock();
	}

	bool GetMicInput()
	{
		micLock.lock();
		return micInput;
		micLock.unlock();
	}

	void SetRecording(bool rec)
	{
		recLock.lock();
		recording = rec;
		recLock.unlock();
	}

	bool GetRecording()
	{
		recLock.lock();
		return recording;
		recLock.unlock();
	}

	void SetDrumVol(double vol, int drumNum)
	{
		drumLock.lock();
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
