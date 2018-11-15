
#include <PE/Render/Model.h>

#include "PE/Render/Model.h"

namespace PE::Render {

    void Model::processNode(aiNode *node, const aiScene *scene) {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            m_meshes.push_back({processMesh(mesh), processMaterial(material)});
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
    void Model::draw(Shader &shader) {
        std::for_each(m_meshes.begin(), m_meshes.end(), [&](MeshRender &m) {
            shader.use(); // todo shader from material

            // Textures (todo move to material class)
            uint8_t i = 0, j;

            j = 0;
            for (TextureHandle &t : m.material.diffuseMaps) {
                t->bindTexture(shader, "texture_diffuse_" + std::to_string(j++), i++);
            }

            j = 0;
            for (TextureHandle &t : m.material.heightMaps) {
                t->bindTexture(shader, "texture_height_" + std::to_string(j++), i++);
            }

            j = 0;
            for (TextureHandle &t : m.material.normalMaps) {
                t->bindTexture(shader, "texture_normal_" + std::to_string(j++), i++);
            }

            j = 0;
            for (TextureHandle &t : m.material.opacityMaps) {
                t->bindTexture(shader, "texture_opacity_" + std::to_string(j++), i++);
            }

            j = 0;
            for (TextureHandle &t : m.material.specularMaps) {
                t->bindTexture(shader, "texture_specular_" + std::to_string(j++), i++);
            }

            // Mesh render
            m.mesh->draw(shader);
        });
    }

    Model::~Model() {
        m_meshes.clear();
    }

}