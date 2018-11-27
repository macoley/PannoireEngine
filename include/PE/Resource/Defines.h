#ifndef PE_RES_DEFINES_H
#define PE_RES_DEFINES_H

#include <cstddef>
#include <string>

namespace PE::Resource {

    /**
     * ID resource
     */
    using ResourceIndex = std::size_t;

    /**
     * Events
     */
    enum class ResourceEvents {
        FILE_CHANGED
    };

    /*
     * Event struct
     */
    struct FileInfo {
        std::string m_path;
    };
}


#endif //PE_RES_DEFINES_H
