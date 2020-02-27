#include "../include/mainWindow.hpp"

#include "../include/colors.hpp"
#include "../include/scaledBitmap.hpp"
#include "../include/titlePanel.hpp"
#include "../include/contentPanel.hpp"
#include "../resources/include/resources.hpp"

using namespace std;

mainWindow *mainWindow::window = nullptr;

mainWindow::mainWindow() : wxFrame(NULL, wxID_ANY, "", wxDefaultPosition, wxSize(700, 480), wxNO_BORDER | wxSIMPLE_BORDER, "parent_frame") {
    SetDoubleBuffered(true);
    readyToClose = true;
    SetBackgroundColour(colorDef.at(colors::background));
    Centre(wxBOTH);

    using namespace Resources;
    backgroundBitmap = new wxStaticBitmap(
        this,
        wxID_ANY,
        scaledBitmap::fromMemory(display::resources[display::names::background].data,
        display::resources[display::names::background].size, 696, 451),
        {0, 24},
        {164, 451}
    );

    title_panel = new titlePanel(this);
    content_panel = new contentPanel(this);

    Bind(wxEVT_CLOSE_WINDOW, &mainWindow::onCloseWindow, this);
}

void mainWindow::onCloseWindow(wxEvent &event) {
    std::terminate();
}

void mainWindow::onLeftDown(wxMouseEvent &event) {
    CaptureMouse();

    auto pos = mainWindow::window->ClientToScreen(event.GetPosition());
    auto origin = mainWindow::window->GetPosition();

    delta = {pos.x - origin.x, pos.y - origin.y};
}

void mainWindow::onLeftUp(wxMouseEvent &event) {
    if (HasCapture())
        ReleaseMouse();
}

void mainWindow::onMouseMove(wxMouseEvent &event) {
    if (HasCapture() && event.Dragging() && event.LeftIsDown()) {
        auto pos = mainWindow::window->ClientToScreen(event.GetPosition());
        pos = {pos.x - delta.x, pos.y - delta.y};

        mainWindow::window->Move(pos);
    }
}

void mainWindow::makeDraggable(wxWindow* wx_object) {
    wx_object->Bind(wxEVT_LEFT_DOWN, &mainWindow::onLeftDown, this);
    wx_object->Bind(wxEVT_LEFT_UP, &mainWindow::onLeftUp, this);
    wx_object->Bind(wxEVT_MOTION, &mainWindow::onMouseMove, this);
}

void mainWindow::unmakeDraggable(wxWindow* wx_object) {
    wx_object->Unbind(wxEVT_LEFT_DOWN, &mainWindow::onLeftDown, this);
    wx_object->Unbind(wxEVT_LEFT_UP, &mainWindow::onLeftUp, this);
    wx_object->Unbind(wxEVT_MOTION, &mainWindow::onMouseMove, this);
}