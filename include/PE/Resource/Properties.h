#ifndef PANNOIREENGINE_PROPERTIES_H
#define PANNOIREENGINE_PROPERTIES_H

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace PE::Resource {

    class Properites {
    public:
        explicit Properites(const std::string & path);
        Properites();
        virtual ~Properites();

        void load(const std::string & path);
        void save(const std::string & path);

        template<typename T>
        T get(const std::string & key);

        template<typename T>
        void set(const std::string & key, const T& value);

    private:
        std::string m_path;
        YAML::Node m_root;
    };

    template<typename T>
    T Properites::get(const std::string &key) {
        return m_root[key].as<T>();
    }

    template<typename T>
    void Properites::set(const std::string &key, const T &value) {
        m_root[key] = value;
    }
}

#endif //PANNOIREENGINE_PROPERTIES_H
