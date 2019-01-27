#ifndef PANNOIREENGINE_UTILS_H
#define PANNOIREENGINE_UTILS_H

#include "Locator.h"
#include "ConsoleLogger.h"
#include "NullLogger.h"

/**
 * Utils FACADE
 */
namespace PE::Utils {

    inline void log(const std::string & text) {
        Locator::getLogger()->log(text);
    }

    inline void logError(const std::string & text) {
        Locator::getLogger()->log("[ERROR] " + text);
    }

}

#endif //PANNOIREENGINE_UTILS_H
