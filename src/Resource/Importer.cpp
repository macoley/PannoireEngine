
#include "PE/Resource/Importer.h"

namespace PE::Resource {

    void Importer::importShader(resourceID id, ShaderData &shaderData) {
        loadResource(getResourcePathFromID(id), shaderData);
    }

    void Importer::importTexture(resourceID id, TextureData &) {
        //loadResource(getResourcePathFromID(id), shaderData);
    }
}
