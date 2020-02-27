#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../include/mainWindow.hpp"
#include "../resources/include/resources.hpp"
#include <memory>
#include "../include/os_preproc.hpp"

#ifdef OS_WIN
    #include <wingdi.h>
#endif

using namespace std;

class installerApp : public wxApp {
public:
    virtual bool OnInit();
    virtual int OnExit();
    virtual void OnUnhandledException();
};

bool installerApp::OnInit() {
    #ifdef OS_WIN
        using namespace Resources;
        DWORD loaded;
        AddFontMemResourceEx(
            const_cast<unsigned char*>(display::resources[display::names::RobotoSlab].data),
            display::resources[display::names::RobotoSlab].size,
            0,
            &loaded
        );
    #endif

    wxImage::AddHandler(new wxPNGHandler);
    mainWindow::window = new mainWindow();
    mainWindow::window->Show(true);

    return true;
}

int installerApp::OnExit() {
    if (mainWindow::window != nullptr)
        mainWindow::window->Destroy();

    return 0;
}

void installerApp::OnUnhandledException() {
    int i = 1 + 1;
}

wxIMPLEMENT_APP(installerApp);