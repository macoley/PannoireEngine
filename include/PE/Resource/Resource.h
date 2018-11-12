#ifndef PE_RES_RESOURCE_H
#define PE_RES_RESOURCE_H

#include "ResourceManager.h"
#include "IResource.h"

namespace PE::Resource {

    std::shared_ptr<ResourceManager> MakeManager();

}

#endif //PE_RES_RESOURCE_H
