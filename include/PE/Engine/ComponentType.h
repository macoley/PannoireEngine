#ifndef PANNOIREENGINE_COMPONENT_TYPE_H
#define PANNOIREENGINE_COMPONENT_TYPE_H


namespace PE::Engine::ComponentType {

    struct Transform {
        float x, y, z;
    };

    struct Render {
        Transform transform;
        float temp;
    };

}


#endif //PANNOIREENGINE_COMPONENT_TYPE_H
