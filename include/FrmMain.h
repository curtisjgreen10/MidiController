#include <gtkmm.h>

class FrmMain : public Gtk::Window{
protected:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Button *btnRecord;
    Gtk::Button *btnPlayback;
    Gtk::Button *btnStopPlayback;
    Gtk::Button *btnDrum1;
    Gtk::Button *btnDrum2;
    Gtk::Button *btnDrum3;
    Gtk::Button *btnDrum4;
    Gtk::VolumeButton *btnVolDrum1;
    Gtk::VolumeButton *btnVolDrum2;
    Gtk::VolumeButton *btnVolDrum3;
    Gtk::VolumeButton *btnVolDrum4;
    Gtk::Entry *txtSeconds;

public:

    FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);//constructor

protected:
    //signal handlers
    void on_record_button_clicked();
    void on_playback_button_clicked();
    void on_stop_playback_button_clicked();
    void on_entry_activated();
    void on_drum_x_button_clicked(int data);
    void on_btnVolDrum_1_value_changed(int data);
    void on_btnVolDrum_2_value_changed(int data);
    void on_btnVolDrum_3_value_changed(int data);
    void on_btnVolDrum_4_value_changed(int data);
};
