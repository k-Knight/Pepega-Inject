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
    Destroy();
    mainWindow::window = nullptr;
}