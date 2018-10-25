
#include "PE/Utils/Utils.h"

namespace PE::Utils {

    void log(const std::string & text) {
        Locator::getLogger()->log(text);
    }

    void logError(const std::string & text) {
        Locator::getLogger()->log("[ERROR] " + text);
    }

}