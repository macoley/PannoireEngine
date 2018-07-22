#ifndef PANNOIREENGINE_RES_UTILS_H
#define PANNOIREENGINE_RES_UTILS_H

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <utility>

namespace PE::Resource {

    /**
     * ID resource
     */
     using resourceID = size_t;

    /**
     * Resources Path
     */
    constexpr const char * resourceDir = "resources";

    /**
     * Extension
     */
    constexpr const char * resourceExtension = "res";

    /**
     * Generate hash from file path
     * @param path
     * @return
     */
    inline resourceID getID(const std::string& path) {
        return std::hash<std::string>{}(path);
    }

    /**
     * Get Resource Filename
     * @param path
     * @return
     */
    inline std::string getResourcePath(const std::string& path) {
        return std::string(resourceDir) + '/' + std::to_string(getID(path)) + '.' + std::string(resourceExtension);
    }

    /**
     * Get Resource Path from ResourceID
     * @param id
     * @return
     */
    inline std::string getResourcePathFromID(resourceID id) {
        return std::string(resourceDir) + '/' + std::to_string(id) + '.' + std::string(resourceExtension);
    }

    /**
     * Save Resource
     * @tparam R
     * @param path
     * @param res
     */
    template<class R>
    void saveResource(const std::string& path, const R& res)
    {
        std::ofstream ostrm(path, std::ios::binary);
        ostrm.write(reinterpret_cast<const char*>(&res), sizeof res); // binary output
    }

    /**
     * Load Resource
     * @tparam R
     * @param path
     * @param res
     */
    template<class R>
    void loadResource(const std::string& path, R& res)
    {
        std::ifstream istrm(path, std::ios::binary);
        istrm.read(reinterpret_cast<char*>(&res), sizeof res);
    }
}

#endif //PANNOIREENGINE_RES_UTILS_H
