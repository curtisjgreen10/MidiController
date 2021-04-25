/*
 * digFilter.cpp
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */
#include "audio.h"
#include "gData.h"
#include <tgmath.h>


bool Audio::getIsRecording()
{
	return recording;
}

void Audio::setIsRecording(bool rec)
{
	recording = rec;
}

void Audio::DigitalFilterInit(FilterType filt)
{
	if (filt == FIR)
	{
		firFilt->FirFilterInit(firDat);
		fType = FIR;
	}
	else if (filt == IIR)
	{
		iirFilt->IirFilterInit(iirDat);
		fType = IIR;
	}
}

void Audio::StartVoiceRecorder(long sec)
{
	seconds = sec;
	pwc = new waveCapture(SAMPLING_RATE, 16, 1);

	RecordVoiceData(pwc);

	// filtering is currently being bypassed.
	// update SaveVoiceData to use wavBufferDouble or output to incorporate filtering
	/*

	double* output = new double[seconds*buffLen];

	if (fType == FIR)
	{
		for (int i = 0; i < buffLen*seconds; i++)
		{
			firFilt->FirFilterUpdate(firDat, wavBufferDouble[i]);
		}
	}
	else if (fType == IIR)
	{
		iirFilt->RunIIRBiquadForm1(iirDat, wavBufferDouble, output, buffLen*seconds);
	}

	*/

	SaveVoiceData(pwc);
}

DWORD Audio::RecordVoiceData(waveCapture *wav)
{
	bufferLength = wav->getSuggestedBufferSize();

	if(wav->start(0) == 0)
	{

		/*
		FILE * pFile1;
		pFile1 = fopen ("pre_normalized.txt","w");


		FILE * pFile2;
		pFile2 = fopen ("normailzed.txt","w");

		FILE * pFile3;
		pFile3 = fopen ("filtered.txt","w");
		*/
		FILE * pFile4;
		pFile4 = fopen ("filtered_mag.txt","w");

		FILE * pFile5;
		pFile5 = fopen ("normalized_mag.txt","w");


		wavBufferChar = new char[bufferLength];
		wavBufferDouble = new double[seconds*bufferLength];
		//double* pWAVBuffer3 = new double[seconds*bufferLength];
		//double* output = new double[seconds*bufferLength];
		wav->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav");

		for (int i = 0; i < seconds; i++)
		{
			wav->readBuffer(wavBufferChar);

			for (int j = 0; j < bufferLength; j++)
			{
				wavBufferDouble[j + (i * bufferLength)] = (double)wavBufferChar[j];
			}

		}


		NormalizeData(wavBufferDouble, seconds*bufferLength, 4);

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
	double temp1 = 0;
	for (int i = 0; i < seconds; i++)
	{
		for (int j = 0; j < bufferLength; j++)
		{
			wavBufferChar[j] = (char)temp1;
		}
		wav->saveWAVEChunk(wavBufferChar, bufferLength);
	}
	wav->stop();
	wav->closeWAVEFile();
	printf("wave file closed\n");
}

double Audio::NormalizeData(double * data, int dataLen, int p)
{
	double sum = 0;
	double* tempBuf = new double[dataLen];

	for (int i = 0; i < dataLen; i++)
	{
		tempBuf[i] = abs(data[i]);
		tempBuf[i] = pow(tempBuf[i],(double)p);
		sum += tempBuf[i];
	}

	sum = pow(sum,1.0/(double)p);

	for (int i = 0; i < dataLen; i++)
	{
		data[i] = data[i] / sum;
	}

	return sum;
}

void Audio::UpdateTotalPreRecordedWav(int bufferlen, double time)
{
	int startIndex = time * SAMPLING_RATE;

	for (int i = startIndex; i < bufferlen; i++)
	{
		totalPreRecWavBuffer[i] = preRecWavBuffer[i];
	}
}

void Audio::ReadPreRecordedWavData(WavFile wav, double time)
{
	std::ifstream infile;

	switch(wav)
	{
		case DRUM_1:
			infile.open("C:\\Users\\cjgree13\\Documents\\CSE593\\MidiController\\MidiController\\synth.wav", std::ios::binary);
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
	int length = infile.tellg();
	preRecWavBuffer = new char[length];    // allocate memory
	infile.seekg (0, std::ios::beg);   // position to start of file
	infile.read (preRecWavBuffer,length);  // read entire file
	infile.close();

	if (preRecBuffInit == false)
	{
		totalPreRecWavBuffer = new char[bufferLength*seconds];
		preRecBuffInit = true;
	}

	UpdateTotalPreRecordedWav(length, time);
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

