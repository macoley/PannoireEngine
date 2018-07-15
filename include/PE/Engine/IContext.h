#ifndef PANNOIREENGINE_ICONTEXT_H
#define PANNOIREENGINE_ICONTEXT_H

namespace PE::Engine {

    class IContext {
    public:
        virtual bool isRunning() = 0;
        virtual void render() = 0;
        virtual void processInput() = 0;
        virtual double getTime() = 0;
        virtual void update() = 0;
    };

}

#endif //PANNOIREENGINE_ICONTEXT_H
