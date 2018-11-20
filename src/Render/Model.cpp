
#include <PE/Render/Model.h>

#include "PE/Render/Model.h"

namespace PE::Render {

    void Model::processNode(aiNode *node, const aiScene *scene) {
        // process all the node's meshes (if any)
        for (std::size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            m_objects.emplace_back(processMesh(mesh), processMaterial(material));
        }

        // then do the same for each of its children
        for (std::size_t i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    /**
     * Model load
     * @param path
     */
    void Model::load(const std::string &path) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            Utils::logError(std::string("ERROR::ASSIMP::") + import.GetErrorString());
            return;
        }

        m_directory = path.substr(0, path.find_last_of('/')); // wrrrr
        processNode(scene->mRootNode, scene);

        import.FreeScene();
    }

    /**
     * Process Mesh
     * @param mesh
     * @return
     */
    Resource::ResourceHandle<Mesh> Model::processMesh(aiMesh *mesh) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        /**
         * Process Vertices
         */

        for (std::size_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            // Positions
            vertex.Position = glm::vec3(
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
            );

            // Normals
            vertex.Normal = glm::vec3(
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
            );

            // UV
            glm::vec2 vec(0.0f, 0.0f);
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
            }
            vertex.TexCoords = vec;

            vertices.push_back(vertex);
        }

        /**
         * process indices
         */
        for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (std::size_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return m_res_manager->create<Mesh>(vertices, indices);
    }

    /**
     * Process Material
     * @param material
     * @return
     */
    Model::MaterialHandle Model::processMaterial(aiMaterial *material) {
        return m_res_manager->create<Material>(
                processTextures(material, aiTextureType_DIFFUSE),
                processTextures(material, aiTextureType_SPECULAR),
                processTextures(material, aiTextureType_HEIGHT),
                processTextures(material, aiTextureType_AMBIENT),
                processTextures(material, aiTextureType_OPACITY)
        );
    }

    /**
     * Process Textures
     * @param material
     * @param type
     * @return
     */
    Model::TextureHandleContainer Model::processTextures(aiMaterial *material, aiTextureType type) {
        TextureHandleContainer handles;

        for (uint32_t i = 0; i < material->GetTextureCount(type); i++) {
            aiString str;
            material->GetTexture(type, i, &str);

            handles.push_back(
                    m_res_manager->load<Texture>(m_directory + "\\" + str.C_Str())
            );
        }

        return handles;
    }

    Model::~Model() {
        m_objects.clear();
    }

    std::vector<Model::Element> &Model::getObject() {
        return m_objects;
    }



}