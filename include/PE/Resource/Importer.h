#ifndef PANNOIREENGINE_IMPORTER_H
#define PANNOIREENGINE_IMPORTER_H

#include "PE/Resource/Utils.h"
#include "PE/Resource/ResourceData.h"

namespace PE::Resource {

    class Importer {
    public:
        Importer() = default;
        virtual ~Importer() = default;



        void importShader(resourceID, ShaderData&);
        void importTexture(resourceID, TextureData&);

    };

}

#endif //PANNOIREENGINE_IMPORTER_H
