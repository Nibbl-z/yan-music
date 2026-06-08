#include <wx/wx.h>

class YanMusic : public wxApp {
public:
    bool OnInit() override;
};

bool YanMusic::OnInit() {
    wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "yan music");
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(YanMusic);