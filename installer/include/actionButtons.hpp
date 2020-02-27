#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>

#ifndef PEPEGA_ACTION_BUTTONS
#define PEPEGA_ACTION_BUTTONS

class actionButton : public wxButton {
protected:
    class btnType;
    wxColour passiveColor, hoverColor;
    void onMouseEnter(wxEvent &event);
    void onMouseLeave(wxEvent &event);
    void onClick(wxEvent &event);

    virtual void performAction() = 0;
    virtual void initActionButton();
public:
    actionButton(wxWindow*, wxWindowID, const wxString&, const wxPoint&, const wxSize&, long);
};

class actionBitmapButton : public actionButton {
protected:
    wxBitmap bitmap;
public:
    actionBitmapButton(wxWindow*, wxWindowID, const wxBitmap&, const wxPoint&, const wxSize&, long);
};

#endif