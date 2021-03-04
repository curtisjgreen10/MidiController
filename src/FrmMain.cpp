#include "FrmMain.h"
#include "micInput.h"
#include "waveCapture.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <conio.h>
#include <fstream>

using namespace Gtk;


char* cymbalBuffer = 0;

static int findDataIndex(int bufferLength, char * buff);




FrmMain::FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject), builder(refGlade){

	builder->get_widget("btnDrum1", btnDrum1);
	builder->get_widget("btnDrum2", btnDrum2);
	builder->get_widget("btnDrum3", btnDrum3);
	builder->get_widget("btnDrum4", btnDrum4);

	builder->get_widget("btnVolDrum1", btnVolDrum1);
	builder->get_widget("btnVolDrum2", btnVolDrum2);
	builder->get_widget("btnVolDrum3", btnVolDrum3);
	builder->get_widget("btnVolDrum4", btnVolDrum4);


	builder->get_widget("btnRecord", btnRecord);
	builder->get_widget("btnPlayback", btnPlayback);
	builder->get_widget("btnStopPlayback", btnStopPlayback);
	builder->get_widget("txtSeconds", txtSeconds);
	txtSeconds->set_editable(TRUE);

	txtSeconds->signal_activate().connect(sigc::mem_fun(*this, &FrmMain::on_entry_activated));

	btnRecord->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_record_button_clicked));
	btnPlayback->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_playback_button_clicked));
	btnStopPlayback->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_stop_playback_button_clicked));


	btnDrum1->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &FrmMain::on_drum_x_button_clicked), 1));
	btnDrum2->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &FrmMain::on_drum_x_button_clicked), 2));
	btnDrum3->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &FrmMain::on_drum_x_button_clicked), 3));
	btnDrum4->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &FrmMain::on_drum_x_button_clicked), 4));


	btnVolDrum1->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolDrum_1_value_changed));
	btnVolDrum2->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolDrum_2_value_changed));
	btnVolDrum3->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolDrum_3_value_changed));
	btnVolDrum4->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolDrum_4_value_changed));
}



unsigned __stdcall startRecorder (void* lpParam)
{

	// may need thread

	return 0;
}

void FrmMain::on_drum_x_button_clicked(int data)
{
	switch(data)
	{
		case 1:
			printf("Drum 1\n");
			break;
		case 2:
			printf("Drum 2\n");
			break;
		case 3:
			printf("Drum 3\n");
			break;
		case 4:
			printf("Drum 4\n");
			break;
		default:
			break;
	}
}

void FrmMain::on_btnVolDrum_1_value_changed(int data)
{
	printf("Drum 1 volume: %f\n" , btnVolDrum1->get_value());
}

void FrmMain::on_btnVolDrum_2_value_changed(int data)
{
	printf("Drum 2 volume: %f\n" , btnVolDrum2->get_value());
}

void FrmMain::on_btnVolDrum_3_value_changed(int data)
{
	printf("Drum 3 volume: %f\n" , btnVolDrum3->get_value());
}

void FrmMain::on_btnVolDrum_4_value_changed(int data)
{
	printf("Drum 4 volume: %f\n" , btnVolDrum4->get_value());
}


void FrmMain::on_record_button_clicked()
{
	long seconds;

    std::ifstream infile("C:\\Users\\cjgree13\\Documents\\CSE593\\MidiController\\MidiController\\synth.wav", std::ios::binary);

	waveCapture *pwc = new waveCapture(48000, 16, 1);


    if (!infile)
    {
         std::cout << "Wave::file error: "<< std::endl;
        return;
    }

    infile.seekg (0, std::ios::end);   // get length of file
    int length = infile.tellg();
    cymbalBuffer = new char[length];    // allocate memory
    infile.seekg (0, std::ios::beg);   // position to start of file
    infile.read (cymbalBuffer,length);  // read entire file

    infile.close();


    int cymbalDataIndex = findDataIndex(length, cymbalBuffer);


	Glib::ustring ustr = txtSeconds->get_text();
	std::stringstream s;
	s << ustr.raw();
	s >> seconds;
	printf("Capturing audio\n");

	/*
	HANDLE hThread;
	unsigned ThreadId;

	// Start capturing Thread: thread[0]
	hThread = (HANDLE)_beginthreadex(NULL, 0, startRecorder,NULL, 0, &ThreadId);
	if (hThread == 0)
	{
		printf("Unable to start sound capture thread! \n\n");
		exit(1);
	}
	printf("thread finished\n");
	*/


	DWORD bufferLength = pwc->getSuggestedBufferSize();


	if(pwc->start(0) == 0)
	{


		char* pWAVBuffer = new char[bufferLength];
		pwc->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav");


		for (int i = 0; i < seconds; i++)
		{
			pwc->readBuffer(pWAVBuffer);


			if (i < 7)
			{
				for (int j = 0; j < bufferLength; j++)
				{
					// perform mixing
					pWAVBuffer[j] = (pWAVBuffer[j] + cymbalBuffer[cymbalDataIndex + j])/2;
				}
			}

			cymbalDataIndex = cymbalDataIndex + bufferLength;

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

static int findDataIndex(int bufferLength, char * buff)
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

unsigned __stdcall startPlayer (void* lpParam)
{
	printf("Thread started\n");
	PlaySound(TEXT("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void FrmMain::on_playback_button_clicked()
{


}

void FrmMain::on_stop_playback_button_clicked()
{
	PlaySound(NULL,0,0);
}

void FrmMain::on_entry_activated()
{
  //std::cout << "Entry activated. Current text=\"" << entry_.get_text() << "\"."
   // << std::endl;
}




