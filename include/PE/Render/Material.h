#ifndef PE_RENDER_MATERIAL_H
#define PE_RENDER_MATERIAL_H

#include "PE/Resource/Resource.h"
#include "Texture.h"
#include "Shader.h"

namespace PE::Render {

    class Material : public Resource::IResource<Material> {
    public:
        using TextureHandle = Resource::ResourceHandle<Texture>;
        using TextureHandleContainer = std::vector<TextureHandle>;

        Material(TextureHandleContainer diffuseMaps,
                 TextureHandleContainer specularMaps,
                 TextureHandleContainer normalMaps,
                 TextureHandleContainer heightMaps,
                 TextureHandleContainer opacityMaps) :
                m_diffuseMaps(std::move(diffuseMaps)),
                m_specularMaps(std::move(specularMaps)),
                m_normalMaps(std::move(normalMaps)),
                m_heightMaps(std::move(heightMaps)),
                m_opacityMaps(std::move(opacityMaps)) {}

        explicit Material() = default;

        virtual ~Material() = default;

        inline void bind(const Shader &shader) const;

        inline bool isTransparent() const;

        void load(const std::string &path);

    private:
        // 1. diffuse maps
        TextureHandleContainer m_diffuseMaps;
        // 2. specular maps
        TextureHandleContainer m_specularMaps;
        // 3. normal maps
        TextureHandleContainer m_normalMaps;
        // 4. height maps
        TextureHandleContainer m_heightMaps;
        // 5. opacity maps
        TextureHandleContainer m_opacityMaps;
    };

    /**
     * Bind Material
     * @param shader
     */
    void Material::bind(const Shader &shader) const {
        uint8_t i = 0, j;

        j = 0;
        for (const TextureHandle &t : m_diffuseMaps) {
            t->bindTexture(shader, "texture_diffuse_" + std::to_string(j++), i++);
        }

        // Placeholder
        if (m_diffuseMaps.empty()) {
            Texture::black->bindTexture(shader, "texture_diffuse_0", i++);
        }

        j = 0;
        for (const TextureHandle &t : m_heightMaps) {
            t->bindTexture(shader, "texture_height_" + std::to_string(j++), i++);
        }

        j = 0;
        for (const TextureHandle &t : m_normalMaps) {
            t->bindTexture(shader, "texture_normal_" + std::to_string(j++), i++);
        }

        j = 0;
        for (const TextureHandle &t : m_opacityMaps) {
            t->bindTexture(shader, "texture_opacity_" + std::to_string(j++), i++);
        }

        // Placeholder
        if (m_opacityMaps.empty()) {
            Texture::white->bindTexture(shader, "texture_opacity_0", i++);
        }

        j = 0;
        for (const TextureHandle &t : m_specularMaps) {
            t->bindTexture(shader, "texture_specular_" + std::to_string(j++), i++);
        }
    }

    bool Material::isTransparent() const {
        return !m_opacityMaps.empty() || std::any_of(m_diffuseMaps.begin(), m_diffuseMaps.end(),
                                                   [](const TextureHandle &t) { return t->isTrasparent(); });
    }

}

#endif //PE_RENDER_MATERIAL_H
