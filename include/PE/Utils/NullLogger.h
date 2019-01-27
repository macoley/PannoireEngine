#ifndef PANNOIREENGINE_NULLLOGGER_H
#define PANNOIREENGINE_NULLLOGGER_H

#include <ctime>
#include <iostream>
#include <iomanip>

#include "ILogger.h"

namespace PE::Utils {

    class NullLogger
            : public ILogger
    {
    public:
        void log(const std::string&) const override;
    };

}

#endif //PANNOIREENGINE_NULLLOGGER_H
