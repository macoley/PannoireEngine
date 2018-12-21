#include <iostream>
#include <fstream>
#include <regex>

#include "duktape.h"
#include "dukglue/dukglue.h"

#define js_lib(filename) (JS_LIB_PATH "/" filename)

void debug(duk_context * ctx) {
    duk_eval_string(ctx, "JSON.stringify(this.LoadedModules);");
    printf("LoadedModules: %s\n", duk_safe_to_string(ctx, -1));
    duk_pop(ctx);

    duk_push_context_dump(ctx);
    printf("%s\n", duk_to_string(ctx, -1));
    duk_pop(ctx);
}

void loadModule(duk_context *ctx, const std::string& path)
{
    duk_get_global_string(ctx, "loadModule");
    duk_push_string(ctx, path.c_str());
    duk_call(ctx, 1);
    duk_pop(ctx);
}

void unloadModule(duk_context *ctx, const std::string& path)
{
    duk_get_global_string(ctx, "unloadModule");
    duk_push_string(ctx, path.c_str());
    duk_call(ctx, 1);
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

    // Typescript
    evalFile(ctx, js_lib("TypeScript.js"));

    // Lib
    evalFile(ctx, js_lib("PannoireEngineLib.js"));

    // Load module
    loadModule(ctx, "./res/script.ts");
    debug(ctx);

    /*
    duk_push_c_function(ctx, require, 1);
    duk_put_global_string(ctx, "require");

    // Module
    loadModule(ctx, resolveModule("script"));
    eval(ctx, "this.script.Test");

     */
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
