#include "EventQueue.h"
#include "GameEvent.h"
#include <iostream>

namespace dae
{
    EventQueue& EventQueue::GetInstance()
    {
        static EventQueue instance;
        return instance;
    }

    void EventQueue::PushEvent(std::shared_ptr<Event> event)
    {
        m_Events.push(event);
    }

    void EventQueue::ProcessEvents()
    {
        while (!m_Events.empty())
        {
            auto event = m_Events.front();
            m_Events.pop();

            // Cast to GameEvent so listeners can handle it correctly
            auto gameEvent = std::dynamic_pointer_cast<GameEvent>(event);
            if (gameEvent)
            {
                for (const auto& listener : m_Listeners)
                {
                    listener(gameEvent);
                }
            }
            else
            {
                std::cerr << "[EventQueue] Warning: Received an unknown event type.\n";
            }
        }
    }

    void EventQueue::AddListener(const std::function<void(std::shared_ptr<Event>)>& listener)
    {
        m_Listeners.push_back(listener);
    }
}
