
#include <PE/Scripting/Scripting.h>

namespace PE::Scripting {

    std::shared_ptr<ScriptEngine> MakeScriptEngine() {
        return std::make_shared<ScriptEngine>();
    }

}