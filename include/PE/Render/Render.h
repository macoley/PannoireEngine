#ifndef PANNOIREENGINE_RENDER_H
#define PANNOIREENGINE_RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Context.h"

namespace PE::Render {

    void init();
    std::shared_ptr<Context> createContext(const std::string& title, uint32_t width, uint32_t height);

}

#endif //PANNOIREENGINE_RENDER_H
