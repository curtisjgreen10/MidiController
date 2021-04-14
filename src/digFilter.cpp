/*
 * digFilter.cpp
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */
#include <tgmath.h>
#include "digFilter.h"
#include "waveCapture.h"

char* pWAVBuffer;
double* pWAVBuffer2;

void DigFilter::StartVoiceRecorder(FirFilter *fir, long seconds)
{
	waveCapture *pwc = new waveCapture(48000, 16, 1);

	DWORD buffLen = RecordVoiceData(pwc, seconds);

	// make call to fir filter function here on pWAVBuffer2

	SaveVoiceData(pwc, buffLen, seconds);
}

void DigFilter::StartVoiceRecorder(IirFilter *iir, long seconds)
{
	waveCapture *pwc = new waveCapture(48000, 16, 1);

	DWORD buffLen = RecordVoiceData(pwc, seconds);

	// make call to fir filter function here on pWAVBuffer2

	SaveVoiceData(pwc, buffLen, seconds);
}

DWORD DigFilter::RecordVoiceData(waveCapture *wav, long seconds)
{

	DWORD bufferLength = wav->getSuggestedBufferSize();

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


		pWAVBuffer = new char[bufferLength];
		pWAVBuffer2 = new double[seconds*bufferLength];
		//double* pWAVBuffer3 = new double[seconds*bufferLength];
		double* output = new double[seconds*bufferLength];
		wav->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav");

		for (int i = 0; i < seconds; i++)
		{
			wav->readBuffer(pWAVBuffer);
			/*
			for (int j = 0; j < bufferLength; j++)
			{
				fprintf(pFile4, "%d   ", pWAVBuffer[j]);
				fprintf(pFile4, "\n");
			}
			*/

			for (int j = 0; j < bufferLength; j++)
			{
				pWAVBuffer2[j + (i * bufferLength)] = (double)pWAVBuffer[j];
			}

		}

		/*
		for (int i = 0; i < seconds*bufferLength; i++)
		{
			fprintf(pFile1, "%lf   ", pWAVBuffer2[i]);

			fprintf(pFile1, "\n");

			fprintf(pFile4, "%d   ", pWAVBuffer3[i]);

			fprintf(pFile4, "\n");

		}
		*/

		NormalizeData(pWAVBuffer2, seconds*bufferLength, 4);

		return bufferLength;
	}
	else
	{
		printf("error\n");
		return -1;
	}
}



void DigFilter::SaveVoiceData(waveCapture *wav, DWORD buffLen, long seconds)
{
	double temp1 = 0;
	double temp2 = 0;
	for (int i = 0; i < seconds; i++)
	{
		for (int j = 0; j < buffLen; j++)
		{
			pWAVBuffer[j] = (char)temp1;
		}
		wav->saveWAVEChunk(pWAVBuffer, buffLen);
	}
	wav->stop();
	wav->closeWAVEFile();
	printf("wave file closed\n");
}

double DigFilter::NormalizeData(double * data, int dataLen, int p)
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

int DigFilter::FindDataIndex(int bufferLength, char * buff)
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


