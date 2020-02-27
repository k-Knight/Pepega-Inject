#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <vector>
#include "formatStringElement.hpp"
#include "utility.hpp"

#ifndef PEPEGA_CONTENT
#define PEPEGA_CONTENT

class contentPanel;

class contentContainer : public wxBoxSizer {
protected:
    contentPanel *parentPanel;
    void virtual createContent() = 0;
    wxBoxSizer* createFormatedString(int padding, std::vector<formatStringElement> stringPieces);

public:
    contentContainer(contentPanel *parent);
};

class contentPanel : public wxPanel {
private:
    contentContainer *contentSizer;
    int containerWidth_;
    void beginInstallation(const char *path);
    void finishInstall(bool success);
    
public:
    void closeInstall();
    const int& containerWidth = containerWidth_;
    contentPanel(wxWindow *parent);
};

#endif