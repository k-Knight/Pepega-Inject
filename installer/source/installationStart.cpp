#include "../include/installationStart.hpp"

#include <vector>
#include <functional>
#include <stdlib.h>
#include "../include/colors.hpp"
#include "../include/constants.hpp"
#include "../include/formatStringElement.hpp"

using namespace std;

class installButton : public actionButton, public subscribable<> {
protected:
    void performAction() {
        notifySubscriber();
    }

public:
    installButton(wxWindow *parent, wxPoint pos, wxSize size)
        : actionButton(parent, wxID_ANY, "Install", pos, size, wxBORDER_NONE) {

        passiveColor = colorDef.at(colors::button);
        hoverColor = colorDef.at(colors::button_hover);

        SetFont(
            wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Roboto Slab")
        );
        SetForegroundColour(colorDef.at(colors::content));

        initActionButton();
    }
};



void dirPickerButton::setPath(const char *path) {
    size_t size = strlen(path) + 1;
    this->path = unique_ptr<char>(new char[size]);
    strncpy(this->path.get(), path, size);
}

void dirPickerButton::performAction() {
    wxDirDialog *dialog = new wxDirDialog(this, "Select Folder", path.get(), wxDD_DEFAULT_STYLE);
    dialog->ShowModal();

    setPath(dialog->GetPath());
    delete dialog;

    notifySubscriber(path.get());
}

dirPickerButton::dirPickerButton(wxWindow *parent, const char *path, wxPoint pos, wxSize size)
    : actionButton(parent, wxID_ANY, "Select Folder", pos, size, wxBORDER_NONE) {

    passiveColor = colorDef.at(colors::button);
    hoverColor = colorDef.at(colors::button_hover);
    setPath(path);

    SetFont(
        wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Roboto Slab")
    );
    SetForegroundColour(colorDef.at(colors::hover_light));

    initActionButton();
}



void folderSelect::selectDir(const char *path) {
    this->path = path;
    inputText->SetValue(this->path.c_str());
}

void folderSelect::initInstallPath() {
    path = getenv("USERPROFILE");
    path += PEPEGA_DEFAULT_INTALL_FOLDER;
}

void folderSelect::createTextInput() {
    int textPanelWidth = parentPanel->containerWidth - 95;
    wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Roboto Slab");

    inputPanel = new wxPanel(this, wxID_ANY, {75, 90}, {textPanelWidth, 26});
    inputPanel->SetBackgroundColour(colorDef.at(colors::text_input));

    inputText = new wxTextCtrl(inputPanel, wxID_ANY, path.c_str(), {6, 3}, {textPanelWidth - 12, 20}, wxNO_BORDER);
    inputText->SetBackgroundColour(colorDef.at(colors::text_input));
    inputText->SetForegroundColour(colorDef.at(colors::main_text));
    inputText->SetFont(font);
}

void folderSelect::createPathLabel() {
    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Roboto Slab");

    pathLabel = new wxStaticText(this, wxID_ANY, "Folder:", {20, 92});
    pathLabel->SetForegroundColour(colorDef.at(colors::secondary_text));
    pathLabel->SetFont(font);
}

void folderSelect::createInstruction() {
    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Roboto Slab");

    instruction = new wxStaticText(this, wxID_ANY, "Select Discord\'s core module folder:", {20, 40});
    instruction->SetForegroundColour(colorDef.at(colors::main_text));
    instruction->SetFont(font);
}

void folderSelect::createBrowseButton() {
    browseButton = new dirPickerButton(this, path.c_str(), {parentPanel->containerWidth - 120, 140}, {100, 30});
    browseButton->subscribe(this, &folderSelect::selectDir);
}

folderSelect::folderSelect(contentPanel *parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, {parent->containerWidth, 210}) {
    parentPanel = parent;
    SetBackgroundColour(colorDef.at(colors::hover_light));
    initInstallPath();

    createTextInput();
    createPathLabel();
    createInstruction();
    createBrowseButton();
}

const char* folderSelect::getPath() {
    return path.c_str();
}



installationStart::installationStart(contentPanel *parent) : contentContainer(parent) {
    parentPanel = parent;
    createContent();
    parent->Layout();
}

void installationStart::beginInstall() {
    size_t size = strlen(dirSelect->getPath()) + 1;
    char *copy = new char[size];
    strncpy(copy, dirSelect->getPath(), size);

    notifySubscriber(copy);
}

void installationStart::createContent() {
    AddSpacer(30);
    Add(createTitleText(), 0);
    AddSpacer(40);
    Add(createInstructionsText(), 0, wxEXPAND);
    AddSpacer(30);

    dirSelect = new folderSelect(parentPanel);
    Add(dirSelect, 0, wxEXPAND);

    (new installButton(parentPanel, {415, 408}, {100, 30}))->subscribe(this, &installationStart::beginInstall);
}

wxBoxSizer* installationStart::createTitleText() {
    vector<formatStringElement> linePieces = {
        { "Install", 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, colorDef.at(colors::main_text), 40 },
        { " Pepega-Inject  ", 20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, colorDef.at(colors::main_text), 40 },
        { PEPEGA_VERSION_NUMBER, 14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, colorDef.at(colors::main_text), 33 }
    };

    return createFormatedString(20, linePieces);
}

wxBoxSizer* installationStart::createInstructionsText() {
    wxBoxSizer *instructionsSizer = new wxBoxSizer(wxVERTICAL);

    vector<formatStringElement> linePieces = {
        { "Select the direcory where the", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::main_text), -1 },
        { " Pepega Inject ", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, colorDef.at(colors::main_text), -1 },
        { "should be installed to.", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::main_text), -1 }
    };
    instructionsSizer->Add(createFormatedString(20, linePieces), 0);

    linePieces = vector<formatStringElement> {
        { "The folder should contain \'", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::secondary_text), -1 },
        { "index.js", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, colorDef.at(colors::secondary_text), -1 },
        { "\' scirpt and \'", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::secondary_text), -1 },
        { "core.asar", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, colorDef.at(colors::secondary_text), -1 },
        { "\' archive files.", 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, colorDef.at(colors::secondary_text), -1 },
    };
    instructionsSizer->Add(createFormatedString(20, linePieces), 0);

    return instructionsSizer;
}