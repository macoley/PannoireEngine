#ifndef PE_ECS_ECS_H
#define PE_ECS_ECS_H

#include <memory>

#include "Manager.h"

// Facade here

namespace PE::ECS {

    std::shared_ptr<Manager> MakeECS();

}

#endif //PE_ECS_ECS_H
