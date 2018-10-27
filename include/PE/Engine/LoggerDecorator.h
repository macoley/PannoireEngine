#ifndef PANNOIREENGINE_LOGGERDECORATOR_H
#define PANNOIREENGINE_LOGGERDECORATOR_H

#include "PE/Utils/Utils.h"

namespace PE::Engine {

    template <typename T>
    class LoggerDecorator : public T
    {
    public:
        template <typename ... Args>
        LoggerDecorator(const std::string& id, Args &&... args)
                : m_id(id),
                  T(std::forward<Args>(args)...)
        {
            Utils::log("Resource loaded: ""\"" + m_id + "\"");
        }

        virtual ~LoggerDecorator() {
            Utils::log("Resource destructed: ""\"" + m_id + "\"");
        }

    protected:
        std::string m_id;
    };

}

#endif //PANNOIREENGINE_LOGGERDECORATOR_H
