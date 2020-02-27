#include "../include/installationFinish.hpp"

#include "../include/colors.hpp"
#include "../include/actionButtons.hpp"
#include <vector>

using namespace std;

class exitInstallerButton : public actionButton, public subscribable<> {
protected:
    void performAction() {
        notifySubscriber();
    }

public:
    exitInstallerButton(wxWindow *parent, wxPoint pos, wxSize size)
        : actionButton(parent, wxID_ANY, "Finish", pos, size, wxBORDER_NONE) {

        passiveColor = colorDef.at(colors::button);
        hoverColor = colorDef.at(colors::button_hover);

        SetFont(
            wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Roboto Slab")
        );
        SetForegroundColour(colorDef.at(colors::content));

        initActionButton();
    }
};

installationFinish::installationFinish(contentPanel* parent) : contentContainer(parent) {
    parentPanel = parent;
    createContent();
    parentPanel->Layout();
}

void installationFinish::createContent() {
    AddSpacer(30);
    Add(createTitleText(), 0);
    AddSpacer(40);
    Add(createInfoText(), 0, wxEXPAND);

    (new exitInstallerButton(parentPanel, {415, 408}, {100, 30}))->subscribe(this, &installationFinish::exitInstall);
}

wxBoxSizer* installationFinish::createTitleText() {
    vector<formatStringElement> linePieces = {
        { "Installation Complete", 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, colorDef.at(colors::main_text), 40 }
    };

    return createFormatedString(20, linePieces);
}

wxBoxSizer*  installationFinish::createInfoText() {
    wxBoxSizer* infoSizer =  new wxBoxSizer(wxVERTICAL);

    vector<formatStringElement> linePieces = {
        { "The", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::main_text), 30 },
        { "Pepega Inject", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, colorDef.at(colors::main_text), 30 },
        { "was successfully installed", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::main_text), 30 }
    };
    infoSizer->Add(createFormatedString(20, linePieces), 0);

    infoSizer->AddSpacer(30);

    linePieces = {
        { "You may now exist the installer. Be sure to restart the ", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::secondary_text), 30 },
        { "Discord", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, colorDef.at(colors::main_text), 30 }
    };
    infoSizer->Add(createFormatedString(20, linePieces), 0);

    linePieces = {
        { "Application", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, colorDef.at(colors::main_text), 30 },
        { " to apply the changes ", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::secondary_text), 30 }
    };
    infoSizer->Add(createFormatedString(20, linePieces), 0);

    return infoSizer;
}

void installationFinish::exitInstall() {
    notifySubscriber();
}
