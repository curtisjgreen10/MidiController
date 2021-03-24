
/*
 * digFilter.cpp
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */
#include "digFilter.h"
#include "waveCapture.h"



namespace DigFilter
{
	static float FIR_IMPULSE_RESPONSE[FIR_FILTER_LENGTH] =
	{
			-0.0004301f,-0.0004408f,-0.0004571f,-0.0004794f,-0.0005079f,-0.0005428f,-0.0005843f,-0.0006327f,-0.0006880f,-0.0007505f,-0.0008201f,-0.0008971f,-0.0009813f,-0.0010730f,-0.0011719f,-0.0012782f,-0.0013917f,-0.0015124f,-0.0016400f,-0.0017745f,-0.0019156f,-0.0020631f,-0.0022168f,-0.0023763f,-0.0025415f,-0.0027118f,-0.0028871f,-0.0030668f,-0.0032506f,-0.0034380f,-0.0036286f,-0.0038219f,-0.0040175f,-0.0042148f,-0.0044132f,-0.0046124f,-0.0048116f,-0.0050104f,-0.0052083f,-0.0054046f,-0.0055988f,-0.0057903f,-0.0059785f,-0.0061630f,-0.0063432f,-0.0065185f,-0.0066883f,-0.0068523f,-0.0070098f,-0.0071604f,-0.0073037f,-0.0074391f,-0.0075663f,-0.0076848f,-0.0077944f,-0.0078946f,-0.0079851f,-0.0080656f,-0.0081360f,-0.0081959f,-0.0082451f,-0.0082836f,-0.0083112f,-0.0083278f,0.9916667f,-0.0083278f,-0.0083112f,-0.0082836f,-0.0082451f,-0.0081959f,-0.0081360f,-0.0080656f,-0.0079851f,-0.0078946f,-0.0077944f,-0.0076848f,-0.0075663f,-0.0074391f,-0.0073037f,-0.0071604f,-0.0070098f,-0.0068523f,-0.0066883f,-0.0065185f,-0.0063432f,-0.0061630f,-0.0059785f,-0.0057903f,-0.0055988f,-0.0054046f,-0.0052083f,-0.0050104f,-0.0048116f,-0.0046124f,-0.0044132f,-0.0042148f,-0.0040175f,-0.0038219f,-0.0036286f,-0.0034380f,-0.0032506f,-0.0030668f,-0.0028871f,-0.0027118f,-0.0025415f,-0.0023763f,-0.0022168f,-0.0020631f,-0.0019156f,-0.0017745f,-0.0016400f,-0.0015124f,-0.0013917f,-0.0012782f,-0.0011719f,-0.0010730f,-0.0009813f,-0.0008971f,-0.0008201f,-0.0007505f,-0.0006880f,-0.0006327f,-0.0005843f,-0.0005428f,-0.0005079f,-0.0004794f,-0.0004571f,-0.0004408f
	};

	void StartVoiceRecorder(FirFilter *fir, long seconds)
	{
		waveCapture *pwc = new waveCapture(48000, 16, 1);

		DWORD bufferLength = pwc->getSuggestedBufferSize();


		if(pwc->start(0) == 0)
		{


			char* pWAVBuffer = new char[bufferLength];
			pwc->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav");


			for (int i = 0; i < seconds; i++)
			{
				pwc->readBuffer(pWAVBuffer);


				for (int j = 0; j < bufferLength; j++)
				{
					pWAVBuffer[j] = FirFilterUpdate(fir, pWAVBuffer[j]);
				}


				pwc->saveWAVEChunk(pWAVBuffer, bufferLength);

			}
			pwc->stop();
			pwc->closeWAVEFile();
			printf("wave file closed\n");
		}
		else
		{
			printf("error\n");
		}
	}


	void FirFilterInit(FirFilter *fir)
	{

		for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++)
		{
			fir->buf[n] = 0.0f;
		}

		fir->bufIndex = 0;
		fir->out = 0.0f;
	}

	float FirFilterUpdate(FirFilter *fir, float input)
	{
		fir->buf[fir->bufIndex] = input;

		fir->bufIndex++;

		if (fir->bufIndex == FIR_FILTER_LENGTH)
		{
			fir->bufIndex = 0;
		}

		fir->out = 0.0f;

		uint8_t sumIndex = fir->bufIndex;

		for (uint8_t n = 0; n < FIR_FILTER_LENGTH; n++)
		{
			if (sumIndex > 0)
			{
				sumIndex--;
			}
			else
			{
				sumIndex = FIR_FILTER_LENGTH - 1;
			}

			fir->out += FIR_IMPULSE_RESPONSE[n]*fir->buf[sumIndex];

		}

		return fir->out;
	}
}





