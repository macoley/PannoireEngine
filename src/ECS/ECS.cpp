
#include "PE/ECS/ECS.h"

namespace PE::ECS {

    std::shared_ptr<Manager> MakeECS() {
        return std::make_shared<Manager>();
    }

}