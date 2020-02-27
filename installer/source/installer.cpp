#include "../include/installer.hpp"

#include <fstream>
#include <functional>
#include <string>
#include <cstring>
#include <algorithm>
#include <filesystem>
#include "../resources/include/resources.hpp"

using namespace std;
using namespace Resources;

bool createDirectories(const char *path) {
    bool result = false;

    #ifdef OS_WIN
        
    #endif

    return result;
}

string* formFilePath(const char *rootPath, const char *filePath) {
    string *result = new string(rootPath);
    replace(result->begin(), result->end(), '/', '\\');

    size_t pathSize = strlen(filePath);
    for (size_t i = 0; i < pathSize; i++)
        switch (filePath[i]) {
            case '.': {
                bool validsymbol = false;
                bool exit = false;
                size_t j = i + 1;

                for (; j < pathSize && !exit; j++)
                    switch (filePath[j]) {
                        case '.':
                            break;
                        case '/':
                        case '\\':
                            i = j - 1;
                            exit = true;
                            break;
                        default:
                            exit = true;
                            validsymbol = true;
                            j--;
                            break;
                    }

                if (validsymbol) {
                    for (; i <= j; i++)
                        *result += filePath[i];
                    
                    i--;
                }

                } break;
            case '/':
                *result += '\\'; break;
            default:
                *result += filePath[i]; break;
        }

    return result;
}

bool installFile(string &filePath, const unsigned char *data, unsigned long long size) {
    try {
        filesystem::path path {filePath};
        filesystem::create_directories(path.parent_path());

        ofstream file(path, ofstream::out | ofstream::binary);
        file.write((const char *)data, size);
        file.close();

        return true;
    } catch (...) {
        return false;
    }
}



installer::installer(const char *path) {
    installRoot = path;
}

void installer::startInstall() {
    proceedInstall = true;

    installThread = thread(&installer::install, this);
    installThread.detach();
}

void installer::abortInstall() {
    proceedInstall = false;
}

void installer::notifyAsync(installInfo *info) {
    CallAfter([this, info] {
        this->notifySubscriber(*info);
        delete info;
    });
}

void installer::installFromResource(int index, long &completed) {
    installInfo *info = new installInfo();

    info->filePath = formFilePath(installRoot, injector::resources[index].path);
    info->status = installFile(
        *(info->filePath),
        injector::resources[index].data,
        injector::resources[index].size
    ) ? installStatus::installing : installStatus::error;
    info->progress =  double(++completed) / double(injector::count);

    notifyAsync(info);
}

void installer::install() {
    long installedCount = 0;
    
    for (size_t i = 0; i < injector::count && proceedInstall; i++) {
        if (i == injector::names::index)
            continue;
        
        installFromResource(i, installedCount);
    }

    if (proceedInstall)
        installFromResource(injector::names::index, installedCount);

    installInfo *final = new installInfo();
    final->filePath = nullptr;

    if (proceedInstall) {
        final->progress = 1.0;
        final->status = installStatus::finished;
        notifyAsync(final);
    }
    else {
        final->progress = 0.0;
        final->status = installStatus::aborted;
        notifyAsync(final);
    }
}