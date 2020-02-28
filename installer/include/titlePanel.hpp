#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>
#include "actionButtons.hpp"

class titlePanel : public wxPanel {
private:
    wxWindow *title_image, *close_button, *minimize_button, *drag_box;
    void addPanelControls();
public:
    titlePanel(wxWindow *parent);
};

class dragBox : public wxControl {
private:
    wxPoint delta = {0, 0};
    void onLeftUp(wxMouseEvent &event);
    void onLeftDown(wxMouseEvent &event);
    void onMouseMove(wxMouseEvent &event);

public:
    dragBox(wxWindow *parent);
};

class titleImage : public wxStaticBitmap {
public:
    titleImage(wxWindow *parent);
};

class closeButton : public actionBitmapButton {
private:
    void performAction() override;
public:
    closeButton(wxWindow *parent);
};

class minimizeButton : public actionBitmapButton {
private:
    void performAction() override;
public:
    minimizeButton(wxWindow *parent);
};