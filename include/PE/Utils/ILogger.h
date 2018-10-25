#ifndef PANNOIREENGINE_ILOGGER_H
#define PANNOIREENGINE_ILOGGER_H

#include <string>

namespace PE::Utils {

    class ILogger
    {
        public:
            virtual ~ILogger() {};
            virtual void log(const std::string&) const = 0;
    };

}

#endif //PANNOIREENGINE_ILOGGER_H
