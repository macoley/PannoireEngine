#ifndef PANNOIREENGINE_ENGINE_H
#define PANNOIREENGINE_ENGINE_H

#include "Core.h"

namespace PE::Engine {

    std::unique_ptr<Core> makeCore()
    {
        return std::make_unique<Core>();
    }


}

#endif //PANNOIREENGINE_ENGINE_H
