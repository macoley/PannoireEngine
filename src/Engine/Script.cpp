
#include <PE/Engine/Script.h>

#include "PE/Engine/Script.h"

namespace PE::Engine {

    void Script::load(const std::string &path) {
        // Class name
        std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
        std::string::size_type const p(base_filename.find_last_of('.'));
        std::string file_without_extension = base_filename.substr(0, p);

        // Get all content from file
        std::string source;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            std::stringstream stream;
            file.open(path);
            stream << file.rdbuf();
            file.close();

            source = stream.str();
        }
        catch (const std::ifstream::failure &e) {
            Utils::logError("ERROR::SCRIPT::FILE_NOT_SUCCESFULLY_READ");
        }


        // Eval
        duk_compile_string(m_ctx, DUK_COMPILE_EVAL, source.c_str());
        duk_call(m_ctx, 0);
        duk_pop(m_ctx);


        /*
        duk_push_context_dump(m_ctx);
        printf("%s\n", duk_to_string(m_ctx, -1));
        duk_pop(m_ctx);
         */

        // Get Class
        duk_get_global_string(m_ctx, file_without_extension.c_str()); // Class name same as filename

        // Create new script object
        auto rc = duk_pnew(m_ctx, 0);
        if (rc != 0) {
            Utils::logError("Script cant to create Class");
        }

        m_obj_idx = duk_get_top_index(m_ctx);

        // Is PEScript
        duk_get_global_string(m_ctx, "PEScript");

        if (!duk_instanceof(m_ctx, m_obj_idx, -1)) {
            Utils::logError(file_without_extension + " script must be instance of PEScript");
        }

        duk_pop(m_ctx);
    }

    Script::Script()
            : m_ctx(duk_create_heap_default()) {

    }

    Script::~Script() {
        //duk_pop(m_ctx);
        duk_destroy_heap(m_ctx);
    }

    void Script::updateFixed() {
        duk_push_string(m_ctx, "updateFixed");

        auto rc = duk_pcall_prop(m_ctx, m_obj_idx, 0);

        if (rc != DUK_EXEC_SUCCESS) {
            Utils::logError("Error fixed update in script");
        }

        printf("%s\n", duk_to_string(m_ctx, -1));

        duk_pop(m_ctx);
    }

    void Script::update(double alpha) {
        duk_push_string(m_ctx, "update");
        duk_push_number(m_ctx, alpha);

        auto rc = duk_pcall_prop(m_ctx, m_obj_idx, 1);

        if (rc != DUK_EXEC_SUCCESS) {
            Utils::logError("Error update in script");
        }

        duk_pop(m_ctx);
    }

    void Script::bindEntity(uint32_t entity) {
        duk_push_string(m_ctx, "entity");
        duk_push_number(m_ctx, entity);

        if (duk_put_prop(m_ctx, m_obj_idx) != 1) {
            Utils::logError("Property entity not added");
        }
    }

}

