#include "../include/actionButtons.hpp"

actionButton::actionButton(
    wxWindow *parent,
    wxWindowID winID = wxID_ANY,
    const wxString &label = "",
    const wxPoint &pos = {-1, -1},
    const wxSize &size = {-1, -1},
    long style = 0
) : wxButton(parent, winID, label, pos, size, style) { }

void actionButton::initActionButton() {
    SetBackgroundColour(passiveColor);

    Bind(wxEVT_ENTER_WINDOW, &actionButton::onMouseEnter, this);
    Bind(wxEVT_LEAVE_WINDOW, &actionButton::onMouseLeave, this);
    Bind(wxEVT_BUTTON, &actionButton::onClick, this);
}

void actionButton::onMouseEnter(wxEvent &event) {
    SetBackgroundColour(hoverColor);
}

void actionButton::onMouseLeave(wxEvent &event) {
    SetBackgroundColour(passiveColor);
}

void actionButton::onClick(wxEvent &event) {
    performAction();
}

actionBitmapButton::actionBitmapButton(
    wxWindow *parent,
    wxWindowID winId = wxID_ANY,
    const wxBitmap &bitmap = wxBitmap(),
    const wxPoint &pos = {-1, -1},
    const wxSize &size = {-1, -1},
    long style = 0
) : actionButton(parent, winId, "", pos, size, style) {
    this->bitmap = bitmap;
    SetBitmap(this->bitmap);
}