#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "contentPanel.hpp"
#include "actionButtons.hpp"
#include <memory>
#include <string>

class dirPickerButton : public actionButton, public subscribable<const char*> {
private:
    std::unique_ptr<char> path;
    void setPath(const char *path);

protected:
    void performAction();

public:
    dirPickerButton(wxWindow *parent, const char *path, wxPoint pos, wxSize size);
};

class folderSelect : public wxPanel {
private:
    contentPanel* parentPanel;
    wxWindow *inputPanel, *pathLabel, *instruction;
    wxTextCtrl *inputText;
    dirPickerButton *browseButton;
    std::string path;

    void selectDir(const char *path);
    void initInstallPath();
    void createTextInput();
    void createPathLabel();
    void createInstruction();
    void createBrowseButton();

public:
    folderSelect(contentPanel *parent);
    const char* getPath();
};

class installationStart : public contentContainer, public subscribable<const char *> {
private:
    wxBoxSizer* createTitleText();
    wxBoxSizer* createInstructionsText();
    void beginInstall();
    void setPath(const char *);
    folderSelect *dirSelect;

protected:
    void createContent();

public:
    installationStart(contentPanel *parent);
};