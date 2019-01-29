#include <iostream>
#include <fstream>
#include <regex>

#include "duktape.h"
#include "dukglue/dukglue.h"

#define js_lib(filename) (JS_LIB_PATH "/" filename)

#define getter_setter(type, name)                                          \
    type get_##name() { return m_component.name; }                         \
    void set_##name(type t_##name) { m_component.name = t_##name; }

#define comp_wrapper_begin(ComponentType)                                                \
template <>                                                                              \
struct ComponentBind<ComponentType> {                                                    \
    explicit ComponentBind(ComponentType& t_component) : m_component(t_component) {};    \
    ComponentBind(ComponentType &&) = delete;                                            \
    ComponentBind() = delete;                                                            \
    ComponentType& m_component;

#define comp_wrapper_end()                                                               \
};


#define reg_component(ComponentType)                                                     \
dukglue_register_constructor<ComponentBind<ComponentType>, ComponentType&>(ctx, #ComponentType);

#define reg_component_val(ComponentType, Value)                                          \
dukglue_register_property(ctx, &ComponentBind<ComponentType>::get_##Value, &ComponentBind<Component>::set_##Value, #Value);


void debug(duk_context * ctx) {
    duk_eval_string(ctx, "JSON.stringify(this.LoadedModules);");
    printf("LoadedModules: %s\n", duk_safe_to_string(ctx, -1));
    duk_pop(ctx);

    duk_eval_string(ctx, "JSON.stringify(this.Instances);");
    printf("Instances: %s\n", duk_safe_to_string(ctx, -1));
    duk_pop(ctx);

    duk_push_context_dump(ctx);
    printf("%s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);
}

std::optional<std::string> loadModule(duk_context *ctx, const std::string& path)
{
    duk_get_global_string(ctx, "loadModule");
    duk_push_string(ctx, path.c_str());
    duk_call(ctx, 1);

    std::string result = duk_safe_to_string(ctx, -1);
    duk_pop(ctx);

    if (result != "undefined")
        return result;

    return {};
}

void unloadModule(duk_context *ctx, const std::string& path)
{
    duk_get_global_string(ctx, "unloadModule");
    duk_push_string(ctx, path.c_str());
    duk_call(ctx, 1);
    duk_pop(ctx);
}

void instantiate(duk_context *ctx, const std::string& module, const std::string& className, const std::string& id, uint32_t entity)
{
    duk_get_global_string(ctx, "instantiate");
    duk_push_string(ctx, module.c_str());
    duk_push_string(ctx, className.c_str());
    duk_push_string(ctx, id.c_str());
    duk_push_int(ctx, entity);

    duk_call(ctx, 4);
    duk_pop(ctx);
}

void evalFile(duk_context *ctx, const std::string& path)
{
    std::ifstream ifs(path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    if (duk_peval_string(ctx, content.c_str()) != 0) {
        printf("File %s eval failed: %s\n", path.c_str(), duk_safe_to_string(ctx, -1));
    } else {
        printf("File %s result is: %s\n", path.c_str(), duk_safe_to_string(ctx, -1));
    }

    duk_pop(ctx);
}

void getInstanceMethod(duk_context *ctx, const std::string& id, const std::string& functionName) {
    duk_get_global_string(ctx, "getModuleMethod");
    duk_push_string(ctx, id.c_str());
    duk_push_string(ctx, functionName.c_str());
    duk_call(ctx, 2);
}

/*
 * API
 */

static duk_ret_t loadFileContent(duk_context *ctx) {
    std::ifstream ifs(duk_safe_to_string(ctx, 0));
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    duk_push_string(ctx, content.c_str());

    return 1;  /* no return value (= undefined) */
}

static duk_ret_t ifFileExists(duk_context *ctx) {
    std::ifstream f(duk_safe_to_string(ctx, 0));
    duk_push_boolean(ctx, static_cast<duk_bool_t>(f.good()));

    return 1;  /* no return value (= undefined) */
}

static duk_ret_t log(duk_context *ctx) {
    printf("Script debug:\n%s\n", duk_safe_to_string(ctx, 0));

    return 0;  /* no return value (= undefined) */
}

struct Component {
    int value = 3;
};

template <typename T>
struct ComponentBind {
    explicit ComponentBind(T& t_component) : m_component(t_component) {};
    ComponentBind(T &&) = delete;
    ComponentBind() = delete;
    T& m_component;
};

comp_wrapper_begin(Component)
    getter_setter(int, value);
comp_wrapper_end()


static duk_ret_t getComponent(duk_context *ctx) {
    int entity = duk_to_number(ctx, 0);
    int component = duk_to_number(ctx, 1);

    printf("Entity: %i\n", entity);
    printf("Component: %i\n", component);

    auto comp = new Component();
    auto wrapp = new ComponentBind<Component>(*comp);
    dukglue_push(ctx, wrapp);

    return 1;
}


int main()
{
    auto ctx = duk_create_heap_default();

    if(!ctx) {
        return -1;
    }

    // Native functions
    duk_push_c_function(ctx, loadFileContent, 1);
    duk_put_global_string(ctx, "loadFileContent");

    duk_push_c_function(ctx, ifFileExists, 1);
    duk_put_global_string(ctx, "ifFileExists");

    duk_push_c_function(ctx, log, 1);
    duk_put_global_string(ctx, "log");

    duk_push_c_function(ctx, getComponent, 2);
    duk_put_global_string(ctx, "getComponent");

    // Typescript
    evalFile(ctx, js_lib("TypeScript.js"));

    // Lib
    evalFile(ctx, js_lib("PannoireEngineLib.js"));

    // Load module
    loadModule(ctx, "./res/Rotate.ts");
    debug(ctx);

    // Instantiate
    instantiate(ctx, "./res/Rotate.ts", "Rotate", "test", 1);
    debug(ctx);

    // register
    //dukglue_register_constructor<ComponentBind<Component>, Component&>(ctx, "Test");
    dukglue_register_property(ctx, &ComponentBind<Component>::get_value, &ComponentBind<Component>::set_value, "value");

    reg_component(Component);
    reg_component_val(Component, value);

    // Get method
    getInstanceMethod(ctx, "test", "update");


    //auto comp = Component();
    //auto test = new ComponentBind<Component>(comp);
    //dukglue_push(ctx, test);
    duk_call(ctx, 0);


    //delete test;
    //printf("Component value: %i\n", test->m_component.value);
    //printf("Component value: %i\n", comp.value);

    debug(ctx);




    // duk_get_prototype(ctx, -3);


    //duk_push_int(ctx, 123);
    //duk_push_int(ctx, 2);
    //duk_push_int(ctx, 3);

    //debug(ctx);


    //getInstanceMethod(ctx, "test", "update");

    // Call update

    /*
    if (duk_peval_string(ctx, "var elo = new this.LoadedModules['res/script.ts'].exports['Test']();") != 0) {
        printf("eval failed: %s\n", duk_get_string(ctx, -1));
    } else {
        printf("result is: %s\n", duk_get_string(ctx, -1));
    }
    duk_pop(ctx);
    debug(ctx);


    duk_push_c_function(ctx, require, 1);
    duk_put_global_string(ctx, "require");

    // Module
    loadModule(ctx, resolveModule("script"));
    eval(ctx, "this.script.Test");

    //std::ifstream ifs2("res/script.ts");
    //std::ifstream ifs2("res/script.ts");
    //std::string content2((std::istreambuf_iterator<char>(ifs2)), (std::istreambuf_iterator<char>()));

    /*
    //duk_push_string(ctx, "1.2.3");
    duk_push_bare_object(ctx);
    (void) duk_put_global_string(ctx, "self");

    duk_push_bare_object(ctx);
    (void) duk_put_global_string(ctx, "console");
    if (duk_peval_string(ctx, "console.assert = function() {};")){
        printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
    } else {
        printf("result is: %s\n", duk_get_string(ctx, -1));
    }
    duk_pop(ctx);

    duk_get_global_string(ctx, "console");

    duk_push_context_dump(ctx);
    printf("%s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);
     */

    /*
    eval(ctx, content.c_str(), false);
    debug(ctx);

    eval(ctx, "var exports = {};");

    duk_push_c_function(ctx, require, 1);
    duk_put_global_string(ctx, "require");

    std::string req = "bundle.ts.transpile('" + clear(content2) + "', {'compilerOptions':{'target': 'es5'}});";
    std::string transpiled = eval(ctx, req.c_str());
    */

    //eval(ctx, "var test = require('PannoireEngine');");
    //eval(ctx, "test.Entity;");
    //debug(ctx);



    //eval(ctx, "PannoireEngine;");
    //debug(ctx);

    //duk_get_global_string(ctx, "ExampleSystem");
    //debug(ctx);

    //eval(ctx, "var test = require('PannoireEngine');");

    //eval(ctx, "test.Entity;");
    //debug(ctx);


    //std::string test = "(function(exports) {" + transpiled + "return exports;})({})";
    //eval(ctx, test.c_str());
    //debug(ctx);

    //duk_pop(m_ctx);

    //eval(ctx, "var exports = {};");
    //dukglue_register_function(ctx, require, "require");

    //eval(ctx, transpiled.c_str());
    //debug(ctx);

    //duk_get_global_string(ctx, "ExampleSystem");
    //debug(ctx);

    ///
    /*
    if (duk_peval_string(ctx, "var e = new Test();") != 0) {
        printf("eval failed: %s\n", duk_get_string(ctx, -1));
    } else {
        printf("result is: %s\n", duk_get_string(ctx, -1));
    }
    duk_pop(ctx);

    //duk_get_global_string(ctx, "Test"); // Class name same as filename

    duk_push_context_dump(ctx);
    printf("%s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);

    // Create new script object
    auto rc = duk_pnew(ctx, 0);
    if (rc != 0) {
        printf("Script cant to create Class\n");
        printf("eval failed: %s\n", duk_get_string(ctx, -1));
    }

    auto obj_idx = duk_get_top_index(ctx);
     */

    //dukglue_register_constructor<API>(ctx, "API");
    //dukglue_register_function(ctx, API::getInstance, "getAPI");
    //dukglue_register_method(ctx, &API::rotateAll, "rotateAll");

    duk_destroy_heap(ctx);
    return 0;
}
