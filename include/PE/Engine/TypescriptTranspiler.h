#ifndef PE_ENGINE_TYPESCRIPTTRANSPILER_H
#define PE_ENGINE_TYPESCRIPTTRANSPILER_H

#include <memory>
#include <fstream>
#include <algorithm>

#include "duktape.h"

#include "PE/Utils/Utils.h"
#include "IScriptTranspiler.h"

namespace PE::Engine {

    class TypescriptTranspiler : public IScriptTranspiler {
    public:
        TypescriptTranspiler();
        virtual ~TypescriptTranspiler();

        std::optional<std::string> transpile(const std::string&) override;

        static std::shared_ptr<IScriptTranspiler> make();

    private:
        static std::string clear(std::string);

        duk_context * m_ctx;
    };

}


#endif //PE_ENGINE_TYPESCRIPTTRANSPILER_H
