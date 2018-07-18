#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <tuple>

#include <PE/Render/Context.h>
#include <PE/Engine/Engine.h>

using namespace PE;

int main()
{
    auto context = std::make_shared<PE::Render::Context>();

    PE::Engine::Engine engine(context);
    engine.init();

    return 0;
}
