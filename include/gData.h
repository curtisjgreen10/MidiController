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
#include <mutex>

class MidiGlobalData
{

private:
	std::mutex micLock;
	std::mutex recLock;
	std::mutex drumLock;
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

public:
	MixQueue* queue;
	Timer *timer;
	Audio *audioCtrl;

	bool SetMicInput(bool input);
	bool GetMicInput(bool * input);

	bool SetRecording(bool input);
	bool GetRecording(bool * rec);

	bool SetDrumVol(double vol, int drumNum);
	bool GetDrumVol(int drumNum,  double * vol);
};

#endif /* INCLUDE_MIDIGLOBALDATA_H_ */
