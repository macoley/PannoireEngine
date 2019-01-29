
#include <PE/Scripting/ScriptEngine.h>

#define js_lib(filename) (JS_LIB_PATH "/" filename)

namespace PE::Scripting {

    ScriptEngine::ScriptEngine()
        : ctx(duk_create_heap_default())
    {

    }

    ScriptEngine::~ScriptEngine() {
        duk_destroy_heap(ctx);
    }

    duk_ret_t ScriptEngine::loadFileContent(duk_context *ctx) {
        std::ifstream ifs(duk_safe_to_string(ctx, 0));
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        duk_push_string(ctx, content.c_str());

        return 1;
    }

    duk_ret_t ScriptEngine::ifFileExists(duk_context *ctx) {
        std::ifstream f(duk_safe_to_string(ctx, 0));
        duk_push_boolean(ctx, static_cast<duk_bool_t>(f.good()));

        return 1;
    }

    duk_ret_t ScriptEngine::log(duk_context *ctx) {
        Utils::log("[Log from the script] " + std::string(duk_safe_to_string(ctx, 0)));

        return 0;
    }

    void ScriptEngine::evalFile(const std::string &path) {
        std::ifstream ifs(path);
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        if (duk_peval_string(ctx, content.c_str()) != 0) {
            Utils::logError("[Eval script failed] " + path + std::string(duk_safe_to_string(ctx, -1)));
        }

        duk_pop(ctx);
    }

    void ScriptEngine::init() {
        if(!ctx) {
            Utils::logError("ScriptingEngine error");
        }

        // Native functions
        duk_push_c_function(ctx, loadFileContent, 1);
        duk_put_global_string(ctx, "loadFileContent");

        duk_push_c_function(ctx, ifFileExists, 1);
        duk_put_global_string(ctx, "ifFileExists");

        duk_push_c_function(ctx, log, 1);
        duk_put_global_string(ctx, "log");

        // Typescript
        evalFile(js_lib("TypeScript.js"));

        // Lib
        evalFile(js_lib("PannoireEngineLib.js"));

    }

    std::optional<std::string> ScriptEngine::loadModule(const std::string &path) {
        duk_get_global_string(ctx, "loadModule");
        duk_push_string(ctx, path.c_str());
        duk_call(ctx, 1);

        std::string result = duk_safe_to_string(ctx, -1);
        duk_pop(ctx);

        if (result != "undefined")
            return result;

        return {};
    }

    void ScriptEngine::getInstanceMethod(const std::string &id, const std::string &functionName) {
        duk_get_global_string(ctx, "getModuleMethod");
        duk_push_string(ctx, id.c_str());
        duk_push_string(ctx, functionName.c_str());
        duk_call(ctx, 2);
    }

    void ScriptEngine::instantiate(
            const std::string &module,
            const std::string &className,
            const std::string &id,
            uint32_t entity)
    {
        duk_get_global_string(ctx, "instantiate");
        duk_push_string(ctx, module.c_str());
        duk_push_string(ctx, className.c_str());
        duk_push_string(ctx, id.c_str());
        duk_push_int(ctx, entity);

        duk_call(ctx, 4);
        duk_pop(ctx);
    }

    void ScriptEngine::unloadModule(const std::string &path) {
        duk_get_global_string(ctx, "unloadModule");
        duk_push_string(ctx, path.c_str());
        duk_call(ctx, 1);
        duk_pop(ctx);
    }

    void ScriptEngine::deleteInstance(const std::string &id) {
        duk_get_global_string(ctx, "deleteInstance");
        duk_push_string(ctx, id.c_str());
        duk_call(ctx, 1);
        duk_pop(ctx);
    }

}

