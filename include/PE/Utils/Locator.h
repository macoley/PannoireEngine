#ifndef PANNOIREENGINE_LOCATOR_H
#define PANNOIREENGINE_LOCATOR_H

#include "ILogger.h"

namespace PE::Utils {

    /**
     * Base Utility Class
     */
    class Locator
    {
    public:
        static ILogger* getLogger() { return m_logger_service; }

        static void provide(ILogger* service)
        {
            m_logger_service = service;
        }

    private:
        static ILogger* m_logger_service;
    };

}

#endif //PANNOIREENGINE_LOCATOR_H
