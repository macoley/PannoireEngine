
#include "PE/Resource/Resource.h"

namespace PE::Resource {

    std::shared_ptr<ResourceManager> MakeManager() {
        return std::make_shared<ResourceManager>();
    }

}