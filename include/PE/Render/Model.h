#ifndef PANNOIREENGINE_MODEL_H
#define PANNOIREENGINE_MODEL_H

#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "PE/Render/Defines.h"
#include "PE/Render/Texture.h"
#include "PE/Resource/Resource.h"
#include "PE/Render/Mesh.h"
#include "PE/Utils/Utils.h"

namespace PE::Render {

    class Model : public Resource::IResource {
        using ManagerPtr = std::shared_ptr<Resource::ResourceManager>;
        using TextureHandle = Resource::ResourceHandle<Texture>;
        using MeshHandle = Resource::ResourceHandle<Mesh>;
        using TextureHandleContainer = std::vector<TextureHandle>;

    public:
        explicit Model(ManagerPtr manager) : m_manager(std::move(manager)) {};
        virtual ~Model();

        void draw(Shader & t_shader);
        void load(const std::string & path) override;

    private:
        struct Material {
            // 1. diffuse maps
            TextureHandleContainer diffuseMaps;
            // 2. specular maps
            TextureHandleContainer specularMaps;
            // 3. normal maps
            TextureHandleContainer normalMaps;
            // 4. height maps
            TextureHandleContainer heightMaps;
            // 5. opacity maps
            TextureHandleContainer opacityMaps;
        };

        struct MeshRender {
            MeshHandle mesh;
            Material material;
        };

        std::vector<MeshRender> m_meshes;
        std::string directory;

        void processNode(aiNode *node, const aiScene *scene);
        MeshHandle processMesh(aiMesh *mesh);
        Material processMaterial(aiMaterial *material);
        TextureHandleContainer processTextures(aiMaterial *material, aiTextureType type);

        ManagerPtr m_manager;
    };

}


#endif //PANNOIREENGINE_MODEL_H
