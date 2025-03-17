#pragma once
#include <vector>
#include <memory>

namespace dae
{
    class Observer;
    class Event;

    class Subject
    {
    public:
        void AddObserver(std::shared_ptr<Observer> observer);
        void RemoveObserver(std::shared_ptr<Observer> observer);

    protected:
        void Notify(std::shared_ptr<Event> event);

    private:
        std::vector<std::shared_ptr<Observer>> m_Observers;
    };
}
