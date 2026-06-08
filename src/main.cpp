#include <wx/wx.h>
#include "downloader.hpp"

Downloader gDownloader;

class YanMusic : public wxApp {
public:
    bool OnInit() override;
};

bool YanMusic::OnInit() {
    wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "yan music");
    frame->Show(true);

    gDownloader.init("/home/nibbles/Music/yanmusic");
    gDownloader.downloadPlaylist("https://www.youtube.com/playlist?list=PLe1eTeDGapCwwQdNwf2_1d-Fam5YMyMwX", "/home/nibbles/Music/yanmusic");

    return true;
}

wxIMPLEMENT_APP(YanMusic);