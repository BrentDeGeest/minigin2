#pragma once
#include <queue>
#include <memory>
//#include <vector>
#include <functional>

namespace dae
{
    class Event;

    class EventQueue
    {
    public:
        static EventQueue& GetInstance();
        void PushEvent(std::shared_ptr<Event> event);
        void ProcessEvents();
        void AddListener(const std::function<void(std::shared_ptr<Event>)>& listener);

    private:
        std::queue<std::shared_ptr<Event>> m_Events;
        std::vector<std::function<void(std::shared_ptr<Event>)>> m_Listeners;
    };
}
