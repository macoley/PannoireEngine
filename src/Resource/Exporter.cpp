
#include "PE/Resource/Exporter.h"

namespace PE::Resource {

    /**
     * Load text helper
     * @param path
     * @return
     */
    std::string loadText(const std::string& path) {
        std::string data;
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            std::stringstream stream;
            file.open(path);
            stream << file.rdbuf();
            file.close();

            data= stream.str();
        }
        catch (const std::ifstream::failure & e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ | " << path  << std::endl;
        }

        return data;
    }

    /**
     * Shader exporter
     * @param vertexShaderPath
     * @param fragmentShaderPath
     */
    void Exporter::exportShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
        ShaderData shaderData{};
        shaderData.vertexShader = loadText(vertexShaderPath);
        shaderData.fragmentShader = loadText(fragmentShaderPath);

        std::string resFilename = getResourcePath(vertexShaderPath + '.' + fragmentShaderPath);

        saveResource(resFilename, shaderData);
    }

    /**
     * Texture exporter
     * @param texturePath
     */
    void Exporter::exportTexture(const std::string &texturePath)
    {
        // Placeholder
        int width, height, components;

        // Loading
        TextureData textureData{};
        textureData.data = stbi_load(texturePath.c_str(), &width, &height, &components, 0);
        textureData.width = static_cast<uint32_t>(width);
        textureData.height = static_cast<uint32_t>(height);
        textureData.components = static_cast<uint8_t>(components);

        if (!textureData.data) {
            std::cout << "Texture failed to load at path: " << texturePath << std::endl;
        }

        // Save resource
        saveResource(getResourcePath(texturePath), textureData);

        // Free data after save
        stbi_image_free(textureData.data);
    }
}