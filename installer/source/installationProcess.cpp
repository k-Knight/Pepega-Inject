#include "../include/installationProcess.hpp"

#include <vector>
#include <cmath>
#include "../include/colors.hpp"
#include "../include/mainWindow.hpp"
#include "../include/actionButtons.hpp"
#include "../include/utility.hpp"

using namespace std;

class abortButton : public actionButton, public subscribable<> {
protected:
    void performAction() {
        notifySubscriber();
    }
public:
    abortButton(wxWindow *parent, wxPoint pos, wxSize size)
        : actionButton(parent, wxID_ANY, "Abort", pos, size, wxBORDER_NONE) {

        passiveColor = colorDef.at(colors::button);
        hoverColor = colorDef.at(colors::hover_red);

        SetForegroundColour(colorDef.at(colors::content));
        SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Roboto Slab"));

        initActionButton();
    }
};


void progressBar::createMovablePanles() {
    (new wxPanel(this, wxID_ANY, {3, 3}, {barWidth - 6, barHeight - 6}))->SetBackgroundColour(colorDef.at(colors::background));
    (new wxPanel(this, wxID_ANY, {3, 3}, {barWidth - 6, barHeight - 6}))->SetBackgroundColour(colorDef.at(colors::progress_pending));

    progressPanel = new wxPanel(this, wxID_ANY, {5, 5}, {0, 0});
    progressPanel->SetBackgroundColour(colorDef.at(colors::progress));
}

progressBar::progressBar(wxWindow *parent, wxPoint pos, wxSize size) : wxPanel(parent, wxID_ANY, pos, size) {
    barWidth = size.x;
    barHeight = size.y;

    SetBackgroundColour(colorDef.at(colors::content));
    createMovablePanles();
}

void progressBar::setProgress(double value) {
    value = value > 1.0 ? 1.0 : value < 0.0 ? 0.0 : value;
    progressPanel->SetSize({round((barWidth - 10) * value), barHeight - 10});
}



void installProcessIndicator::createLabel() {
    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Roboto Slab");

    actionText = new wxStaticText(this, wxID_ANY, " ", {20, 50});
    actionText->SetForegroundColour(colorDef.at(colors::main_text));
    actionText->SetFont(font);
}

void installProcessIndicator::createStatusText() {
    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Roboto Slab");

    actionText = new wxStaticText(this, wxID_ANY, " ", {40, 80}, {parentPanel->containerWidth - 60, -1});
    actionText->SetForegroundColour(colorDef.at(colors::secondary_text));
    actionText->SetFont(font);
}

void installProcessIndicator::createProgressBar() {
    progBar = new progressBar(this, {20, 120}, {parentPanel->containerWidth - 40, 32});
}

installProcessIndicator::installProcessIndicator(contentPanel *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, {parent->containerWidth, 210}) {

    parentPanel = parent;
    SetBackgroundColour(colorDef.at(colors::hover_light));
    createLabel();
    createStatusText();
    createProgressBar();
}

void installProcessIndicator::setProgress(double value) {
    progBar->setProgress(value);
}



installationProcess::installationProcess(contentPanel *parent, const char *path) : contentContainer(parent) {
    parentPanel = parent;
    createContent();
    parentPanel->Layout();
    this->path = unique_ptr<const char>(path);
}

void installationProcess::createContent() {
    AddSpacer(30);
    Add(createTitleText(), 0);
    AddSpacer(40);
    Add(createInfoText(), 0, wxEXPAND);
    AddSpacer(30);

    auto abortBtn = new abortButton(parentPanel, {415, 408}, {100, 30});
    abortBtn->subscribe(this, &installationProcess::abortInstall);

    processIndicator = new installProcessIndicator(parentPanel);
    Add(processIndicator, 0, wxEXPAND);
}

wxBoxSizer* installationProcess::createTitleText() {
    vector<formatStringElement> linePieces = {
        { "Installing ", 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, colorDef.at(colors::main_text), 40 },
        { "Pepega-Inject ", 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, colorDef.at(colors::main_text), 40 },
        { "...", 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, colorDef.at(colors::main_text), 40 }
    };

    return createFormatedString(20, linePieces);
}

wxBoxSizer* installationProcess::createInfoText() {
    vector<formatStringElement> linePieces = {
        {
            "Please wait while the injector is being installed. ",
            11,
            wxFONTFAMILY_DEFAULT,
            wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL,
            colorDef.at(colors::main_text),
            30
        }
    };

    return createFormatedString(20, linePieces);
}

void installationProcess::onInstallEvent(installInfo &event) {
    switch (event.status) {
        case installStatus::installing:
            processIndicator->setProgress(event.progress);
            break;

        case installStatus::aborted:
            delete __installer;
            //notifySubscriber(false);
            closeInstall();
            break;

        case installStatus::error:
            cout << "Error writing file :: [" << event.filePath << "]\n";

        case installStatus::finished:
            delete __installer;
            notifySubscriber(true);
            break;

        default: break;
    }    
}

void installationProcess::beginInstall() {
    __installer = new installer(path.get());
    __installer->subscribe(this, &installationProcess::onInstallEvent);
    __installer->startInstall();
}

void installationProcess::closeInstall() {
    if (mainWindow::window->readyToClose)
        parentPanel->closeInstall();
    else {
        int answer = wxMessageBox(
            "Are you sure you want to stop the installation?",
            "Abort Installation",
            wxYES_NO | wxICON_EXCLAMATION,
            mainWindow::window
        );

        if (answer == wxYES)
            parentPanel->closeInstall();
    }
}

void installationProcess::abortInstall() {
    __installer->abortInstall();
}