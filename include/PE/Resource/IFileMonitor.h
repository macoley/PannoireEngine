#ifndef PE_RES_IFILEMONITOR_H
#define PE_RES_IFILEMONITOR_H

#include <string>
#include <functional>

namespace PE::Resource {

    class IFileMonitor {
    protected:
        using CallbackFnc = std::function<void (const std::string&)>;

    public:
        virtual void watchDirectory(const std::string& path, CallbackFnc callback) = 0;
    };

}


#endif //PE_RES_IFILEMONITOR_H
