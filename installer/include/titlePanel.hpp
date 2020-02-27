#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>
#include "actionButtons.hpp"

class titlePanel : public wxPanel {
private:
    wxWindow *title_image, *close_button, *minimize_button;
    void addPanelControls();
public:
    titlePanel(wxWindow *parent);
    ~titlePanel();
};

class titleImage : public wxStaticBitmap {
public:
    titleImage(wxWindow *parent);
    ~titleImage();
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