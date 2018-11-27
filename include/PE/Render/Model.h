#ifndef PE_RENDER_MODEL_H
#define PE_RENDER_MODEL_H

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
#include "PE/Render/Material.h"
#include "PE/Utils/Utils.h"

namespace PE::Render {

    class Model : public Resource::IResource<Model> {
        using ResManagerPtr = std::shared_ptr<Resource::ResourceManager>;
        using TextureHandle = Resource::ResourceHandle<Texture>;

        using TextureHandleContainer = std::vector<TextureHandle>;

        using MeshHandle = Resource::ResourceHandle<Mesh>;
        using MaterialHandle = Resource::ResourceHandle<Material>;
        using Element = std::pair<MeshHandle, MaterialHandle>;
    public:
        explicit Model(ResManagerPtr res_manager) : m_res_manager(std::move(res_manager)) {};
        virtual ~Model();

        void load(const std::string & path);
        std::vector<Element> &getObject();

    private:
        std::vector<Element> m_objects;
        std::string m_directory;

        void processNode(aiNode *node, const aiScene *scene);
        MeshHandle processMesh(aiMesh *mesh);
        MaterialHandle processMaterial(aiMaterial *material);
        TextureHandleContainer processTextures(aiMaterial *material, aiTextureType type);

        ResManagerPtr m_res_manager;
    };

}


#endif //PE_RENDER_MODEL_H
