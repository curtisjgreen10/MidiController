/*
 * digFilter.h
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */
#ifndef INCLUDE_AUDIO_H_
#define INCLUDE_AUDIO_H_

#include "iirFilter.h"
#include "firFilter.h"
#include "waveCapture.h"

#define SAMPLING_RATE 48000

typedef enum FilterType
{
	FIR = 0,
	IIR
} FilterType;

typedef enum WavFile
{
	DRUM_1 = 0,
	DRUM_2,
	DRUM_3,
	DRUM_4,
	CYMBAL_1,
	CYMBAL_2,
	CYMBAL_3,
	CYMBAL_4,
} WavFile;

class Audio
{

private:
	bool recording = false;
	long seconds = 0;	/*!< seconds recorded */
	DWORD bufferLength = 0;	/*!< buffer length of wavBufferChar */
	bool preRecBuffInit = false;

	char* wavBufferChar;	/*!< buffer to hold voice data in char format */
	double* wavBufferDouble;	/*!< buffer to hold voice data in double format */
	char* preRecWavBuffer;	/*!< buffer to hold pre-recorded wav data */
	char* totalPreRecWavBuffer;	/*!< buffer to hold total pre-recorded wav data */

	FirData *firDat;	/*!< fir filter data */
	FirFilter *firFilt;	/*!< fir filter class object pointer */
	IirData *iirDat;	/*!< fir filter data */
	IirFilter *iirFilt;	/*!< iir filter class object pointer */
	FilterType fType = FIR; /*!< initialized to fir and reset in init method */

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
	DWORD RecordVoiceData(waveCapture *wav);

	//! \brief TBD.
	//!
	//! @return TBD.
	void SaveVoiceData(waveCapture *wav);

	//! \brief TBD.
	//!
	//! @return TBD.
	void UpdateTotalPreRecordedWav(int bufferlen);

public:

	//! \brief TBD.
	//!
	//! @return TBD.
	bool getIsRecording();

	//! \brief TBD.
	//!
	//! @return TBD.
	void setIsRecording(bool rec);

	//! \brief TBD.
	//!
	//! @return TBD.
	void DigitalFilterInit(FilterType filt);

	//! \brief TBD.
	//!
	//! @return TBD.
	void ReadPreRecordedWavData(WavFile wav);

	//! \brief TBD.
	//!
	//! @return TBD.
	void StartVoiceRecorder(long sec);

};

#endif /* INCLUDE_AUDIO_H_ */
