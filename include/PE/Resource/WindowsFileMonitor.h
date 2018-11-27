#ifndef PE_RES_WINDOWSFILEMONITOR_H
#define PE_RES_WINDOWSFILEMONITOR_H

#include <thread>
#include <vector>
#include <unordered_map>

#include <windows.h>

#include "IFileMonitor.h"

namespace PE::Resource {

    class WindowsFileMonitor : public IFileMonitor {
    public:
        WindowsFileMonitor() = default;
        virtual ~WindowsFileMonitor() = default;

        void watchDirectory(const std::string& path, CallbackFnc callback) override;

    private:
        std::unordered_map<std::string, std::thread> m_watchers{};
    };

}

#endif //PE_RES_WINDOWSFILEMONITOR_H
