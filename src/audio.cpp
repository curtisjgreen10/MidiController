/*
 * digFilter.cpp
 *
 *  Created on: Mar 19, 2021
 *      Author: cjgree13
 */
#include "audio.h"
#include "gData.h"
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

	// filtering is currently being bypassed.
	// update SaveVoiceData to use wavBufferDouble or output to incorporate filtering


	wavBufferDoubleOutput = new double[bufferLength];

	if (fType == FIR)
	{
		for (int i = 0; i < bufferLength; i++)
		{
			/*

			if (i  == 6000)
			{
				wavBufferDoubleOutput[i] = firFilt->FirFilterUpdate(&firDat, wavBufferDouble[i]);
			}
			else
			{
				wavBufferDoubleOutput[i] = firFilt->FirFilterUpdate(&firDat, wavBufferDouble[i]);

			}
			*/

			wavBufferDoubleOutput[i] = firFilt->FirFilterUpdate(&firDat, wavBufferDouble[i]);
			//wavBufferDoubleOutput[i] = wavBufferDouble[i];
		}
	}
	else if (fType == IIR)
	{
		//iirFilt->RunIIRBiquadForm1(&iirDat, wavBufferDouble, output, buffLen*seconds);
	}


	// un-normalize data
	NormalizeData(wavBufferDoubleOutput, bufferLength, 0, false);

	SaveVoiceData(pwc);
}

DWORD Audio::RecordVoiceData(waveCapture *wav)
{
	//bufferLength = wav->getSuggestedBufferSize();
	if(wav->start(0) == 0)
	{

		//wavBufferChar = new char[bufferLength];


	    std::ifstream infile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/total_signal_unfiltered.wav", std::ios::binary);
	    if (!infile)
	    {
	         std::cout << "Wave::file error: "<< std::endl;
	    }

	    infile.seekg (0, std::ios::end);   // get length of file
	    int length = infile.tellg();
	    infile.seekg (0, std::ios::beg);   // position to start of file
	    bufferLength = length;
		wavBufferCharTotal = new char[bufferLength];
		//int16_t * wavBuffer16Total = new int16_t[bufferLength];
		wavBufferCharTotal_test = new char[bufferLength];
		wavBufferDouble = new double[bufferLength];
	    infile.read (wavBufferCharTotal,length);  // read entire file
	    int testDataIndex = FindDataIndex(length, wavBufferCharTotal);


		wav->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/total_signal_filtered_FIR_C.wav");


		/*
		for (int i = 0; i < seconds; i++)
		{
			wav->readBuffer(wavBufferChar);
			for (int j = 0; j < bufferLength; j++)
			{
				wavBufferCharTotal[j +(i * bufferLength)] = wavBufferChar[j];
				wavBufferCharTotal_test[j +(i * bufferLength)] = wavBufferChar[j];
			}
		}
		*/



		int idx = 0;
		char byte1;
		char byte2;
		int16_t sample = 0;
		for (int i = 40; i < bufferLength; i+=2)
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

		//std::cout << "The smallest element is " << *std::min_element(wavBuffer16Total, wavBuffer16Total + bufferLength) << '\n';
		//std::cout << "The largest element is "  << *std::max_element(wavBuffer16Total, wavBuffer16Total + bufferLength) << '\n';

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


	for (int j = 40; j < bufferLength; j+=2)
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

	wav->saveWAVEChunk(wavBufferCharTotal, bufferLength);


	/*
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
*/



	//std::ofstream outfile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/test_filtered.wav", std::ios::binary);
	//outfile.write(wavBufferChar, bufferLength);
	//outfile.close();

	//wav->stop();
	wav->closeWAVEFile();
/*
	char* wavBufferChar_test = new char[bufferLength];

	wav->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test_unfiltered.wav");
	for (int i = 0; i < seconds; i++)
	{
		for (int j = 0; j < bufferLength; j++)
		{
			wavBufferChar_test[j] = wavBufferCharTotal_test[j +(i * bufferLength)];
		}
		wav->saveWAVEChunk(wavBufferChar_test, bufferLength);
	}

	wav->stop();
	wav->closeWAVEFile();
*/
	printf("wave file closed\n");

	// clean-up the heap
	delete[] wavBufferChar;
	delete[] wavBufferCharTotal;
	delete[] wavBufferDouble;
	delete[] wavBufferDoubleOutput;
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

