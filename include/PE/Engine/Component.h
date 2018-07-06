#ifndef PANNOIREENGINE_COMPONENTS_H
#define PANNOIREENGINE_COMPONENTS_H


namespace PE::Engine::ComponentType {

    struct Transform {
        Transform(float x = .0f, float y = .0f, float z = .0f)
                : x(x), y(y), z(z) {};

        float x, y, z;
    };

    struct Render {
        Render(Transform transform, float temp)
                : transform(transform), temp(temp) {};

        Transform transform;
        float temp;
    };

}


#endif //PANNOIREENGINE_COMPONENTS_H
