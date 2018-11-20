
#include "PE/Render/Render.h"

namespace PE::Render {

    std::shared_ptr<Context> createContext(const std::string& title, uint32_t width, uint32_t height) {
        return std::make_shared<Context>(title, width, height);
    }

}