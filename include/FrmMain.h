#include <gtkmm.h>
#include "audio.h"
#include "timer.h"

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
    Gtk::Button *btnCymbal1;
    Gtk::Button *btnCymbal2;
    Gtk::Button *btnCymbal3;
    Gtk::Button *btnCymbal4;
    Gtk::VolumeButton *btnVolCymbal1;
    Gtk::VolumeButton *btnVolCymbal2;
    Gtk::VolumeButton *btnVolCymbal3;
    Gtk::VolumeButton *btnVolCymbal4;
    Gtk::Entry *txtSeconds;

    Timer *timer;
    Audio *audioCtrl;

public:

    FrmMain(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);//constructor

protected:
    //signal handlers
    void on_record_button_clicked();
    void on_playback_button_clicked();
    void on_stop_playback_button_clicked();
    void on_entry_activated();

    //drum controls signal handlers
    void on_drum_x_button_clicked(int data);
    void on_btnVolDrum_1_value_changed(int data);
    void on_btnVolDrum_2_value_changed(int data);
    void on_btnVolDrum_3_value_changed(int data);
    void on_btnVolDrum_4_value_changed(int data);

    //cymbal controls signal handlers
    void on_cymbal_x_button_clicked(int data);
    void on_btnVolCymbal_1_value_changed(int data);
    void on_btnVolCymbal_2_value_changed(int data);
    void on_btnVolCymbal_3_value_changed(int data);
    void on_btnVolCymbal_4_value_changed(int data);
    void static startRecorder(Audio* audio);
    void static cleanUpHeap(Audio* audio, Timer * timer);
};
