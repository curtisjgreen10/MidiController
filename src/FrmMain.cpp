#include "audio.h"
#include "FrmMain.h"
#include "micInput.h"
#include <iostream>
#include <stdio.h>
#include <mmsystem.h>
#include <conio.h>
#include <thread>
#include <mutex>
#include "gData.h"
#include "timer.h"

using namespace Gtk;

static void startRecorder(long seconds);
Timer *timer;
Audio *audioCtrl;

FrmMain::FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject), builder(refGlade){

	builder->get_widget("btnDrum1", btnCymbal1);
	builder->get_widget("btnDrum2", btnCymbal2);
	builder->get_widget("btnDrum3", btnCymbal3);
	builder->get_widget("btnDrum4", btnCymbal4);

	builder->get_widget("btnVolDrum1", btnVolCymbal1);
	builder->get_widget("btnVolDrum2", btnVolCymbal2);
	builder->get_widget("btnVolDrum3", btnVolCymbal3);
	builder->get_widget("btnVolDrum4", btnVolCymbal4);

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

	btnCymbal1->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &FrmMain::on_cymbal_x_button_clicked), 1));
	btnCymbal2->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &FrmMain::on_cymbal_x_button_clicked), 2));
	btnCymbal3->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &FrmMain::on_cymbal_x_button_clicked), 3));
	btnCymbal4->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &FrmMain::on_cymbal_x_button_clicked), 4));

	btnVolCymbal1->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolCymbal_1_value_changed));
	btnVolCymbal2->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolCymbal_2_value_changed));
	btnVolCymbal3->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolCymbal_3_value_changed));
	btnVolCymbal4->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolCymbal_4_value_changed));
}



static void startRecorder(long seconds)
{
	//printf("thread started\n");
	//DigFilter::IirFilterInit(&iir);
	//DigFilter::FirFilterInit(&fir);
	//DigFilter::StartVoiceRecorder(&fir,seconds);
}


void FrmMain::on_drum_x_button_clicked(int data)
{
	if(audioCtrl->getIsRecording())
	{
		switch(data)
		{
			case 1:
				MidiGlobalData::drum1hits[MidiGlobalData::drum1numHits] = timer->GetElapsedTimeMilliSeconds();
				MidiGlobalData::drum1numHits++;
				audioCtrl->ReadPreRecordedWavData(DRUM_1);
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

void FrmMain::on_cymbal_x_button_clicked(int data)
{
	if(audioCtrl->getIsRecording())
	{
		switch(data)
		{
			case 1:
				MidiGlobalData::drum1hits[MidiGlobalData::drum1numHits] = timer->GetElapsedTimeMilliSeconds();
				MidiGlobalData::drum1numHits++;

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
}

void FrmMain::on_btnVolCymbal_1_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::drum1vol = btnVolCymbal1->get_value();
}

void FrmMain::on_btnVolCymbal_2_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::drum2vol = btnVolCymbal2->get_value();
}

void FrmMain::on_btnVolCymbal_3_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::drum3vol = btnVolCymbal3->get_value();
}

void FrmMain::on_btnVolCymbal_4_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::drum4vol = btnVolCymbal4->get_value();
}

void FrmMain::on_record_button_clicked()
{
	audioCtrl->setIsRecording(true);
	printf("button clicked\n");

	long seconds = 0;
	Glib::ustring ustr = txtSeconds->get_text();
	std::stringstream s;
	s << ustr.raw();
	s >> seconds;

	timer->Start();

	std::thread recorderThread(startRecorder, seconds);

	recorderThread.detach();
}



unsigned __stdcall startPlayer (void* lpParam)
{
	printf("Thread started\n");
	PlaySound(TEXT("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void FrmMain::on_playback_button_clicked()
{
	/*
    std::ifstream infile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav", std::ios::binary);
    if (!infile)
    {
         std::cout << "Wave::file error: "<< std::endl;
        return;
    }

    infile.seekg (0, std::ios::end);   // get length of file
    int length = infile.tellg();
    char* testBuffer = new char[length];    // allocate memory
    infile.seekg (0, std::ios::beg);   // position to start of file
    infile.read (testBuffer,length);  // read entire file

    infile.close();
    int testDataIndex = findDataIndex(length, testBuffer);

	FILE * pFile1;
	pFile1 = fopen ("test_data.txt","w");

	for (int j = testDataIndex; j < length; j++)
	{
		fprintf(pFile1, "%d   ", testBuffer[j]);
		fprintf(pFile1, "\n");
	}
	*/

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




