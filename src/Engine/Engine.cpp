#include "PE/Engine/Engine.h"


namespace PE::Engine {

    Engine::Engine() {
        Render::Render render;
        render.render("Render z engina");
    }

    template<typename C, typename... Types>
    void Engine::assignComponent(Types... args) {

    }

}