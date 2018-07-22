
#include "PE/Resource/BaseResource.h"

namespace PE::Resource {

    bool BaseResource::operator<(BaseResource &t_container) {
        if(m_priority < t_container.getPriority())
            return true;
        else if(m_priority > t_container.getPriority())
            return false;
        else
        {
            if(m_lastAccess < t_container.getLastAccess())
                return true;
            else if(m_lastAccess > t_container.getLastAccess())
                return false;
            else
            {
                return getSize() < t_container.getSize();
            }
        }
    }

}