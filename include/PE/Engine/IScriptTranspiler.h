#ifndef PE_ENGINE_ISCRIPTTRANSPILER_H
#define PE_ENGINE_ISCRIPTTRANSPILER_H

#include <string>
#include <optional>

namespace PE::Engine {

    class IScriptTranspiler {
    public:
        virtual std::optional<std::string> transpile(const std::string&) = 0;
    };

}


#endif //PE_ENGINE_ISCRIPTTRANSPILER_H
