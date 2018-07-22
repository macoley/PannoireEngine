#ifndef PANNOIREENGINE_LOADERS_H
#define PANNOIREENGINE_LOADERS_H

#include <fstream>
#include <string>
#include <streambuf>
#include <cassert>
#include <iostream>
#include <sstream>

#include "PE/Render/Shader.h"

// Todo emplace in specify pool

namespace PE::Resource {

    using namespace PE;

    /**
     * Main fallback
     * @tparam R
     */
    template<typename R>
    R* load(const std::string &path ...)
    {
        assert(false);
    };

    /**
     * Shader
     * @tparam R
     * @param path
     * @return
     */
    template<>
    Render::Shader* load<Render::Shader>(const std::string &path)
    {
        // Load all file
        std::string data;
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            std::stringstream stream;
            file.open(path);
            stream << file.rdbuf();
            file.close();

            data= stream.str();
        }
        catch (const std::ifstream::failure & e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        // Alloc Shader
        auto shader = new Render::Shader()

        return shader;
    };

}

#endif //PANNOIREENGINE_LOADERS_H
