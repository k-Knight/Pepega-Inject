#include "../include/titlePanel.hpp"

#include "../include/colors.hpp"
#include "../include/mainWindow.hpp"
#include "../include/scaledBitmap.hpp"
#include "../resources/include/resources.hpp"
#include "../include/os_preproc.hpp"

using namespace std;
using namespace Resources;

titlePanel::titlePanel(wxWindow *parent) : wxPanel(parent, wxID_ANY, {0, 0}, {700, 24}) {
    SetDoubleBuffered(true);
    SetBackgroundColour(colorDef.at(colors::background));

    addPanelControls();
}

void titlePanel::addPanelControls() {
    close_button = new closeButton(this);
    minimize_button = new minimizeButton(this);
    drag_box = new dragBox(this);
    title_image = new titleImage(this);
}

dragBox::dragBox(wxWindow *parent) : wxControl(parent, wxID_ANY, {0, 0}, {616, 24}) {
    
    wxColour transparentColor = wxColour(wxTransparentColour);

    #ifdef OS_WIN
        SetBackgroundColour(transparentColor);
        SetBackgroundStyle(wxBG_STYLE_CUSTOM);

        SetWindowLong(GetHandle(), GWL_EXSTYLE, GetWindowLong(GetHandle(), GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(GetHandle(), transparentColor.GetRGB(), 255, LWA_ALPHA);
        SetLayeredWindowAttributes(GetHandle(), transparentColor.GetRGB(), 255, LWA_COLORKEY);
    #endif
    
    Bind(wxEVT_LEFT_DOWN, &dragBox::onLeftDown, this);
    Bind(wxEVT_LEFT_UP, &dragBox::onLeftUp, this);
    Bind(wxEVT_MOTION, &dragBox::onMouseMove, this);
}

void dragBox::onLeftDown(wxMouseEvent &event) {
    CaptureMouse();

    auto pos = mainWindow::window->ClientToScreen(event.GetPosition());
    auto origin = mainWindow::window->GetPosition();

    delta = {pos.x - origin.x, pos.y - origin.y};
}

void dragBox::onLeftUp(wxMouseEvent &event) {
    if (HasCapture())
        ReleaseMouse();
}

void dragBox::onMouseMove(wxMouseEvent &event) {
    if (HasCapture() && event.Dragging() && event.LeftIsDown()) {
        auto pos = mainWindow::window->ClientToScreen(event.GetPosition());
        pos = {pos.x - delta.x, pos.y - delta.y};

        mainWindow::window->Move(pos);
    }
}

titleImage::titleImage(wxWindow *parent) : wxStaticBitmap(
    parent,
    wxID_ANY,
    scaledBitmap::fromMemory(
        display::resources[display::names::logo].data,
        display::resources[display::names::logo].size,
        587,
        24
    ),
    {2, 0}
) {
    //wxStaticBitmap::SetSize(GetBitmap().GetWidth(), GetBitmap().GetHeight());
    //mainWindow::window->makeDraggable(this);
}


closeButton::closeButton(wxWindow *parent) : actionBitmapButton(
    parent,
    wxID_ANY,
    scaledBitmap::fromMemory(
        display::resources[display::names::close].data,
        display::resources[display::names::close].size,
        42,
        24
    ),
    {658, 0},
    {42, 24},
    wxBORDER_NONE
) {

    passiveColor = colorDef.at(colors::background);
    hoverColor = colorDef.at(colors::hover_red);

    initActionButton();
}

void closeButton::performAction() {
    if(mainWindow::window->readyToClose) {
        mainWindow::window->Close();
    }
}

minimizeButton::minimizeButton(wxWindow *parent) : actionBitmapButton(
    parent,
    wxID_ANY,
    scaledBitmap::fromMemory(
        display::resources[display::names::minimize].data,
        display::resources[display::names::minimize].size,
        42,
        24
    ),
    {616, 0},
    {42, 24},
    wxBORDER_NONE
) {

    passiveColor = colorDef.at(colors::background);
    hoverColor = colorDef.at(colors::hover_light);

    initActionButton();
}

void minimizeButton::performAction() {
    mainWindow::window->Iconize();
}