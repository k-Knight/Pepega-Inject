#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>

class mainWindow : public wxFrame {
private:
    wxPoint delta;
    wxStaticBitmap *backgroundBitmap;
    wxWindow *title_panel, *content_panel;
    void onLeftUp(wxMouseEvent &event);
    void onLeftDown(wxMouseEvent &event);
    void onMouseMove(wxMouseEvent &event);
    void onCloseWindow(wxEvent &event);

public:
    bool readyToClose;
    static mainWindow *window;

    mainWindow();
    void makeDraggable(wxWindow *object);
    void unmakeDraggable(wxWindow *object);
};