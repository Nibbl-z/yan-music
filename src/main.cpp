#include <wx/wx.h>
#include "download.hpp"
#include "config.hpp"
#include <iostream>
#include "globals.hpp"

class YanMusic : public wxApp {
public:
    bool OnInit() override;
    int OnExit() override;
};

bool YanMusic::OnInit() {
    wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "yan music");
    frame->Show(true);

    gConfig.init();
    gDownload.initPlaylistDbFile();


    for (std::string playlist : gConfig.config.playlists) {
        std::cout << playlist << "\n";
    }

    for (std::string flag : gConfig.config.ytdlpFlags) {
        std::cout << flag << "\n";
    }

    // at some point when theres user inputted playlists
    // "/(?<=&list=).{34}/g" is the regex to get the id

    if (gConfig.config.musicDirectory != "") {
        bool success = gDownload.init(gConfig.config.musicDirectory);
        if (!success) {
            std::cout << "failed to initialize directory at " << gConfig.config.musicDirectory << "\n";
        } else {
            for (std::string playlist : gConfig.config.playlists) {
                gDownload.downloadPlaylist(playlist, gConfig.config.musicDirectory);
            }
        }
    }

    return true;
}

int YanMusic::OnExit() {
    gConfig.save();
    gDownload.saveDb();

    return 0;
}

wxIMPLEMENT_APP(YanMusic);