#ifndef PE_SCRIPTING_SCRIPTING_CPP_H
#define PE_SCRIPTING_SCRIPTING_CPP_H

#include "Script.h"
#include "ScriptEngine.h"

namespace PE::Scripting {

    std::shared_ptr<ScriptEngine> MakeScriptEngine();

}

#endif //PE_SCRIPTING_SCRIPTING_CPP_H
