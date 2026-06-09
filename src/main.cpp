#include <wx/wx.h>
#include "download.hpp"
#include "config.hpp"
#include <iostream>

DownloadHandler gDownload;
ConfigHandler gConfig;

class YanMusic : public wxApp {
public:
    bool OnInit() override;
};

bool YanMusic::OnInit() {
    wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "yan music");
    frame->Show(true);

    gConfig.init();

    for (std::string playlist : gConfig.config.playlists) {
        std::cout << playlist << "\n";
    }

    for (std::string flag : gConfig.config.ytdlpFlags) {
        std::cout << flag << "\n";
    }

    // gDownload.init("/home/nibbles/Music/yanmusic");
    // gDownload.downloadPlaylist("https://www.youtube.com/playlist?list=PLe1eTeDGapCwwQdNwf2_1d-Fam5YMyMwX", "/home/nibbles/Music/yanmusic");

    return true;
}

wxIMPLEMENT_APP(YanMusic);