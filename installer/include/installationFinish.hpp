#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "contentPanel.hpp"

class installationFinish : public contentContainer, public subscribable<> {
private:
    wxBoxSizer* createTitleText();
    wxBoxSizer* createInfoText();
    void exitInstall();

protected:
    void createContent();

public:
    installationFinish(contentPanel* parent);
};