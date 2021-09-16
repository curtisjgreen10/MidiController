/*
 * gData.h
 *
 *  Created on: Mar 4, 2021
 *      Author: cjgree13
 */

#ifndef INCLUDE_MIDIGLOBALDATA_H_
#define INCLUDE_MIDIGLOBALDATA_H_

#include "mixqueue.h"
#include "timer.h"

namespace MidiGlobalData
{
	extern MixQueue* queue;
	extern Timer *timer;
	extern Audio *audioCtrl;

	void SetMicInput(bool input);
	bool GetMicInput();

	void SetRecording(bool input);
	bool GetRecording();

	void SetDrumVol(double vol, int drumNum);
	double GetDrumVol(int drumNum);
}

#endif /* INCLUDE_MIDIGLOBALDATA_H_ */
