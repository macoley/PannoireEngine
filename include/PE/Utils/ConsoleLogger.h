#ifndef PANNOIREENGINE_CONSOLELOGGER_H
#define PANNOIREENGINE_CONSOLELOGGER_H

#include <ctime>
#include <iostream>
#include <iomanip>

#include "ILogger.h"

namespace PE::Utils {

    class ConsoleLogger
            : public ILogger
    {
    public:
        void log(const std::string&) const override;
    };

}

#endif //PANNOIREENGINE_CONSOLELOGGER_H
