#include "PE/Resource/Properties.h"

namespace PE::Resource {

    void Properties::load(const std::string &path) {
        try {
            m_root = YAML::LoadFile(path);
        }
        catch (const YAML::BadFile &e) {
            Utils::logError("Can't load file " + path);
        }
    }

    void Properties::save(const std::string &path) {
        std::ofstream fout(path);
        fout << m_root;
        fout.close();
    }

}