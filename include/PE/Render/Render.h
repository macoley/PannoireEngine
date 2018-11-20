#ifndef PE_RENDER_RENDER_H
#define PE_RENDER_RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Context.h"
#include "Texture.h"

namespace PE::Render {

    std::shared_ptr<Context> createContext(const std::string& title, uint32_t width, uint32_t height);

}

#endif //PE_RENDER_RENDER_H
