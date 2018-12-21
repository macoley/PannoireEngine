
#include <PE/Engine/TypescriptTranspiler.h>

#include "PE/Engine/TypescriptTranspiler.h"

namespace PE::Engine {

    TypescriptTranspiler::TypescriptTranspiler()
        : m_ctx(duk_create_heap_default())
    {
        if(!m_ctx) {
            Utils::logError("Typescript transpiler heap has not initialised.");
        }

        std::ifstream ifs("bundle.js");
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        if (duk_peval_string(m_ctx, content.c_str()) != 0) {
            Utils::logError("Typescript library has not loaded.");
            Utils::logError(duk_safe_to_string(m_ctx, -1));
        } else {
            Utils::log("Typescript library loaded.");
        }
        duk_pop(m_ctx); // Clear 'undefined' result
    }

    /**
     * Transpile typescript code
     * @return
     */
    std::optional<std::string> TypescriptTranspiler::transpile(const std::string &content) {
        std::string cmd = "bundle.ts.transpile('" + clear(content) + "');";
        std::optional<std::string> output;

        if (duk_peval_string(m_ctx, cmd.c_str()) != 0) {
            Utils::logError("Typescript transpile error.");
            Utils::logError(duk_safe_to_string(m_ctx, -1));
        } else {
            output = duk_safe_to_string(m_ctx, -1);
        }
        duk_pop(m_ctx);

        return output;
    }

    /**
     * Factory method
     * @return
     */
    std::shared_ptr<IScriptTranspiler> TypescriptTranspiler::make() {
        return std::make_shared<TypescriptTranspiler>();
    }

    /**
     * Clear source
     * @return
     */
    std::string TypescriptTranspiler::clear(std::string source) {
        std::replace(source.begin(), source.end(), '\n', ' ');

        size_t pos;
        while( ( pos = source.find( '\t' ) )!=std::string::npos )
            source = source.replace( pos, 2, " " );

        while( ( pos = source.find( "  " ) )!=std::string::npos )
            source = source.replace( pos, 2, " " );

        return source;
    }

    /**
     * Destructor
     */
    TypescriptTranspiler::~TypescriptTranspiler() {
        duk_destroy_heap(m_ctx);
    }

}