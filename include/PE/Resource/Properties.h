#ifndef PE_RES_PROPERTIES_H
#define PE_RES_PROPERTIES_H

#include <fstream>

#include <yaml-cpp/yaml.h>

#include "Resource.h"
#include "PE/Utils/Utils.h"

namespace PE::Resource {

    class Properties : public IResource<Properties> {
    public:
        Properties() = default;
        virtual ~Properties() = default;

        void load(const std::string & path);
        void save(const std::string & path);

        template<typename T>
        T get(const std::string & key) const;

        template<typename T>
        void set(const std::string & key, const T& value);

    private:
        std::string m_path;
        YAML::Node m_root{};
    };

    template<typename T>
    T Properties::get(const std::string &key) const {
        return m_root[key].as<T>();
    }

    template<typename T>
    void Properties::set(const std::string &key, const T &value) {
        m_root[key] = value;
    }
}

#endif //PE_RES_PROPERTIES_H
