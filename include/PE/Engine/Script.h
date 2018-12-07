#ifndef PE_ENGINE_SCRIPT_H
#define PE_ENGINE_SCRIPT_H

#include <fstream>
#include <string>
#include <streambuf>
#include <cassert>
#include <iostream>
#include <sstream>

#include "duktape.h"
#include "dukglue/dukglue.h"

#include "PE/Resource/Resource.h"

namespace PE::Engine {

    class Script : public Resource::IResource<Script> {
        using Source = std::string;

    public:
        explicit Script();
        virtual ~Script();

        void load(const std::string& path);
        void updateFixed();
        void update(double alpha);

        void bindEntity(uint32_t entity);

    private:
        duk_idx_t m_obj_idx;
        duk_context * m_ctx;
    };


}

#endif //PE_ENGINE_SCRIPT_H
