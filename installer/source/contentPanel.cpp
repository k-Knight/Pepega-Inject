#include "../include/contentPanel.hpp"

#include "../include/colors.hpp"
#include "../include/mainWindow.hpp"
#include "../include/installationStart.hpp"
#include "../include/installationProcess.hpp"
#include "../include/installationFinish.hpp"

using namespace std;

contentContainer::contentContainer(contentPanel *parent) : wxBoxSizer(wxVERTICAL) {
    this->parentPanel = parent;
    
    parent->SetSizer(this, true);
}

wxBoxSizer* contentContainer::createFormatedString(int padding, vector<formatStringElement> stringPieces) {
    wxBoxSizer *stringSizer = new wxBoxSizer(wxHORIZONTAL);
    stringSizer->AddSpacer(padding);

    for (formatStringElement &stringPiece: stringPieces) {
        wxFont font(stringPiece.fontSize, stringPiece.fontFamily, stringPiece.fontStyle, stringPiece.fontWeight, false, "Roboto Slab");
        wxStaticText* text = new wxStaticText(parentPanel, wxID_ANY, stringPiece.text, wxDefaultPosition, {-1, stringPiece.height});
        text->SetForegroundColour(stringPiece.color);
        text->SetFont(font);

        stringSizer->Add(text, 0, wxALIGN_BOTTOM);
    }

    return stringSizer;
}

contentPanel::contentPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY, {164, 24}, {536, 451}) {
    Freeze();
    SetDoubleBuffered(true);
    containerWidth_ = 536;
    SetBackgroundColour(colorDef.at(colors::content));

    contentSizer = new installationStart(this);
    ((installationStart*)contentSizer)->subscribe(this, &contentPanel::beginInstallation);
    Thaw();
}

void contentPanel::beginInstallation(const char *path) {
    Freeze();
    mainWindow::window->readyToClose = false;
    DestroyChildren();

    contentSizer = new installationProcess(this, path);
    ((installationProcess*)contentSizer)->subscribe(this, &contentPanel::finishInstall);
    Thaw();
    ((installationProcess*)contentSizer)->beginInstall();
}

void contentPanel::finishInstall(bool success) {
    Freeze();
    mainWindow::window->readyToClose = true;
    DestroyChildren();

    contentSizer = new installationFinish(this);
    ((installationFinish*)contentSizer)->subscribe(this, &contentPanel::closeInstall);
    Thaw();
    Update();
}

void contentPanel::closeInstall() {
    mainWindow::window->Close();
}