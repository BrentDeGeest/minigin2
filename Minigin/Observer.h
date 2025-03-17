#pragma once
#include <memory>

namespace dae
{
    class Event;

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void OnNotify(std::shared_ptr<Event> event) = 0;
    };
}
