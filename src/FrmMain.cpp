#include "FrmMain.h"
#include <iostream>
#include <stdio.h>
#include <mmsystem.h>
#include <conio.h>
#include <thread>
#include <mutex>
#include "gData.h"
#include "mixqueue.h"
#include <future>


using namespace Gtk;



FrmMain::FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject), builder(refGlade){

	builder->get_widget("btnCymbal1", btnCymbal1);
	builder->get_widget("btnCymbal2", btnCymbal2);
	builder->get_widget("btnCymbal3", btnCymbal3);
	builder->get_widget("btnCymbal4", btnCymbal4);

	builder->get_widget("btnVolCymbal1", btnVolCymbal1);
	builder->get_widget("btnVolCymbal2", btnVolCymbal2);
	builder->get_widget("btnVolCymbal3", btnVolCymbal3);
	builder->get_widget("btnVolCymbal4", btnVolCymbal4);

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

	btnDrum1->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_drum_1_button_clicked));
	btnDrum2->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_drum_2_button_clicked));
	btnDrum3->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_drum_3_button_clicked));
	btnDrum4->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_drum_4_button_clicked));

	btnVolDrum1->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolDrum_1_value_changed));
	btnVolDrum2->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolDrum_2_value_changed));
	btnVolDrum3->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolDrum_3_value_changed));
	btnVolDrum4->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolDrum_4_value_changed));

	btnCymbal1->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_cymbal_1_button_clicked));
	btnCymbal2->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_cymbal_2_button_clicked));
	btnCymbal3->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_cymbal_3_button_clicked));
	btnCymbal4->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_cymbal_4_button_clicked));

	btnVolCymbal1->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolCymbal_1_value_changed));
	btnVolCymbal2->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolCymbal_2_value_changed));
	btnVolCymbal3->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolCymbal_3_value_changed));
	btnVolCymbal4->signal_value_changed().connect(sigc::mem_fun(*this, &FrmMain::on_btnVolCymbal_4_value_changed));
}



void FrmMain::startRecorder(Audio* audio)
{
	printf("record thread started\n");
	float time1, time2;
	time1 = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
	printf("time 1: %f\n", time1);
	//task 1
	audio->DigitalFilterInit(IIR);
	//task 2
	audio->Record();
	// task 3 normalize data
	audio->NormalizeData(audio->wavBufferDouble, audio->bufferLength, 4, true);
	//task 4
	audio->FilterVoiceData();
	//task 5 un-normalize data
	audio->NormalizeData(audio->wavBufferDoubleOutput, audio->bufferLength, 0, false);
	// task 6
	audio->MixAudio();
	// task 7 save filtered voice data
	audio->SaveVoiceData();
	time2 = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
	printf("time 2: %f\n", time2);

	printf("thread time: %f\n", (time2 - time1));
}

//void FrmMain::cleanUpHeap(Audio* audio, Timer * timer)
//{
//	printf("clean up thread started\n");
//	while(MidiGlobalData::GetRecording() == true);
//	//delete timer;
//	//delete audio;
//}

void FrmMain::on_drum_1_button_clicked()
{
	// always play back sound - add to queue if recording
	PlaySound(TEXT("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/Closed-Hi-Hat-2.wav"), NULL, SND_FILENAME | SND_ASYNC);
	if(MidiGlobalData::GetRecording())
	{
		MusicData item;
		item.file = DRUM_1;
		item.msec = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
		MidiGlobalData::queue->enqueue(item);
	}
}

void FrmMain::on_drum_2_button_clicked()
{
	if(MidiGlobalData::GetRecording())
	{
		MusicData item;
		item.file = DRUM_2;
		item.msec = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
		MidiGlobalData::queue->enqueue(item);
	}
}

void FrmMain::on_drum_3_button_clicked()
{
	if(MidiGlobalData::GetRecording())
	{
		MusicData item;
		item.file = DRUM_3;
		item.msec = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
		MidiGlobalData::queue->enqueue(item);
	}
}

void FrmMain::on_drum_4_button_clicked()
{
	if(MidiGlobalData::GetRecording())
	{
		MusicData item;
		item.file = DRUM_4;
		item.msec = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
		MidiGlobalData::queue->enqueue(item);
	}
}

void FrmMain::on_btnVolDrum_1_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::SetDrumVol((double)btnVolCymbal4->get_value(), 1);
}

void FrmMain::on_btnVolDrum_2_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::SetDrumVol((double)btnVolCymbal4->get_value(), 2);
}

void FrmMain::on_btnVolDrum_3_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::SetDrumVol((double)btnVolCymbal4->get_value(), 3);
}

void FrmMain::on_btnVolDrum_4_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::SetDrumVol((double)btnVolCymbal4->get_value(), 4);
}

void FrmMain::on_cymbal_1_button_clicked()
{
	if(MidiGlobalData::GetRecording())
	{
		MusicData item;
		item.file = CYMBAL_1;
		item.msec = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
		MidiGlobalData::queue->enqueue(item);
	}
}

void FrmMain::on_cymbal_2_button_clicked()
{
	if(MidiGlobalData::GetRecording())
	{
		MusicData item;
		item.file = CYMBAL_2;
		item.msec = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
		MidiGlobalData::queue->enqueue(item);
	}
}

void FrmMain::on_cymbal_3_button_clicked()
{
	if(MidiGlobalData::GetRecording())
	{
		MusicData item;
		item.file = CYMBAL_3;
		item.msec = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
		MidiGlobalData::queue->enqueue(item);
	}
}

void FrmMain::on_cymbal_4_button_clicked()
{
	if(MidiGlobalData::GetRecording())
	{
		MusicData item;
		item.file = CYMBAL_4;
		item.msec = MidiGlobalData::timer->GetElapsedTimeMilliSeconds();
		MidiGlobalData::queue->enqueue(item);
	}
}

void FrmMain::on_btnVolCymbal_1_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::SetDrumVol((double)btnVolCymbal4->get_value(), 1);
}

void FrmMain::on_btnVolCymbal_2_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::SetDrumVol((double)btnVolCymbal4->get_value(), 2);
}

void FrmMain::on_btnVolCymbal_3_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::SetDrumVol((double)btnVolCymbal4->get_value(), 3);
}

void FrmMain::on_btnVolCymbal_4_value_changed(int data)
{
	// update global data with new volume.
	MidiGlobalData::SetDrumVol((double)btnVolCymbal4->get_value(), 4);
}

void FrmMain::on_record_button_clicked()
{

	MidiGlobalData::queue = new MixQueue(1000);
	MidiGlobalData::timer = new Timer();
	MidiGlobalData::audioCtrl = new Audio();

	printf("button clicked\n");

	long seconds = 0;
	Glib::ustring ustr = txtSeconds->get_text();
	std::stringstream s;
	s << ustr.raw();
	s >> seconds;
	MidiGlobalData::audioCtrl->SetSecondsToRecord(seconds);

	MidiGlobalData::timer->Start();

	std::thread recorderThread(startRecorder, MidiGlobalData::audioCtrl);
	recorderThread.detach();

}

unsigned __stdcall startPlayer (void* lpParam)
{
	printf("Thread started\n");
	PlaySound(TEXT("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void FrmMain::on_playback_button_clicked()
{

    std::ifstream infile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/test.wav", std::ios::binary);
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
    int testDataIndex = MidiGlobalData::audioCtrl->FindDataIndex(length, testBuffer);

	FILE * pFile1;
	pFile1 = fopen ("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test_data_lil_E.txt","w");

	int32_t sample;

	int16_t sample1;

	int16_t bs;
	char byte1;
	char byte2;
	uint32_t val;
	for (int j = testDataIndex; j < length; j+=2)
	{
		sample = 0;
		bs = 0;

		byte1 = testBuffer[j];
		byte2 = testBuffer[j+1];

		bs = byte2 << 8;
		bs |= (byte1 & 0xFF);

		fprintf(pFile1, "%d", bs);
		fprintf(pFile1, "\n");
	}
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




