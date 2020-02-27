#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <memory>
#include "contentPanel.hpp"
#include "installer.hpp"

class progressBar : public wxPanel {
private:
    int barWidth, barHeight;
    wxPanel *progressPanel;
    void createMovablePanles();

public:
    progressBar(wxWindow *parent, wxPoint pos, wxSize size);
    void setProgress(double value);
};

class installProcessIndicator : public wxPanel {
private:
    contentPanel *parentPanel;
    wxStaticText *actionText, fileName;
    progressBar *progBar;

    void createLabel();
    void createStatusText();
    void createProgressBar();

public:
    installProcessIndicator(contentPanel *parent);
    void setProgress(double value);
};

class installationProcess : public contentContainer, public subscribable<bool> {
private:
    contentPanel *parentPanel;
    installer *__installer;
    std::unique_ptr<const char> path;
    installProcessIndicator *processIndicator;
    wxBoxSizer* createTitleText();
    wxBoxSizer* createInfoText();
    void closeInstall();
    void abortInstall();
    void onInstallEvent(installInfo &event);

protected:
    void createContent();

public:
    void beginInstall();
    installationProcess(contentPanel *parent, const char *path);
};