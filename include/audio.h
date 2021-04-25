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

//! Enum used for filter type.
typedef enum FilterType
{
	FIR = 0,
	IIR
} FilterType;

//! Enum used for pre-recorded wav files.
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
	bool recording = false; /*!< true if software is recording */
	long seconds = 0;	/*!< seconds recorded */
	DWORD bufferLength = 0;	/*!< buffer length of wavBufferChar */
	bool preRecBuffInit = false; /*!< true if space has already been allocated reading pre-recorded wav files */

	char* wavBufferChar;	/*!< buffer to hold voice data in char format */
	double* wavBufferDouble;	/*!< buffer to hold voice data in double format */
	char* preRecWavBuffer;	/*!< buffer to hold pre-recorded wav data */
	char* totalPreRecWavBuffer;	/*!< buffer to hold total pre-recorded wav data */

	waveCapture *pwc;	/*!< wave capture object pointer */
	FirData *firDat;	/*!< fir filter data */
	FirFilter *firFilt;	/*!< fir filter class object pointer */
	IirData *iirDat;	/*!< fir filter data */
	IirFilter *iirFilt;	/*!< iir filter class object pointer */
	FilterType fType = FIR; /*!< initialized to fir and reset in init method */

	//! \brief Finds the index after header in a wav file where the data is located.
	//!
	//! @param bufferLength		Length of the wav data buffer to search.
	//! @param buff		Wav data buffer to search through.
	//! @return		Starting index of data in wav file.
	int FindDataIndex(int bufferLength, char * buff);

	//! \brief Normalizes a buffer of data using p-norm to a range of -1.0 and 1.0.
	//!
	//! @param data		Data to normalize.
	//! @param dataLen		Length of data buffer.
	//! @param p		P value for p-norm equation.
	//! @return		Magnitude of data (can be used to un-normalize data).
	double NormalizeData(double * data, int dataLen, int p);

	//! \brief Records voice from microphone input.
	//!
	//! @param wav		waveCapture object to hold recording data.
	//! @return		Buffer length of voice data recorded.
	DWORD RecordVoiceData(waveCapture *wav);

	//! \brief Save voice data to disk.
	//!
	//! @param wav		waveCapture object to hold recording data.
	void SaveVoiceData(waveCapture *wav);

	//! \brief Update the total pre-recorded buffer with new pre-recorded buffer data.
	//!
	//! @param wav		Buffer length of new pre-recorded data.
	void UpdateTotalPreRecordedWav(int bufferlen, double time);

public:

	//! \brief Get whether or not the software is recording.
	//!
	//! @return True if recording, otherwise false.
	bool getIsRecording();

	//! \brief Set whether or not the software is recording.
	//!
	//! @return True if recording, false otherwise.
	void setIsRecording(bool rec);

	//! \brief Initialize the type of digital filter to be used for mic recording.
	//!
	//! @param filt		The type of filter to be used.
	void DigitalFilterInit(FilterType filt);

	//! \brief Read pre-recorded wav files after button presses.
	//!
	//! @param wav		The pre-recorded wav file to read from disk.
	void ReadPreRecordedWavData(WavFile wav, double time);

	//! \brief Read pre-recorded wav files after button presses.
	//!
	//! @param sec		The number of seconds to record for (user specified).
	void StartVoiceRecorder(long sec);

};

#endif /* INCLUDE_AUDIO_H_ */
