#ifndef PE_SCRIPT_SCRIPTENGINE_H
#define PE_SCRIPT_SCRIPTENGINE_H

#include <iostream>
#include <fstream>
#include <regex>

#include "duktape.h"
#include "dukglue/dukglue.h"

#include "PE/Utils/Utils.h"

namespace PE::Scripting {

    class ScriptEngine {

    public:
        ScriptEngine();
        virtual ~ScriptEngine();

        void init();

        void unloadModule(const std::string& path);
        std::optional<std::string> loadModule(const std::string& path);
        void getInstanceMethod(const std::string& id, const std::string& functionName);
        void instantiate(const std::string& module, const std::string& className, const std::string& id, uint32_t entity);
        void deleteInstance(const std::string& id);

        duk_context *getCtx() {
            return ctx;
        }

        template <typename ... Ts>
        void registerConstructor(const std::string& name) {
            dukglue_register_constructor<Ts...>(ctx, name.c_str());
        }

        template <typename T, typename RetT, typename ArgT>
        void registerProperty(RetT(T::*getter)(), void(T::*setter)(ArgT), const std::string& name) {
            dukglue_register_property(ctx, getter, setter, name.c_str());
        }

        template <typename T>
        void registerGlobal(const T& obj, const std::string& name) {
            dukglue_register_global(ctx, obj, name.c_str());
        }

        template<class T, typename RetT, typename... Ts>
        void registerMethod(RetT(T::*method)(Ts...), const std::string& name)
        {
            dukglue_register_method(ctx, method, name.c_str());
        }

        template<typename RetT, typename... Ts>
        void registerFunction(RetT(*funcToCall)(Ts...), const std::string& name)
        {
            dukglue_register_function(ctx, funcToCall, name.c_str());
        }

        template <typename T>
        void pushObject(const T& obj) {
            dukglue_push(ctx, obj);
        }


    private:
        duk_context *ctx;

        void evalFile(const std::string& path);

        static duk_ret_t loadFileContent(duk_context *ctx);
        static duk_ret_t ifFileExists(duk_context *ctx);
        static duk_ret_t log(duk_context *ctx);
    };

}

#endif //PE_SCRIPT_SCRIPTENGINE_H
