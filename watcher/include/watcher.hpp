#include <efsw.hpp>
#include <string>

class Watcher : public efsw::FileWatchListener {
private:
    void handleFileAction(efsw::WatchID, const std::string&, const std::string&, efsw::Action, std::string = "");
public:
    Watcher();
};