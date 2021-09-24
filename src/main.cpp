/*
 * main.cpp
 *
 *  Created on: Mar 4, 2021
 *      Author: cjgree13
 */

#include "FrmMain.h"
#include "main.h"


using namespace std;
MidiGlobalData* globalData;

int main(int argc, char* argv[])
{
	globalData = new MidiGlobalData();
	Gtk::Main kit(argc,argv);
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/src/midigui.glade");
	FrmMain *frm = 0;
	builder->get_widget_derived("frmMain", frm);
	kit.run(*frm);
	return 0;
}


