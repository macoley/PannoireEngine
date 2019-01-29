#ifndef PE_SCRIPT_SCRIPT_H
#define PE_SCRIPT_SCRIPT_H

#include <fstream>
#include <string>
#include <streambuf>
#include <cassert>
#include <iostream>
#include <sstream>

#include "duktape.h"
#include "dukglue/dukglue.h"

#include "PE/Resource/Resource.h"

#include "ScriptEngine.h"

namespace PE::Scripting {

    class Script : public Resource::IResource<Script> {
        using Source = std::string;
        using EnginePtr = std::shared_ptr<Scripting::ScriptEngine>;

    public:
        struct Behaviour {
            std::function<void(float)> update;
            std::function<void()> fixedUpdate;
        };

        explicit Script(EnginePtr);
        virtual ~Script();

        void load(const std::string& path);
        std::string instantiate(uint32_t);
        void deleteInstance(const std::string &);
        Behaviour getBehaviour(uint32_t);

        std::string getId(uint32_t);

    private:
        EnginePtr m_engine;

        std::string getFileName(const std::string& path);

        std::string m_path;
        std::string m_className;
    };


}

#endif //PE_SCRIPT_SCRIPT_H
