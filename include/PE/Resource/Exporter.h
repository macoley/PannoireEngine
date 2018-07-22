#ifndef PANNOIREENGINE_EXPORTER_H
#define PANNOIREENGINE_EXPORTER_H

#include <fstream>
#include <string>
#include <streambuf>
#include <cassert>
#include <iostream>
#include <sstream>

#include <stb_image.h>

#include "PE/Resource/Utils.h"
#include "PE/Resource/ResourceData.h"

namespace PE::Resource {

    class Exporter {
    public:
        Exporter() = default;
        virtual ~Exporter() = default;

        void exportShader(const std::string& vertexShaderPath, const std::string &fragmentShaderPath);
        void exportTexture(const std::string& texturePath);

    };

}

#endif //PANNOIREENGINE_EXPORTER_H
