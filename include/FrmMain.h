#include <gtkmm.h>

class FrmMain : public Gtk::Window{
protected:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Button *btnRecord;
    Gtk::Button *btnPlayback;
    Gtk::Button *btnStopPlayback;
    Gtk::Entry *txtSeconds;

public:

    FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);//constructor

protected:
    //signal handlers
    void on_record_button_clicked();
    void on_playback_button_clicked();
    void on_stop_playback_button_clicked();
    void on_entry_activated();
};
