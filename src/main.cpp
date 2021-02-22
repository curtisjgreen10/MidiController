#include <FrmMain.h>
#include <gtkmm.h>

int main(int argc, char* argv[])
{
	/*
	// Initialize gtkmm
	Gtk::Main app(argc, argv);
	// Create the window
	mainwindow w;
	// Start main loop
	Gtk::Main::run(w);
	return 0;
	*/
	Gtk::Main kit(argc,argv);
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("C:/Users/cjgree13/Documents/CSE593/MidiController/MidiController/src/midigui.glade");

	FrmMain *frm = 0;
	builder->get_widget_derived("frmMain", frm);
	kit.run(*frm);

	  return 0;

}


