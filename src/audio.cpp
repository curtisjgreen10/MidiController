/*
 * digFilter.cpp
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */
#include "audio.h"
#include "gData.h"
#include "mixqueue.h"
#include <tgmath.h>
#include <iostream>
#include <cmath>
#include <algorithm>

/*
bool Audio::getIsRecording()
{
	return recording;
}

void Audio::setIsRecording(bool rec)
{
	recording = rec;
}
*/

void Audio::DigitalFilterInit(FilterType filt)
{
	if (filt == FIR)
	{
		firFilt = new FirFilter();
		firFilt->FirFilterInit(&firDat);
		fType = FIR;
	}
	else if (filt == IIR)
	{
		iirFilt = new IirFilter();
		iirFilt->IirFilterInit(&iirDat);
		fType = IIR;
	}
}

void Audio::StartVoiceRecorder(long sec)
{
	seconds = sec;
	pwc = new waveCapture(SAMPLING_RATE, 16, 1);

	RecordVoiceData(pwc);

	wavBufferDoubleOutput = new double[bufferLength];

	if (fType == FIR)
	{
		for (int i = 0; i < bufferLength; i++)
		{
			wavBufferDoubleOutput[i] = firFilt->FirFilterUpdate(&firDat, wavBufferDouble[i]);
		}
	}
	else if (fType == IIR)
	{
		iirFilt->RunIIRBiquadForm2(&iirDat, wavBufferDouble, wavBufferDoubleOutput, bufferLength);
	}


	// un-normalize data
	NormalizeData(wavBufferDoubleOutput, bufferLength, 0, false);

	SaveVoiceData(pwc);

}

DWORD Audio::RecordVoiceData(waveCapture *wav)
{
	if(wav->start(0) == 0)
	{

#if TONE_TEST
	    std::ifstream infile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/total_signal_unfiltered.wav", std::ios::binary);
	    if (!infile)
	    {
	         std::cout << "Wave::file error: "<< std::endl;
	    }

	    infile.seekg (0, std::ios::end);   // get length of file
	    int length = infile.tellg();
	    infile.seekg (0, std::ios::beg);   // position to start of file
	    bufferLength = length;
		wavBufferCharTotal_test = new char[bufferLength];
	    infile.read (wavBufferCharTotal,length);  // read entire file
	    int testDataIndex = FindDataIndex(length, wavBufferCharTotal);
	    wavBufferDouble = new double[bufferLength];
	    wavBufferCharTotal = new char[bufferLength];
	    wav->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/total_signal_filtered_IIRC.wav");
#else
	    wav->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/total_signal_filtered_voice.wav");
		bufferLength = wav->getSuggestedBufferSize();
		wavBufferChar = new char[bufferLength];
		wavBufferDouble = new double[bufferLength*seconds];
		wavBufferCharTotal = new char[bufferLength*seconds];
		for (int i = 0; i < seconds; i++)
		{
			wav->readBuffer(wavBufferChar);
			for (int j = 0; j < bufferLength; j++)
			{
				wavBufferCharTotal[j +(i * bufferLength)] = wavBufferChar[j];
			}
		}

#endif
		int idx = 0;
		char byte1;
		char byte2;
		int16_t sample = 0;
#if TONE_TEST
		for (int i = 40; i < bufferLength; i+=2)
#else
		bufferLength = bufferLength * seconds;
		for (int i = 0; i < bufferLength; i+=2)
#endif
		{
			// convert to byte data to 16 bit little endian
			sample = 0;
			byte1 = wavBufferCharTotal[i];
			byte2 = wavBufferCharTotal[i+1];
			sample = byte2 << 8;
			sample |= (byte1 & 0xFF);
			// cast new 16-bit data to double for processing
			wavBufferDouble[idx] = (double)ceil(sample);
			idx++;
		}
		NormalizeData(wavBufferDouble, bufferLength, 4, true);
		return bufferLength;
	}
	else
	{
		printf("error\n");
		return -1;
	}
}



void Audio::SaveVoiceData(waveCapture *wav)
{
	char byte1 = 0;
	char byte2 = 0;
	double temp1 = 0;
	int16_t sample = 0;
	int idx = 0;

#if TONE_TEST
		for (int j = 40; j < bufferLength; j+=2)
#else
		for (int j = 0; j < bufferLength; j+=2)
#endif
	{
		// cast back to 16-bit data
		sample = (int16_t)wavBufferDoubleOutput[idx];
		// convert 16-bit data back to 8-bit.
		byte1 = (sample & 0xFF);
		byte2 = ((sample >> 8) & 0xFF);
		wavBufferCharTotal[j] = byte1;
		wavBufferCharTotal[j+1] = byte2;
		idx++;
	}



	MixAudio(wav);

#if TONE_TEST
	wav->saveWAVEChunk(wavBufferCharTotal, bufferLength);
#else
	bufferLength = wav->getSuggestedBufferSize();
	wavBufferChar = new char[bufferLength];
	for (int i = 0; i < seconds; i++)
	{
		for (int j = 0; j < bufferLength; j++)
		{
			wavBufferChar[j] = wavBufferCharTotal[j +(i * bufferLength)];
		}
		wav->saveWAVEChunk(wavBufferChar, bufferLength);
	}
#endif

	wav->closeWAVEFile();
	printf("wave file closed\n");

	// clean-up the heap
	//delete[] wavBufferChar;
	//delete[] wavBufferCharTotal;
	//delete[] wavBufferDouble;
	//delete[] wavBufferDoubleOutput;
}

void Audio::MixAudio(waveCapture *wav)
{
	int idx = 0;
	char byte1;
	char byte2;
	int16_t sample = 0;
	MusicData item;

	if (MidiGlobalData::queue->isEmpty(MidiGlobalData::queue))
	{
		// no buttons pushed during recording, no mixed required
		return;
	}

	wavBuffer16bitMixed = new int16_t[bufferLength];
#if TONE_TEST
	for (int i = 40; i < bufferLength; i+=2)
#else
	for (int i = 0; i < bufferLength; i+=2)
#endif
	{
		// convert to byte data to 16 bit little endian
		sample = 0;
		byte1 = wavBufferCharTotal[i];
		byte2 = wavBufferCharTotal[i+1];
		sample = byte2 << 8;
		sample |= (byte1 & 0xFF);
		wavBuffer16bitMixed[idx] = sample;
		idx++;
	}

	sample = 0;
	float sec = 0;
	int eventSample = 0;
	preRecWavBuffer16bit = new int16_t[bufferLength];
	for (int i = 0; i < MidiGlobalData::queue->size_; i++)
	{
		item = MidiGlobalData::queue->dequeue(MidiGlobalData::queue);
		sec = item.msec / 1000.0; //convert to seconds
		eventSample = sec * SAMPLING_RATE; // get the sample at which the button press occured
		ReadPreRecordedWavData(item.file);
		idx = eventSample;

		for (int i = 40; i < bufferLengthPreRec; i+=2)
		{
			// convert to byte data to 16 bit little endian
			sample = 0;
			byte1 = preRecWavBuffer[i];
			byte2 = preRecWavBuffer[i+1];
			sample = byte2 << 8;
			sample |= (byte1 & 0xFF);
			preRecWavBuffer16bit[idx] = sample;
			idx++;
		}

#if TONE_TEST
		for (int i = 40; i < bufferLength; i+=2)
#else
		for (int i = 0; i < bufferLength; i+=2)
#endif
		{
			wavBuffer16bitMixed[i] = (wavBuffer16bitMixed[i] + preRecWavBuffer16bit[i]) / 2;
		}

		idx = 0;
#if TONE_TEST
		for (int j = 40; j < bufferLength; j+=2)
#else
		for (int j = 0; j < bufferLength; j+=2)
#endif
		{
			// cast back to 16-bit data
			sample = wavBuffer16bitMixed[idx];
			// convert 16-bit data back to 8-bit.
			byte1 = (sample & 0xFF);
			byte2 = ((sample >> 8) & 0xFF);
			wavBufferCharTotal[j] = byte1;
			wavBufferCharTotal[j+1] = byte2;
			idx++;
		}

		//wav->saveWAVEChunk(wavBufferCharTotal, bufferLength);
	}
}

void Audio::NormalizeData(double * data, int dataLen, int p, bool normalize)
{
	double temp;
	double sum = 0;
	if (normalize)
	{
		for (int i = 0; i < dataLen; i++)
		{
			temp = 0;
			temp = abs(data[i]);
			temp = pow(temp,(double)p);
			sum += temp;
		}

		normalizeFactor = pow(sum,1.0/(double)p);

		for (int i = 0; i < dataLen; i++)
		{
			data[i] = data[i] / normalizeFactor;
		}
	}
	else
	{
		for (int i = 0; i < dataLen; i++)
		{
			data[i] = data[i]*normalizeFactor;
		}
	}
}

void Audio::UpdateTotalPreRecordedWav(int bufferlen, double time)
{
	int startIndex = time * SAMPLING_RATE;

	for (int i = startIndex; i < bufferlen; i++)
	{
		totalPreRecWavBuffer[i] = preRecWavBuffer[i];
	}
}

void Audio::ReadPreRecordedWavData(WavFile wav)
{
	std::ifstream infile;

	switch(wav)
	{
		case DRUM_1:
			infile.open("C:\\Users\\cjgree13\\Documents\\CSE593\\MidiController\\MidiController\\Closed-Hi-Hat-2.wav", std::ios::binary);
			break;
		/*
		case DRUM_2:
			break;
		case DRUM_3:
			break;
		case DRUM_4:
			break;
		case CYMBAL_1:
			break;
		case CYMBAL_2:
			break;
		case CYMBAL_3:
			break;
		case CYMBAL_4:
			break;
			*/
		default:
			break;

	}

	if (!infile)
	{
		printf("Wave::file error: ");
		return;
	}

	infile.seekg (0, std::ios::end);   // get length of file
	bufferLengthPreRec = infile.tellg();
	preRecWavBuffer = new char[bufferLengthPreRec];    // allocate memory
	infile.seekg (0, std::ios::beg);   // position to start of file
	infile.read (preRecWavBuffer,bufferLengthPreRec);  // read entire file
	infile.close();
}

int Audio::FindDataIndex(int bufferLength, char * buff)
{
	char dataCheck = 0;

	for (int j = 0; j < bufferLength; j++)
	{
		// check for a "d"
		if (buff[j] == 100)
		{
			// set 1st bit true, found a "d"
			dataCheck |= 1UL << 1;
		}
		else
		{
			dataCheck = 0;
		}

		// if the 1st bit is set, check next index for "a"
		if ((dataCheck >> 1) & 1U)
		{
			// check for "a"
			if (buff[j + 1] == 97)
			{
				// set 2nd bit true, found an "a"
				dataCheck |= 1UL << 2;
			}
			else
			{
				dataCheck = 0;
			}
		}

		// if the 2nd bit is set, check next index for "t"
		if ((dataCheck >> 2) & 1U)
		{
			// check for "t"
			if (buff[j + 2] == 116)
			{
				// set 3rd bit true, found a "t"
				dataCheck |= 1UL << 3;
			}
			else
			{
				dataCheck = 0;
			}
		}

		// if the 3rd bit is set, check next index for "a"
		if ((dataCheck >> 3) & 1U)
		{
			// check for "t"
			if (buff[j + 3] == 97)
			{
				// set 4th bit true, found an "a"
				dataCheck |= 1UL << 4;
				return (j + 4);
			}
			else
			{
				dataCheck = 0;
			}
		}
	}

	// data word not found
	return -1;
}

