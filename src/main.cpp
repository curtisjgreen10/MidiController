#include "FrmMain.h"

int main(int argc, char* argv[])
{

	Gtk::Main kit(argc,argv);
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/src/midigui.glade");

	FrmMain *frm = 0;
	builder->get_widget_derived("frmMain", frm);
	kit.run(*frm);

	return 0;

}


