/*
 * digFilter.h
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */
#ifndef INCLUDE_DIGFILTER_H_
#define INCLUDE_DIGFILTER_H_

#include "iirFilter.h"
#include "firFilter.h"
#include "waveCapture.h"


class DigFilter
{

private:

	//! \brief TBD.
	//!
	//! @return TBD.
	int FindDataIndex(int bufferLength, char * buff);

	//! \brief TBD.
	//!
	//! @return TBD.
	double NormalizeData(double * data, int dataLen, int p);

	//! \brief TBD.
	//!
	//! @return TBD.
	DWORD RecordVoiceData(waveCapture *wav, long seconds);

	//! \brief TBD.
	//!
	//! @return TBD.
	void SaveVoiceData(waveCapture *wav, DWORD buffLen, long seconds);

public:
	//! \brief TBD.
	//!
	//! @return TBD.
	void StartVoiceRecorder(FirFilter *filter, long seconds);

	//! \brief TBD.
	//!
	//! @return TBD.
	void StartVoiceRecorder(IirFilter *filter, long seconds);
};

#endif /* INCLUDE_DIGFILTER_H_ */
