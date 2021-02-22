#include "FrmMain.h"
#include "micInput.h"
#include "waveCapture.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wavelib.h>
#include <conio.h>

using namespace Gtk;

//MicInput *mic1;
//waveCapture *wav1;
//float meterVal;



FrmMain::FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
    Gtk::Window(cobject), builder(refGlade){

	builder->get_widget("btnRecord", btnRecord);
	builder->get_widget("btnPlayback", btnPlayback);
	builder->get_widget("btnStopPlayback", btnStopPlayback);
	builder->get_widget("txtSeconds", txtSeconds);
	txtSeconds->set_editable(TRUE);

	txtSeconds->signal_activate().connect(sigc::mem_fun(*this, &FrmMain::on_entry_activated));

	btnRecord->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_record_button_clicked));
	btnPlayback->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_playback_button_clicked));
	btnStopPlayback->signal_clicked().connect(sigc::mem_fun(*this, &FrmMain::on_stop_playback_button_clicked));

	//mic1 = new MicInput(1);
	//wav1 = new waveCapture(8000,8,1);
}


long seconds;

unsigned __stdcall startRecorder (void* lpParam)
{
	waveCapture *pwc = new waveCapture(44100, 16, 2);

	WORD numOutDevs = pwc->getWaveOutNumDevices();



	WORD numDevs = pwc->getWaveInNumDevices();
	LPSTR devName1 = pwc->getWaveInDevName(0);
	LPSTR devName2 = pwc->getWaveInDevName(1);
	WORD devMid1 = pwc->getWaveInDevManufacturerId(0);
	WORD devMid2 = pwc->getWaveInDevManufacturerId(1);
	LPSTR devoutName1 = pwc->getWaveOutDevName(0);
	LPSTR devoutName2 = pwc->getWaveOutDevName(1);
	LPSTR devoutName3 = pwc->getWaveOutDevName(2);
	LPSTR devoutName4 = pwc->getWaveOutDevName(3);
	//LPSTR devName3 = pwc->getWaveInDevName(2);
	//LPSTR devName4 = pwc->getWaveInDevName(3);

	printf("Number of input devices: %d \n",numDevs);
	printf("Number of output devices: %d \n",numOutDevs);



	//printf(devName1);
	std::cout << devName1;
	printf("\n");
	printf("Mid 1: %d \n",devMid1);
	//printf(devName2);
	std::cout << devName2;
	printf("\n");
	printf("Mid 2: %d \n",devMid2);

	//printf(devoutName1);
	std::cout << devoutName1;
	printf("\n");
	//printf(devoutName2);
	std::cout << devoutName2;
	printf("\n");
	//printf(devoutName3);
	std::cout << devoutName3;
	printf("\n");
	//printf(devoutName4);
	std::cout << devoutName4;
	printf("\n");


	DWORD bufferLength = pwc->getSuggestedBufferSize();


	if(pwc->start(0) == 0)
	{


		char* pWAVBuffer = new char[bufferLength];
		//printf("Buffer length: %d \n",bufferLength);
		pwc->createWAVEFile("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav");


		for (int i = 0; i < seconds; i++)
		{
			pwc->readBuffer(pWAVBuffer);
			//printf("buffer read\n");
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

	return 0;
}

void FrmMain::on_record_button_clicked()
{
	Glib::ustring ustr = txtSeconds->get_text();
	std::stringstream s;
	s << ustr.raw();
	s >> seconds;
	printf("Capturing audio\n");
	HANDLE hThread;
	unsigned ThreadId;

	// Start capturing Thread: thread[0]
	hThread = (HANDLE)_beginthreadex(NULL, 0, startRecorder,NULL, 0, &ThreadId);
	if (hThread == 0)
	{
		printf("Unable to start sound capture thread! \n\n");
		exit(1);
	}
}

unsigned __stdcall startPlayer (void* lpParam)
{
	printf("Thread started\n");
	PlaySound(TEXT("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void FrmMain::on_playback_button_clicked()
{

	bool pSound = PlaySound("C:\\Users\\cjgree13\\Documents\\CSE593\\MidiController\\MidiController\\bin\\test.wav", NULL, SND_FILENAME| SND_ASYNC);

	if(pSound)
	{
		printf("success");
	}

	/*

	HWAVELIB hWaveLib = NULL;

	 hWaveLib = WaveLib_Init("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/bin/test.wav", FALSE);

	 if(hWaveLib)
	 {
		 printf(" Press a key to stop> ");
		 getch();

		 WaveLib_UnInit(hWaveLib);
	 }
	 else
	 {
		 printf(" FAIL\n ");
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



