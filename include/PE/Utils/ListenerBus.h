#ifndef PE_UTILS_LISTENERBUS_H
#define PE_UTILS_LISTENERBUS_H

#include <list>
#include <functional>

#include "Listener.h"

namespace PE::Utils {

    template <typename Events>
    class ListenerBus : public Listener<ListenerBus<Events>, Events> {
        using Subscriber = std::function<void (Events, const std::any&)>;
        using Subscribers = std::list<Subscriber>;

    public:
        void onMsg(Events, const std::any&);

        void addSubscriber(Subscriber fnc);

    private:
        Subscribers m_subscribers;
    };

    /**
     * Inform all subscribers
     * @tparam Events
     */
    template<typename Events>
    void ListenerBus<Events>::onMsg(Events event, const std::any & data) {
        for(Subscriber& fnc : m_subscribers)
        {
            fnc(event, data);
        }
    }

    /**
     * Adding subscriber
     * @tparam Events
     * @param fnc
     */
    template<typename Events>
    void ListenerBus<Events>::addSubscriber(ListenerBus::Subscriber fnc) {
        m_subscribers.emplace_back(fnc);
    }


}

#endif //PE_UTILS_LISTENERBUS_H
