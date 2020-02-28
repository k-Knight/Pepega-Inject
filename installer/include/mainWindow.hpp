#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>

class mainWindow : public wxFrame {
private:
    wxStaticBitmap *backgroundBitmap;
    wxWindow *title_panel, *content_panel;
    void onCloseWindow(wxEvent &event);

public:
    bool readyToClose;
    static mainWindow *window;

    mainWindow();
};