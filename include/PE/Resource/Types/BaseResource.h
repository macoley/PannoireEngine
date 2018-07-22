#ifndef PANNOIREENGINE_BASETYPE_H
#define PANNOIREENGINE_BASETYPE_H

#include <tuple>
#include <iostream>

namespace PE::Resource {

    /**
     * Base Type
     * @tparam Args
     */
    template <typename ... Args>
    class BaseResource {
    public:
        using constructorArgs = std::tuple<Args...>;

        BaseResource() = default;
        virtual ~BaseResource() = default;

        virtual constructorArgs get() = 0;
    };

    /**
     * Null Resource
     * @tparam R
     */
    using nullBase = BaseResource<bool>;

    template <typename R>
    class Resource : public nullBase
    {
    public:
        Resource() {
            std::cout << "basic \n";
        }

        nullBase::constructorArgs get() override {
            return nullBase::constructorArgs(false);
        }
    };


}


#endif //PANNOIREENGINE_BASETYPE_H
