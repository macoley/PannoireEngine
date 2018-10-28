#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <tuple>

#include "PE/Engine/Engine.h"

int main()
{
    auto engine = PE::Engine::makeCore();
    engine->init();

    return 0;
}
