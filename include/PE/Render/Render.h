#ifndef PANNOIREENGINE_RENDER_H
#define PANNOIREENGINE_RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Context.h"

namespace PE::Render {

    void init();
    Context* createContext();

}

#endif //PANNOIREENGINE_RENDER_H
