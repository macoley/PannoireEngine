#include "PE/Engine/Properties.h"

namespace PE::Engine {


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
        try {
            m_root = YAML::LoadFile(path);
        }
        catch (const YAML::BadFile& e)
        {
            Utils::logError("Can't load file " + path);
        }
    }

    void Properites::save(const std::string &path) {
        std::ofstream fout(path);
        fout << m_root;
        fout.close();
    }

}