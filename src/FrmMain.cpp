#include "FrmMain.h"
#include "micInput.h"
#include "waveCapture.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <conio.h>
#include <fstream>
#include <thread>
#include <mutex>
#include "gData.h"

using namespace Gtk;


char* cymbalBuffer = 0;

static int findDataIndex(int bufferLength, char * buff);
static void startTimer(long seconds);
static void startRecorder(long seconds);
bool drum1flag = false;
bool recordDone, timerDone = false;
std::mutex mRecorder, mTimer, mDrum1Flag;
std::mutex gMidiData;


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



static void startRecorder(long seconds)
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
	mRecorder.lock();
	recordDone = true;
	mRecorder.unlock();
}


void FrmMain::on_drum_x_button_clicked(int data)
{
	switch(data)
	{
		case 1:
			//printf("Drum 1 hit\n");
			mDrum1Flag.lock();
			drum1flag = true;
			mDrum1Flag.unlock();
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
	// update global data with new volume.
	MidiGlobalData::drum1vol = btnVolDrum1->get_value();
}

void FrmMain::on_btnVolDrum_2_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::drum2vol = btnVolDrum2->get_value();
}

void FrmMain::on_btnVolDrum_3_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::drum3vol = btnVolDrum3->get_value();
}

void FrmMain::on_btnVolDrum_4_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::drum4vol = btnVolDrum4->get_value();
}

static void startTimer(long seconds)
{
	bool flg = false;
	int minutes_ = 0;
	double seconds_ = 0;
	MidiGlobalData::drum1numHits = 0;

	//printf("starting timer\n");

    for (int i = 0; i <= (seconds*2); i++) {

        // sleep system call to sleep
        // for 1 second
        Sleep(500);

        // increment half second increments
        seconds_ = seconds_ + 0.5;

        /*
        // if seconds reaches 60
        if (seconds == 60) {

            // increment minutes
            minutes++;
            seconds = 0;
        }
        */
        if(mDrum1Flag.try_lock())
        {
        	//printf("locked 1\n");
        	flg = drum1flag;
        	mDrum1Flag.unlock();
        }



        if (flg)
        {
        	//printf("update global data\n");

        	gMidiData.lock();
        	// store the time of drum hit
        	MidiGlobalData::drum1hits[MidiGlobalData::drum1numHits] = seconds_;
        	printf("second: %f \n", seconds_);
        	// increment count
        	MidiGlobalData::drum1numHits++;
        	// reset drum flag
        	gMidiData.unlock();

        	if(mDrum1Flag.try_lock())
        	{
        		//printf("locked 2\n");
        		drum1flag = false;
        		mDrum1Flag.unlock();
        	}

        	//printf("drum hit\n");
        }

    }

    //printf("Timer done\n");

    gMidiData.lock();
	for (int i = 0; i < MidiGlobalData::drum1numHits; i++)
	{
		printf("drum 1 hit time: %f \n", MidiGlobalData::drum1hits[i]);
	}
	gMidiData.unlock();

	mTimer.lock();
	timerDone = true;
	mTimer.unlock();
}


void FrmMain::on_record_button_clicked()
{

	long seconds = 0;
	Glib::ustring ustr = txtSeconds->get_text();
	std::stringstream s;
	s << ustr.raw();
	s >> seconds;

	std::thread recorderThread(startRecorder, seconds);
	std::thread timerThread(startTimer, seconds);

	recorderThread.detach();
	timerThread.detach();


	/*
    std::ifstream infile("C:\\Users\\cjgree13\\Documents\\CSE593\\MidiController\\MidiController\\synth.wav", std::ios::binary);
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
*/
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




