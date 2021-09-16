/*
 * audio.h
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */
#ifndef INCLUDE_AUDIO_H_
#define INCLUDE_AUDIO_H_

#include "iirFilter.h"
#include "firFilter.h"
#include "waveCapture.h"

#define SAMPLING_RATE 44100



//! Enum used for filter type.
typedef enum FilterType
{
	FIR = 0,
	IIR
} FilterType;

//! Enum used for pre-recorded wav files.
typedef enum WavFile
{
	NONE = 0,
	DRUM_1,
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
	FirData firDat;	/*!< fir filter data */
	IirData iirDat;	/*!< iir filter data */

	int dataIndex;
	unsigned long bufferLengthPreRec = 0;	/*!< buffer length of wavBufferChar */
	bool preRecBuffInit = false; /*!< true if space has already been allocated reading pre-recorded wav files */

	char* wavBufferChar;	/*!< buffer to hold raw voice data in char format */
	int16_t* wavBuffer16bitMixed;	/*!< buffer to hold raw voice data in char format */
	char* wavBufferCharTotal;	/*!< buffer to hold raw voice data in char format */
	char* preRecWavBuffer;	/*!< buffer to hold pre-recorded wav data */
	int16_t* preRecWavBuffer16bit;	/*!< buffer to hold pre-recorded wav data */
	char* totalPreRecWavBuffer;	/*!< buffer to hold total pre-recorded wav data */
	double normalizeFactor;

	waveCapture *waveRecordObj;	/*!< wave capture object pointer */
	FirFilter *firFilt;	/*!< fir filter class object pointer */
	IirFilter *iirFilt;	/*!< iir filter class object pointer */
	FilterType fType = FIR; /*!< initialized to fir and reset in init method */
	long seconds = 0;	/*!< seconds recorded */




	//! \brief Records voice from microphone input.
	//!
	//! @param wav		waveCapture object to hold recording data.
	//! @return		Buffer length of voice data recorded.
	unsigned long RecordVoiceData(waveCapture *wav);



	//! \brief Update the total pre-recorded buffer with new pre-recorded buffer data.
	//!
	//! @param wav		Buffer length of new pre-recorded data.
	void UpdateTotalPreRecordedWav(int bufferlen, double time);



	//! \brief Read pre-recorded wav files after button presses.
	//!
	//! @param wav		The pre-recorded wav file to read from disk.
	void ReadPreRecordedWavData(WavFile wav);

public:
	unsigned long bufferLength = 0;	/*!< buffer length of wavBufferChar */
	double* wavBufferDouble;	/*!< buffer to hold voice data in double format */
	double* wavBufferDoubleOutput;	/*!< buffer to hold voice data in double format */

	//! \brief Finds the index after header in a wav file where the data is located.
	//!
	//! @param bufferLength		Length of the wav data buffer to search.
	//! @param buff		Wav data buffer to search through.
	//! @return		Starting index of data in wav file.
	int FindDataIndex(int bufferLength, char * buff);

	//! \brief Get whether or not the software is recording.
	//!
	//! @return True if recording, otherwise false.
	long GetSecondsToRecord();

	//! \brief Set whether or not the software is recording.
	//!
	//! @return True if recording, false otherwise.
	void SetSecondsToRecord(long sec);

	//! \brief Initialize the type of digital filter to be used for mic recording.
	//!
	//! @param filt		The type of filter to be used.
	void DigitalFilterInit(FilterType filt);

	//! \brief Read pre-recorded wav files after button presses.
	//!
	//! @param sec		The number of seconds to record for (user specified).
	void Record();

	void FilterVoiceData();

	void MixAudio();

	//! \brief Save voice data to disk.
	//!
	//! @param wav		waveCapture object to hold recording data.
	void SaveVoiceData();

	//! \brief Normalizes a buffer of data using p-norm to a range of -1.0 and 1.0.
	//!
	//! @param data		Data to normalize.
	//! @param dataLen		Length of data buffer.
	//! @param p		P value for p-norm equation.
	//! @return		Magnitude of data (can be used to un-normalize data).
	void NormalizeData(double * data, int dataLen, int p, bool normailze);



};

#endif /* INCLUDE_AUDIO_H_ */
