#include "PE/Resource/Properties.h"

namespace PE::Resource {


    Properites::Properites(const std::string &path) {
        load(path);
    }

    Properites::Properites()
            : m_root()
    {

    }

    Properites::~Properites() {

    }

    void Properites::load(const std::string &path) {
        m_root = YAML::LoadFile(path);
    }

    void Properites::save(const std::string &path) {
        std::ofstream fout(path);
        fout << m_root;
        fout.close();
    }

}