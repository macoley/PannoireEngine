#ifndef PE_UTILS_LISTENER_H
#define PE_UTILS_LISTENER_H

#include <queue>
#include <array>
#include <list>
#include <any>
#include <mutex>

namespace PE::Utils {

    template<typename T, typename Events>
    class Listener {
        static_assert(std::is_enum<Events>::value, "Events template parameter has to be Enum of events");

        using Message = std::pair<Events, const std::any>;
        using Queue = std::queue<Message, std::list<Message>>;
    public:
        template<Events Type, typename Data>
        void addMsg(Data data);

        void dispatch();

    private:
        std::mutex m_queue_mutex;
        Queue m_queue{};
    };

    /**
     * Adding Message
     * @tparam T Listener class
     * @tparam Events Enum of events type
     * @tparam size Size of queue
     * @tparam Type Event type from enum
     * @tparam Data
     */
    template<typename T, typename Events>
    template<Events Type, typename Data>
    void Listener<T, Events>::addMsg(Data data) {
        //if(memcmp ( buffer1, buffer2, sizeof(buffer1) );)

        std::lock_guard guard(m_queue_mutex);
        m_queue.emplace(Type, data);
    }

    /**
     * Detach events
     * @tparam T
     * @tparam Events
     */
    template<typename T, typename Events>
    void Listener<T, Events>::dispatch() {

        std::lock_guard guard(m_queue_mutex);
        while(!m_queue.empty())
        {
            const Message& msg = m_queue.front();
            static_cast<T&>(*this).onMsg(msg.first, msg.second);
            m_queue.pop();
        }
    }

}

#endif //PE_UTILS_LISTENER_H
