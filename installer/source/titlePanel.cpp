#include "../include/titlePanel.hpp"

#include "../include/colors.hpp"
#include "../include/mainWindow.hpp"
#include "../include/scaledBitmap.hpp"
#include "../resources/include/resources.hpp"

using namespace std;
using namespace Resources;

titlePanel::titlePanel(wxWindow *parent) : wxPanel(parent, wxID_ANY, {0, 0}, {700, 24}) {
    SetDoubleBuffered(true);
    SetBackgroundColour(colorDef.at(colors::background));

    mainWindow::window->makeDraggable(this);
    addPanelControls();
}

titlePanel::~titlePanel() {
    mainWindow::window->unmakeDraggable(this);
    DestroyChildren();
}

void titlePanel::addPanelControls() {
    title_image = new titleImage(this);
    close_button = new closeButton(this);
    minimize_button = new minimizeButton(this);
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
    wxStaticBitmap::SetSize(GetBitmap().GetWidth(), GetBitmap().GetHeight());
    mainWindow::window->makeDraggable(this);
}

titleImage::~titleImage() {
    mainWindow::window->unmakeDraggable(this);
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