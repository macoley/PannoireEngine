#ifndef PE_RES_RESOURCECOUNTER_H
#define PE_RES_RESOURCECOUNTER_H

#include "Defines.h"
#include <iostream>

namespace PE::Resource {

    using FamilyIndex = std::size_t;

    /**
     * Base Resource Counter
     */
    struct BaseResourceCounter {
    protected:
        static FamilyIndex m_family_counter;
    };

    /**
     * Resource Counter
     */
    template<typename Resource>
    struct ResourceCounter : public BaseResourceCounter {
        static FamilyIndex getFamily();
    };

    template<typename Resource>
    FamilyIndex ResourceCounter<Resource>::getFamily() {
        static FamilyIndex family = m_family_counter++;

        std::cout << "Resource index" << std::endl;
        std::cout << std::to_string(family) << std::endl;
        std::cout << typeid(Resource).name() << std::endl;

        return family;
    }

}

#endif //PE_RES_RESOURCECOUNTER_H
