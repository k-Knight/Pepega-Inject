#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "utility.hpp"
#include <thread>
#include <memory>
#include <string>

enum installStatus {
    installing = 0,
    aborted,
    error,
    finished
};

struct installInfo {
    double progress;
    std::string *filePath;
    installStatus status;

    ~installInfo() {
        if (filePath != nullptr)
            delete filePath;
    }
};

class installer : public wxEvtHandler, public subscribable<installInfo&> {
private:
    const char *installRoot;
    bool proceedInstall = false;
    std::thread installThread;
    void install();
    void installFromResource(int index, long &completed);
    void notifyAsync(installInfo*);

public:
    void startInstall();
    void abortInstall();
    installer(const char *path);
};