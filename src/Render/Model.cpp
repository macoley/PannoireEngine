
#include <PE/Render/Model.h>

#include "PE/Render/Model.h"

namespace PE::Render {

    void Model::processNode(aiNode *node, const aiScene *scene) {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            //meshes.push_back({processMesh(mesh), processMaterial(material)});
            m_meshes.push_back({{}, processMaterial(material)});
            //test2.push_back(processMaterial(material));
        }

        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
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

        directory = path.substr(0, path.find_last_of('/'));
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
        std::vector<unsigned int> indices;

        /**
         * Process Vertices
         */

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
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
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return m_manager->create<Mesh>(std::move(vertices), std::move(indices));
    }

    /**
     * Process Material
     * @param material
     * @return
     */
    Model::Material Model::processMaterial(aiMaterial *material) {
        Material mat;
        mat.diffuseMaps = processTextures(material, aiTextureType_DIFFUSE);
        mat.specularMaps = processTextures(material, aiTextureType_SPECULAR);
        mat.normalMaps = processTextures(material, aiTextureType_HEIGHT);
        mat.heightMaps = processTextures(material, aiTextureType_AMBIENT);
        mat.opacityMaps = processTextures(material, aiTextureType_OPACITY);
        return mat;
    }

    /**
     * Process Textures
     * @param material
     * @param type
     * @return
     */
    Model::TextureHandleContainer Model::processTextures(aiMaterial *material, aiTextureType type) {
        TextureHandleContainer handles;

        for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
            aiString str;
            material->GetTexture(type, i, &str);

            handles.push_back(
                    m_manager->load<Texture>(directory + "\\" + str.C_Str())
            );
        }

        return handles;
    }

    /**
     * Draw Model
     * @param t_shader
     */
    void Model::draw(Shader &t_shader) {

    }

    Model::~Model() {
        m_meshes.clear();
    }

}