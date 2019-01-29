
#include <PE/Scripting/Script.h>

namespace PE::Scripting {

    Script::Script(std::shared_ptr<Scripting::ScriptEngine> m_engine)
        : m_engine(std::move(m_engine))
    {

    }

    Script::~Script() {
        //m_engine->unloadModule(m_path);
    }


    void Script::load(const std::string &path) {
        m_engine->loadModule(path);
        m_path = path;
        m_className = getFileName(path);
    }

    std::string Script::getFileName(const std::string &path) {
        std::string className = path;

        // Remove directory
        const size_t last_slash_idx = className.find_last_of("\\/");
        if (std::string::npos != last_slash_idx)
        {
            className.erase(0, last_slash_idx + 1);
        }

        // Remove extension if present.
        const size_t period_idx = className.rfind('.');
        if (std::string::npos != period_idx)
        {
            className.erase(period_idx);
        }

        return className;
    }

    std::string Script::instantiate(uint32_t entity) {
        std::string id = getId(entity);
        m_engine->instantiate(m_path, m_className, id, entity);

        return id;
    }

    Script::Behaviour Script::getBehaviour(uint32_t entity) {
        std::string id = getId(entity);
        Behaviour s;

        s.update = [id, this](float delta) {
            m_engine->getInstanceMethod(id, "update");
            duk_push_number(m_engine->getCtx(), delta);
            duk_call(m_engine->getCtx(), 1);
            duk_pop(m_engine->getCtx());
        };

        s.fixedUpdate = [id, this]() {
            m_engine->getInstanceMethod(id, "fixedUpdate");
            duk_call(m_engine->getCtx(), 0);
            duk_pop(m_engine->getCtx());
        };

        return s;
    }

    std::string Script::getId(uint32_t entity) {
        return {m_className + "_" + std::to_string(entity)};
    }

    void Script::deleteInstance(const std::string &id) {
        m_engine->deleteInstance(id);
    }

}

