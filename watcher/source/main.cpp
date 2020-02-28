#include <efsw.hpp>
#include "../include/watcher.hpp"
#include <future>

using namespace efsw;
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 0;

    FileWatcher fileWatcher = FileWatcher();
    Watcher watcher = Watcher();

    fileWatcher.addWatch(argv[1], &watcher, true);
    fileWatcher.watch();

    promise<void>().get_future().wait();
    return 0;
}