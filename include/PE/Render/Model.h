#ifndef PANNOIREENGINE_MODEL_H
#define PANNOIREENGINE_MODEL_H

#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PE/Render/Mesh.h"

namespace PE::Render {

    class Model {
    public:
        explicit Model(std::string path);

        inline void draw(Shader & t_shader) {
            for (auto &meshe : meshes)
                meshe.draw(t_shader);
        };

    private:
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;

        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    };

}


#endif //PANNOIREENGINE_MODEL_H
